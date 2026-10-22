#!/usr/bin/env python3
"""Validate a five-document embedded software delivery set without rewriting it."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from urllib.parse import unquote


DOCUMENT_TYPES: Tuple[str, ...] = (
    "需求方案",
    "设计方案",
    "测试说明",
    "协议变更",
    "更新说明",
)
STALE_MARKERS: Tuple[str, ...] = (
    "待定位",
    "完成后补充",
    "TODO",
    "TBD",
)
FILENAME_PATTERN = re.compile(
    r"^(?P<topic>.+?)(?P<doc_type>需求方案|设计方案|测试说明|协议变更|更新说明)"
    r"_(?P<version>V\d+(?:\.\d+)*)_(?P<date>\d{8})\.md$",
    re.IGNORECASE,
)
MARKDOWN_LINK_PATTERN = re.compile(r"\[[^\]]*\]\(([^)]+)\)")
TABLE_SEPARATOR_PATTERN = re.compile(r"^:?-{3,}:?$")


def ParseArgs() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Validate one version of five embedded delivery documents."
    )
    parser.add_argument("directory", type=Path, help="Directory containing the documents")
    parser.add_argument("--topic", help="Required filename topic prefix")
    parser.add_argument("--version", help="Required document version, for example V1.2")
    parser.add_argument(
        "--date",
        help="Required date in YYYYMMDD or YYYY-MM-DD format",
    )
    parser.add_argument("--firmware", help="Firmware string required in every document")
    parser.add_argument("--baseline", help="Baseline string required in every document")
    return parser.parse_args()


def NormalizeDate(value: Optional[str]) -> Optional[str]:
    if value is None:
        return None

    normalized = value.replace("-", "")
    if re.fullmatch(r"\d{8}", normalized) is None:
        raise ValueError("--date must use YYYYMMDD or YYYY-MM-DD")

    return normalized


def SelectDocuments(
    directory: Path,
    topic: Optional[str],
    version: Optional[str],
    date: Optional[str],
) -> Tuple[Dict[str, Path], List[str]]:
    errors: List[str] = []
    selected: Dict[str, Path] = {}

    for doc_type in DOCUMENT_TYPES:
        candidates: List[Path] = []

        for path in directory.glob("*.md"):
            match = FILENAME_PATTERN.match(path.name)
            if match is None or match.group("doc_type") != doc_type:
                continue
            if topic is not None and match.group("topic") != topic:
                continue
            if version is not None and match.group("version").lower() != version.lower():
                continue
            if date is not None and match.group("date") != date:
                continue
            candidates.append(path)

        if len(candidates) == 0:
            errors.append(f"missing {doc_type} document")
        elif len(candidates) > 1:
            names = ", ".join(sorted(path.name for path in candidates))
            errors.append(f"ambiguous {doc_type} documents: {names}")
        else:
            selected[doc_type] = candidates[0]

    return selected, errors


def CheckRelativeLinks(path: Path, text: str) -> List[str]:
    errors: List[str] = []

    for raw_target in MARKDOWN_LINK_PATTERN.findall(text):
        raw_target = raw_target.strip()
        if raw_target.startswith("<") and ">" in raw_target:
            target = raw_target[1:raw_target.index(">")]
        else:
            target = raw_target.split(maxsplit=1)[0]
        if not target or target.startswith(("#", "http://", "https://", "mailto:")):
            continue
        if re.match(r"^[A-Za-z][A-Za-z0-9+.-]*:", target) is not None:
            continue

        link_path = unquote(target.split("#", maxsplit=1)[0])
        if not link_path:
            continue
        if Path(link_path).is_absolute():
            errors.append(f"{path.name}: absolute local link is not allowed: {target}")
            continue
        if not (path.parent / link_path).resolve().exists():
            errors.append(f"{path.name}: unresolved relative link: {target}")

    return errors


def CheckTextFile(path: Path, text: str) -> Tuple[List[str], List[str]]:
    errors: List[str] = []
    warnings: List[str] = []
    raw_data = path.read_bytes()

    if raw_data.startswith(b"\xef\xbb\xbf"):
        warnings.append(f"{path.name}: UTF-8 BOM detected")
    if raw_data and not raw_data.endswith(b"\n"):
        errors.append(f"{path.name}: missing newline at end of file")

    for line_number, line in enumerate(text.splitlines(), start=1):
        if line.rstrip(" \t") != line:
            errors.append(f"{path.name}:{line_number}: trailing whitespace")
        for marker in STALE_MARKERS:
            if marker in line:
                errors.append(
                    f"{path.name}:{line_number}: stale placeholder marker '{marker}'"
                )

    errors.extend(CheckRelativeLinks(path, text))
    return errors, warnings


def ExtractMarkdownTable(text: str, heading_name: str) -> Tuple[List[str], List[List[str]]]:
    lines = text.splitlines()
    heading_index: Optional[int] = None

    for index, line in enumerate(lines):
        if re.match(r"^#{2,6}\s+", line) is not None and heading_name in line:
            heading_index = index
            break

    if heading_index is None:
        return [], []

    table_lines: List[str] = []
    for line in lines[heading_index + 1:]:
        if re.match(r"^#{1,6}\s+", line) is not None:
            break
        if line.strip().startswith("|"):
            table_lines.append(line.strip())
        elif table_lines and line.strip():
            break

    parsed_rows: List[List[str]] = []
    for line in table_lines:
        parsed_rows.append([cell.strip() for cell in line.strip("|").split("|")])

    if len(parsed_rows) < 2:
        return [], []
    if not all(TABLE_SEPARATOR_PATTERN.fullmatch(cell) for cell in parsed_rows[1]):
        return [], []

    return parsed_rows[0], parsed_rows[2:]


def ExtractRequirementIds(rows: List[List[str]]) -> Tuple[set[str], List[str]]:
    requirement_ids: set[str] = set()
    duplicates: List[str] = []

    for row in rows:
        if not row:
            continue
        requirement_id = row[0].strip()
        if re.fullmatch(r"\d+", requirement_id) is None:
            continue
        if requirement_id in requirement_ids:
            duplicates.append(requirement_id)
        requirement_ids.add(requirement_id)

    return requirement_ids, duplicates


def CheckTraceability(
    documents: Dict[str, Path],
    document_texts: Dict[str, str],
) -> List[str]:
    errors: List[str] = []
    table_specs = {
        "需求方案": ("需求清单总览", "序号"),
        "设计方案": ("需求设计映射", "需求编号"),
        "测试说明": ("需求追溯矩阵", "需求编号"),
    }
    requirement_sets: Dict[str, set[str]] = {}
    test_header: List[str] = []
    test_rows: List[List[str]] = []

    for doc_type, (heading_name, first_column) in table_specs.items():
        text = document_texts.get(doc_type)
        if text is None:
            continue
        header, rows = ExtractMarkdownTable(text, heading_name)
        if not header or header[0] != first_column:
            errors.append(
                f"{documents[doc_type].name}: missing '{heading_name}' table "
                f"with first column '{first_column}'"
            )
            continue

        requirement_ids, duplicates = ExtractRequirementIds(rows)
        if not requirement_ids:
            errors.append(f"{documents[doc_type].name}: no requirement identifiers found")
        if duplicates:
            errors.append(
                f"{documents[doc_type].name}: duplicate requirement identifiers: "
                f"{', '.join(sorted(set(duplicates), key=int))}"
            )
        requirement_sets[doc_type] = requirement_ids

        if doc_type == "测试说明":
            test_header = header
            test_rows = rows

    if len(requirement_sets) == len(table_specs):
        baseline_ids = requirement_sets["需求方案"]
        for doc_type in ("设计方案", "测试说明"):
            current_ids = requirement_sets[doc_type]
            if current_ids != baseline_ids:
                missing = sorted(baseline_ids - current_ids, key=int)
                extra = sorted(current_ids - baseline_ids, key=int)
                errors.append(
                    f"{documents[doc_type].name}: requirement identifier set differs; "
                    f"missing={missing}, extra={extra}"
                )

    required_test_columns = (
        "设计位置",
        "测试用例",
        "当前验证/处置状态",
        "剩余验证或阻塞",
    )
    if test_header and all(column in test_header for column in required_test_columns):
        column_indexes = {column: test_header.index(column) for column in required_test_columns}
        for row in test_rows:
            if not row or re.fullmatch(r"\d+", row[0].strip()) is None:
                continue
            requirement_id = row[0].strip()
            for column, column_index in column_indexes.items():
                if column_index >= len(row) or not row[column_index].strip():
                    errors.append(
                        f"{documents['测试说明'].name}: requirement {requirement_id} "
                        f"has empty '{column}'"
                    )

            test_case = row[column_indexes["测试用例"]].strip()
            state = row[column_indexes["当前验证/处置状态"]].strip()
            if test_case == "不适用":
                if "不实施" not in state and "暂停" not in state:
                    errors.append(
                        f"{documents['测试说明'].name}: requirement {requirement_id} "
                        "uses '不适用' without an unimplemented or postponed state"
                    )
            elif re.search(r"TC-\d+", test_case) is None:
                errors.append(
                    f"{documents['测试说明'].name}: active requirement {requirement_id} "
                    "does not reference a TC test case"
                )
    elif test_header:
        missing_columns = [
            column for column in required_test_columns if column not in test_header
        ]
        errors.append(
            f"{documents['测试说明'].name}: traceability matrix missing columns: "
            f"{', '.join(missing_columns)}"
        )

    protocol_text = document_texts.get("协议变更")
    if protocol_text is not None:
        protocol_header, _ = ExtractMarkdownTable(protocol_text, "变更总览")
        if "关联需求" not in protocol_header:
            errors.append(f"{documents['协议变更'].name}: change summary missing '关联需求'")

    summary_text = document_texts.get("更新说明")
    if summary_text is not None and "需求追溯矩阵" not in summary_text:
        errors.append(f"{documents['更新说明'].name}: missing traceability matrix entry")

    return errors


def ValidateDocuments(args: argparse.Namespace) -> Tuple[List[str], List[str]]:
    errors: List[str] = []
    warnings: List[str] = []
    directory = args.directory.resolve()

    if not directory.is_dir():
        return [f"document directory does not exist: {directory}"], warnings

    try:
        requested_date = NormalizeDate(args.date)
    except ValueError as error:
        return [str(error)], warnings

    documents, selection_errors = SelectDocuments(
        directory,
        args.topic,
        args.version,
        requested_date,
    )
    errors.extend(selection_errors)
    if selection_errors:
        return errors, warnings

    versions = set()
    dates = set()
    topics = set()

    document_texts: Dict[str, str] = {}

    for doc_type, path in documents.items():
        match = FILENAME_PATTERN.match(path.name)
        if match is None:
            errors.append(f"{path.name}: invalid filename")
            continue
        versions.add(match.group("version").lower())
        dates.add(match.group("date"))
        topics.add(match.group("topic"))

        try:
            text = path.read_text(encoding="utf-8")
        except UnicodeDecodeError as error:
            errors.append(f"{path.name}: not valid UTF-8: {error}")
            continue

        document_texts[doc_type] = text

        file_errors, file_warnings = CheckTextFile(path, text)
        errors.extend(file_errors)
        warnings.extend(file_warnings)

        file_version = match.group("version")
        file_date = match.group("date")
        display_date = f"{file_date[:4]}-{file_date[4:6]}-{file_date[6:]}"
        if file_version.lower() not in text.lower():
            errors.append(f"{path.name}: filename version {file_version} not found in content")
        if file_date not in text and display_date not in text:
            errors.append(f"{path.name}: filename date {display_date} not found in content")
        if args.firmware is not None and args.firmware.lower() not in text.lower():
            errors.append(f"{path.name}: firmware '{args.firmware}' not found in content")
        if args.baseline is not None and args.baseline.lower() not in text.lower():
            errors.append(f"{path.name}: baseline '{args.baseline}' not found in content")

    if len(versions) != 1:
        errors.append(f"document versions differ: {', '.join(sorted(versions))}")
    if len(dates) != 1:
        errors.append(f"document dates differ: {', '.join(sorted(dates))}")
    if len(topics) != 1:
        errors.append(f"document topics differ: {', '.join(sorted(topics))}")

    for doc_type, path in documents.items():
        text = document_texts.get(doc_type)
        if text is None:
            continue

        linked_types = {
            linked_type
            for linked_type, linked_path in documents.items()
            if linked_type != doc_type and linked_path.name in text
        }
        if not linked_types:
            errors.append(f"{path.name}: does not link to another delivery document")

    summary_text = document_texts.get("更新说明")
    if summary_text is not None:
        for linked_type, linked_path in documents.items():
            if linked_type == "更新说明":
                continue
            if linked_path.name not in summary_text:
                errors.append(f"{documents['更新说明'].name}: missing link to {linked_path.name}")

    errors.extend(CheckTraceability(documents, document_texts))

    return errors, warnings


def Main() -> int:
    args = ParseArgs()
    errors, warnings = ValidateDocuments(args)

    for warning in warnings:
        print(f"[WARN] {warning}")
    for error in errors:
        print(f"[ERROR] {error}")

    if errors:
        print(f"Validation failed: {len(errors)} error(s), {len(warnings)} warning(s).")
        return 1

    print(f"Validation passed: 5 documents, {len(warnings)} warning(s).")
    return 0


if __name__ == "__main__":
    sys.exit(Main())
