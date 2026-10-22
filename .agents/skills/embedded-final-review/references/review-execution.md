# Final Review Execution Contract

## Candidate Sources

Use one explicit source and name it in the report.

| Review source | Diff source | Candidate file content |
|---|---|---|
| Staged pre-commit candidate | `git diff --cached` | Git index, for example `git show :path` |
| Single commit | `git show <commit>` or `<commit>^!` | `<commit>:path` |
| Commit range | `git diff <base>..<head>` | `<head>:path` |
| Branch or PR comparison | `git diff <target>...<source>` | `<source>:path` |

Do not mix two-dot and three-dot semantics without explaining the intended baseline. Use three-dot for a feature branch compared from its merge base when that matches the repository workflow.

An unstaged working-tree diff can be reviewed when the user explicitly requests it, but it is not a final pre-commit gate. Label it as a provisional review and require a new staged review before commit.

## Candidate Fingerprint

Fingerprint the binary diff using the same source as the review:

```powershell
git diff --cached --binary | git hash-object --stdin
```

For another source, replace `--cached` with the exact commit or branch range. Record the endpoint commit IDs as well as the hash. Recompute both before the final decision.

The review is invalid when any of these changes:

- Diff fingerprint or candidate file list.
- Staged selection or reviewed commit endpoints.
- Build configuration, protocol resources, generated source, fonts, or binary resources in the candidate.
- Requirement, design, test, protocol, or release evidence used to justify the gate.

## Candidate-Accurate Context

The working tree may contain changes outside the candidate. Keep candidate behavior separate from those changes:

- Use index or endpoint content for changed candidate files.
- Use the corresponding candidate tree for interfaces and callers whenever practical.
- Report unstaged changes in a staged file because working-tree line numbers and behavior may differ from the candidate.
- Do not include untracked files in a final staged review; they must be deliberately staged before they can be reviewed as submission content.

Expand context according to the change:

- GUI: parent window, template, callbacks, refresh, navigation, cache, save, cancel, and destruction.
- Configuration: type layout, defaults, validation, cache, formal configuration, persistence, migration, and protocol serialization.
- Control: state calculation, all activation and shutdown paths, manual and automatic modes, alarms, calibration, empty or disabled modes, and physical outputs.
- Protocol: producers, consumers, command direction, fields, lengths, buffers, sequence IDs, ACK, timeout, retry, concurrency, platform, and external firmware.
- Build or resources: project entries, target variants, generated inputs, encodings, fonts, language resources, and binary provenance.

These context expansions supplement the repository's review rules; they do not replace its risk checklist.

## Evidence Standard

Report a defect only when code and context establish a realistic trigger and consequence. A good finding contains:

- Candidate file and accurate post-change line or function.
- Input, state, timing, configuration, or failure condition that triggers it.
- Observable impact and affected scope.
- Severity justified by likelihood, impact, and recoverability.
- Minimal repair direction without unrelated redesign.

Separate these from defects:

- Unverified hardware or platform behavior.
- Missing validation evidence.
- Ambiguous requirements.
- Optional refactoring or style preferences.

They may still block a release or review gate when repository policy requires the missing evidence, but do not mislabel them as proven code failures.

## Final Report Facts

Always include:

- Review source, branch, base and head commits, and diff fingerprint.
- Candidate file list and any outside worktree changes relevant to submission boundaries.
- Findings in the repository-required format and severity order.
- Review limitations, validation gaps, and external dependencies.
- One explicit gate decision.
- A statement that any later candidate change invalidates the decision.
