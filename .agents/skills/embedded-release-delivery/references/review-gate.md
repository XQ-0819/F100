# Review and Delivery Gate

## Workflow Order

Use this order unless the repository defines a stricter process:

1. Confirm requirements and design.
2. Implement the function.
3. Perform developer self-checks and layered validation.
4. Synchronize the five delivery documents.
5. Clean generated artifacts and group only the intended submission files.
6. Perform an independent, read-only final code review on the exact candidate diff.
7. Fix findings, repeat affected validation and document synchronization, then review the entire candidate diff again.
8. Commit only after review passes and authorization exists.
9. Prepare PR title and description from the committed comparison range, recording the exact range and included commits without reopening the five documents solely to backfill commit identifiers.

## Separation of Responsibilities

This Skill maintains delivery documents and prepares PR text. It does not perform the final code review. Read and follow the repository's dedicated review rules in a separate review task or review mode.

The review becomes invalid when any candidate file, code block, build setting, protocol resource, font, generated binary, or staged selection changes after review. A partial recheck does not restore validity.

## Blocking Behavior

- High-risk findings block commit and PR readiness.
- Medium-risk findings require correction or an explicit, evidence-backed acceptance decision under repository policy.
- Low-risk recommendations do not normally block, but must not hide functional risk.
- A missing, stale, or mismatched review must be reported. Do not generate PR copy labeled ready to submit.

Review success does not prove target-board, platform, external-device, or release acceptance. Carry those gaps into the test document, release summary, and PR description.
