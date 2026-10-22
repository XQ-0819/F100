---
name: embedded-release-delivery
description: Generate or update embedded software delivery artifacts from repository evidence, including requirement, design, test, protocol-change, release-summary, and cross-document requirement traceability, and prepare PR titles and descriptions from actual Git commits and diffs. Use for stage closeout, release-document synchronization, traceability maintenance, delivery-document consistency checks, or PR copy preparation in embedded repositories. Do not use for code implementation, code review, Git commit, push, or PR creation.
---

# Embedded Release Delivery

Use repository evidence to maintain a consistent five-document delivery set and, after the final review gate, prepare factual PR copy.

## Establish Repository Context

1. Locate the repository root and read its native AI instructions, such as `AGENTS.md`.
2. Follow those instructions to find and read the project base guide, current versioned delivery documents, and team SOP. Do not rely on remembered paths from an earlier conversation.
3. Inspect the current branch, `git status`, relevant commits, and the requested diff range. Treat existing worktree changes as user-owned.
4. Identify the target firmware, document version, delivery date, comparison baseline, implementation state, and validation evidence from repository facts. Ask only when a material value cannot be established safely.
5. Never invent a branch, commit, test result, target-board result, platform result, or release state.

## Choose the Workflow

### Maintain Delivery Documents

Read [document-contracts.md](references/document-contracts.md) completely before creating or updating documents.

1. Compare confirmed requirements, actual code and configuration changes, Git evidence, and validation records.
2. Create or update the five documents: requirement, design, test, protocol change, and release summary.
3. Keep one version, date, target firmware, and comparison baseline across the set. Use relative links between documents.
4. Preserve stable requirement identifiers. Add a requirement-to-design mapping in the design document, keep the only complete requirement-to-design-to-test-to-result traceability matrix in the test document, add associated requirement identifiers to protocol changes, and link to the matrix from the release summary.
5. Remove superseded proposals, rejected implementation details, stale placeholders, and status claims contradicted by current code.
6. State explicitly when the release has no protocol change; still record the inspected protocol scope and baseline.
7. Update the project base guide only when stable architecture, common mechanisms, toolchains, or document entry points changed.
8. Record only branch, baseline, existing commit, or candidate-worktree facts available before submission. Do not invent a future final commit or reopen the reviewed five-document set only to backfill it; associate the final commit range in the later PR description.
9. Run `scripts/validate_delivery_docs.py` against the completed document directory, then inspect the Git diff and fix all errors. Treat warnings according to repository policy.

Example:

```powershell
py .agents/skills/embedded-release-delivery/scripts/validate_delivery_docs.py `
    doc/release-topic --topic "ReleaseTopic" --version V1.0 `
    --date 20260818 --firmware V3.2.0
```

### Prepare PR Copy

Read [review-gate.md](references/review-gate.md) and [pr-template.md](references/pr-template.md) completely before preparing PR text.

1. Confirm the repository's independent final code review passed for the exact candidate diff and that the reviewed changes were committed without later modification.
2. Establish the target branch, source branch, exact comparison range, and included commit list. Prefer actual `target...source` commits and diffs over chat summaries.
3. Read the five delivery documents and compare their statements with the committed diff and recorded validation.
4. Produce a concise PR title and the sections defined in `pr-template.md`, including the actual comparison range and commit list that bind the five documents to the submitted source.
5. If the review is missing, blocking, stale, or covers another range, report the gap and do not label the PR copy ready to submit.

### Check an Existing Delivery Set

Run the validation script first, then inspect semantic consistency that automation cannot prove: business scope, implementation status, compatibility, validation level, external dependencies, and cross-document links.

## Preserve Workflow Boundaries

- Keep code review independent and read-only under the repository's review rules. This Skill does not replace or perform that review.
- Do not modify business code while closing documents unless the user separately authorizes implementation.
- Do not stage files, create commits, push branches, create PRs, assign reviewers, or send notifications without separate explicit authorization.
- Do not delete historical documents unless the repository policy and user request clearly authorize replacement or archival.
- Do not modify the reviewed five-document set solely to add the final commit identifier. Record that identifier and comparison range in PR text after commit.
- Distinguish code inspection, static checks, unit tests, host builds, GUI simulation, target toolchain builds, target-board tests, and platform or joint-device validation.
- Report incomplete external validation and release blockers plainly.

## Report the Result

Summarize:

- Documents created or updated, or the PR title and description produced.
- Evidence and comparison range used.
- Validation commands and results.
- Unresolved inconsistencies, unverified environments, external dependencies, and whether the result is ready for the next workflow gate.
