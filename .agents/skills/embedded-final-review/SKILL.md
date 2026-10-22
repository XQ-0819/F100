---
name: embedded-final-review
description: Perform an independent, read-only final code review of embedded software candidate changes, including staged diffs, commits, commit ranges, or branch comparisons. Use after implementation, self-checks, layered validation, documentation synchronization, artifact cleanup, and submission file grouping, as the final quality gate before a Git commit or PR handoff. Read repository review rules and project evidence, inspect complete call chains, and report evidence-backed risks and a gate decision. Do not use to modify files, change the index, commit, push, create PRs, or replace target-board and platform validation.
---

# Embedded Final Review

Review the exact candidate change independently and issue a reproducible gate decision without changing repository state.

## Preserve Read-Only Boundaries

- Do not edit, create, delete, rename, format, stage, unstage, commit, push, or clean files.
- Do not run builds, tests, generators, simulators, or cleanup commands unless the user separately authorizes them; these may create artifacts or change external state.
- Allow read-only Git inspection, repository searches, file reads, and non-mutating checks such as `git diff --check`.
- Provide findings and minimal repair guidance. Modify code only in a later task after explicit authorization.
- Never describe review success as build, target-board, platform, external-device, or release acceptance.

## Establish the Exact Candidate

Read [review-execution.md](references/review-execution.md) completely before inspecting a candidate.

1. Use the review range explicitly supplied by the user.
2. For a final pre-commit review with no explicit range, use `git diff --cached` only when the staged candidate is non-empty.
3. Do not silently treat unstaged worktree changes as the final candidate. If there is no staged candidate and no explicit commit or branch range, report that the final-review scope is undefined.
4. Record the branch, `HEAD`, status, file list, and a fingerprint of the exact diff before analysis.
5. Report staged, unstaged, and untracked changes outside the candidate so the user can see submission-boundary risks.

For a staged candidate, use read-only commands equivalent to:

```powershell
git status --short
git rev-parse --abbrev-ref HEAD
git rev-parse HEAD
git diff --cached --name-status
git diff --cached --stat
git diff --cached --binary | git hash-object --stdin
```

Use the corresponding explicit range in place of `--cached` for commit or branch reviews.

## Load Authoritative Context

1. Read the repository's native instructions, such as root `AGENTS.md`.
2. Follow those instructions to locate and completely read the active code-review rules. Do not assume `.team/` or another arbitrary directory is auto-loaded.
3. Read the project base or AI development guide and the latest team development SOP when required by repository instructions.
4. Read the requirement, design, test, protocol-change, and release-summary documents relevant to the candidate.
5. If the required review rules are missing, renamed ambiguously, or contradictory, report the rule problem and do not claim the final gate passed.
6. Treat current repository code, configuration, reproducible evidence, and the user's latest confirmed conclusions as authoritative under repository priority rules.

## Inspect the Candidate

1. Run `git diff --check` for the exact candidate range, then inspect name status, statistics, binary changes, renames, and the complete patch.
2. Read candidate versions rather than accidentally reading later worktree content. For staged tracked files, use the index form such as `git show :path/to/file` when exact candidate content matters.
3. Expand from each changed block to complete functions, callers, callees, data structures, configuration, build entries, save paths, protocol endpoints, and error or cleanup paths.
4. Compare behavior with confirmed requirements and established local implementations. Do not infer correctness or defects from names alone.
5. Apply the repository's review checklist according to the actual change type. Do not manufacture findings merely to cover every checklist item.
6. Check whether candidate files match the intended feature and whether required headers, project files, resources, documents, or protocol counterparts are missing.
7. Check validation records and distinguish every validation layer accurately. List unperformed or externally blocked validation as residual risk, not as a code defect unless it prevents a required gate.
8. Prefer one well-supported root-cause finding over several duplicates. Every finding must include location, trigger, consequence, severity rationale, and the smallest viable repair direction.

## Issue the Gate Decision

1. Follow the output structure and severity definitions from the active repository review rules.
2. Lead with findings ordered by severity. If there are no findings, state that clearly and list remaining test gaps or residual risks.
3. Use the repository's required gate vocabulary. If none is defined, use:
   - Critical: blocks submission.
   - Major: requires repair or an explicit evidence-backed acceptance decision, followed by a new full review.
   - Good: no blocking finding in the reviewed candidate.
4. Recompute the same diff fingerprint and candidate file list immediately before the final decision.
5. If the fingerprint, file list, index, or specified endpoint changed, mark the review invalid and do not issue a passing final gate.
6. State the exact scope and fingerprint in the report so a later submitter can confirm the reviewed candidate is unchanged.

## Keep the Gate Separate

- A repair invalidates the prior review. Return to implementation, affected validation, documentation synchronization, file grouping, and a complete new final review.
- A passing review authorizes neither Git commit nor external actions. Commit, push, PR creation, reviewer assignment, and notifications require their own authorization.
- PR copy preparation occurs only after the reviewed candidate is committed without change.
