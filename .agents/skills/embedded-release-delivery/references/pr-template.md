# PR Title and Description Contract

## Required Evidence

Establish these facts before drafting:

- Target branch and source branch.
- Exact commit or comparison range.
- Commits included in the PR.
- Files and behavior changed by the actual diff.
- Relevant five-document delivery set.
- Final review result for the same candidate diff.
- Validation performed and validation still pending.

Do not include unrelated worktree changes, later plans, abandoned approaches, or chat-only claims.

## Title

Use one searchable subject. Prefer the repository's existing language and commit convention. A useful pattern is:

```text
<type>: <primary delivered outcome>
```

Keep the title concise. Do not list every small fix or claim release readiness unless that state is established.

## Description

Use the following structure, adjusting only where the repository has a stricter PR template:

```markdown
## 提交范围

- 目标分支：<target branch>
- 来源分支：<source branch>
- 比较范围：<exact comparison range>
- 提交列表：<commits included in this PR>

## 变更背景

说明本 PR 解决的问题、需求来源和交付范围。

## 主要修改

- 只列实际差异中已实现的主要行为。

## 影响与兼容

- 说明产品、硬件、配置、存储、协议、外设固件和平台影响。
- 明确保持不变的关键行为及升级要求。

## 验证结果

- 按静态检查、单元测试、宿主编译、模拟器、目标工具链、目标板、平台或联合设备分层列出。

## 未验证与风险

- 列出尚未执行的验证、外部依赖、已接受风险和发布限制。

## 文档与协议

- 链接需求方案、设计方案、测试说明、协议变更和更新说明。
```

When useful, append a compact file-area summary, but do not turn the PR description into a raw diff dump.

Use the submission range above as the final commit association for the five delivery documents. Do not require a separate document-only change merely to backfill the final commit identifier.

## Readiness Label

Only describe the text as ready to submit when:

- The final review passed for the exact committed range.
- No later diff invalidated that review.
- The five documents agree with the code and commits.
- Blocking validation or rollout requirements are stated accurately.

Generating text never authorizes pushing, opening the PR, assigning reviewers, or notifying external teams.
