# Five-Document Delivery Contract

## Shared Metadata

Use the repository's established metadata labels and local Markdown style. All five documents in one delivery set must agree on:

- Document version.
- Delivery or publication date.
- Target firmware or software version.
- Comparison baseline, such as a release, branch, or commit.
- Product and hardware scope.
- Implementation and validation state.

Use filenames in this form unless the repository defines another active convention:

```text
<topic>需求方案_V<version>_<YYYYMMDD>.md
<topic>设计方案_V<version>_<YYYYMMDD>.md
<topic>测试说明_V<version>_<YYYYMMDD>.md
<topic>协议变更_V<version>_<YYYYMMDD>.md
<topic>更新说明_V<version>_<YYYYMMDD>.md
```

Link the five documents with relative Markdown links. Do not use local absolute paths. Use a stable project-base-document link where the repository provides one.

Use one stable requirement identifier across all five documents. The requirement document owns the identifier; never renumber an established requirement merely to make a table contiguous. The design document maps every identifier to stable design sections and implementation records. The test document owns the only complete traceability matrix from requirement identifier through design location and test case to current verification or disposition. Protocol changes add associated requirement identifiers, while the release summary links to the matrix and summarizes its state instead of copying it.

## Requirement Document

Answer what must be implemented and accepted. Include:

- Purpose, source, product scope, and priority of conclusions.
- Confirmed business rules and applicable preconditions.
- Normal, cancellation, failure, recovery, and restart behavior.
- Units, ranges, defaults, capacity, permissions, and mode boundaries.
- Compatibility expectations for existing configuration and external systems.
- Explicit exclusions, postponed items, and unchanged behavior.
- Verifiable acceptance criteria and implementation status without code-level speculation.
- Stable requirement identifiers that remain unchanged across document revisions.

Do not retain rejected requirements or implementation detail that belongs only in design.

## Design Document

Answer how the confirmed requirement is actually implemented. Include:

- Architecture and complete affected call chains.
- Modified modules, files, interfaces, data flow, and state ownership.
- Existing APIs, templates, save paths, timers, dialogs, protocol handlers, and common mechanisms reused.
- Necessary new interfaces and why existing mechanisms could not satisfy the requirement.
- Storage layout, migration, defaulting, compatibility, concurrency, lifecycle, and failure handling.
- Protocol or external-device impact with links to the protocol-change document.
- Implementation status and development validation at the exact level performed.
- A complete requirement-to-design mapping using stable section, file, and function references rather than volatile line numbers.

Remove design text that no longer matches the current implementation. Do not describe a proposal as implemented.

## Test Document

Answer how delivery staff can verify the change. Include:

- Firmware, hardware, simulator, platform, external-device, and toolchain prerequisites.
- Traceability from each requirement to test cases.
- Normal, boundary, invalid-input, cancellation, failure, recovery, restart, and long-duration cases.
- Regression scope for shared templates, storage, GUI, control outputs, timing, and protocols.
- Expected results that can be observed by testers rather than only internal code states.
- Validation already performed, remaining target-board or platform work, blockers, and release conditions.
- One complete traceability matrix containing requirement identifier, requirement summary, design location, test case or explicit non-applicable disposition, current verification or disposition state, and remaining validation or blocker.

Never promote a static check, host build, or simulator result to target-board or end-to-end acceptance.

## Protocol-Change Document

Answer what platform, external-device, or integration teams must change relative to the baseline. Include:

- Comparison baseline and affected endpoints.
- Message or command identifiers, direction, trigger, response, and sequence semantics.
- Field order, offset, width, byte order, unit, scale, range, enum, and default meaning.
- Old and new lengths, worst-case payload size, buffer impact, checksums, timeouts, retries, and ACK behavior where changed.
- Compatibility, rollout order, external ownership, and joint-test checklist.
- Source-code locations or formal protocol references that support the stated change.
- Associated requirement identifiers for every changed protocol item.

If no protocol changed, state "本版本无协议变化" and list the code paths and interfaces checked. Do not copy the entire unchanged protocol into this document.

## Release-Summary Document

Answer what the version delivers and whether it can advance. Include:

- Target version, baseline, available branch, existing commit, or candidate-worktree facts, product and hardware scope.
- Completed, excluded, postponed, and regression-only items.
- Compatibility and upgrade notes for configuration, storage, protocol, external firmware, and platform.
- Validation evidence separated by level.
- Remaining risks, blockers, external dependencies, and release restrictions.
- Relative links to all five documents and other authoritative delivery artifacts.
- A direct entry to the test document's complete requirement traceability matrix and a concise coverage-state summary.

Use this document as the entry point, not as a duplicate of all requirement and design details.

Do not require a final commit identifier before that commit exists, and do not reopen an already reviewed delivery set solely to backfill it. Bind the final committed range to this delivery set in the later PR description.

## Status Vocabulary

Use precise states such as:

- 需求已确认
- 方案已确认
- 已编码
- 静态检查通过
- 单元测试通过
- 宿主编译通过
- 模拟器验证通过
- 目标工具链构建通过
- 目标板验证通过
- 平台或联合设备验证通过
- 验收通过
- 不实施
- 暂缓
- 受外部依赖阻塞

These states are not interchangeable. Always name unperformed validation.

## Consistency Pass

Before closing the document task:

1. Confirm names, versions, dates, firmware, baseline, scope, and links agree.
2. Search for old filenames, superseded version numbers, stale branches, and obsolete implementation text.
3. Search for placeholders such as `待定位`, `完成后补充`, `TODO`, and `TBD`.
4. Confirm each completed claim has code or validation evidence.
5. Confirm protocol, storage, and external dependencies are consistent across design, test, protocol, and release summary.
6. Inspect the final Git diff for unrelated edits and formatting noise.
7. Confirm the requirement, design mapping, and test traceability matrix contain the same identifier set, and that every active requirement has a test case and non-empty verification state.
