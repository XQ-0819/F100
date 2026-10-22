---
name: embedded-feature-plan
description: Investigate an embedded software feature before coding and produce an evidence-backed minimum-change implementation plan. Use when a user asks for analysis, assessment, impact evaluation, a solution proposal, or plan confirmation before implementation, especially for GUI, configuration, persistence, control, RTOS, protocol, or cross-module changes. Read repository rules and current project documents, trace complete call and data flows, inventory existing APIs and similar implementations, define necessary and unchanged scope, identify compatibility and validation needs, and surface only material questions. Do not use to modify code, documents, configuration, Git state, or external systems, and do not present the plan as implemented or validated.
---

# Embedded Feature Plan

Investigate the current repository and produce a reviewable minimum-change plan before implementation begins.

## Preserve Planning Boundaries

- Keep the task read-only. Do not edit code, documents, project files, generated resources, or Git state.
- Do not run builds, tests, simulators, generators, cleanup scripts, or external actions unless the user separately authorizes diagnostic execution.
- Do not create placeholder functions, speculative abstractions, compatibility branches, or test hooks while planning.
- Separate confirmed requirements, code facts, assumptions, open questions, and optional improvements.
- Do not describe a proposal as encoded, built, tested, or accepted.

## Establish the Planning Baseline

Read [planning-execution.md](references/planning-execution.md) completely before investigating the feature.

1. Restate the requested behavior, applicable product or mode, acceptance boundary, and explicitly unchanged behavior.
2. Read the repository's native instructions, project base or AI development guide, current versioned requirement and design documents, and the team SOP required by those instructions.
3. Inspect the current branch and worktree status. Treat existing changes as user-owned and account for them without reverting or absorbing them into the plan.
4. Identify the current requirement priority. The user's latest confirmed conclusion overrides older proposals and stale documents.
5. Ask early only when a missing answer changes storage layout, protocol compatibility, control safety, permission behavior, external ownership, or the proposed implementation path.

## Trace Current Behavior

1. Locate every user, task, protocol, or hardware entry relevant to the feature.
2. Trace the complete call and data flow from input through cache or state calculation to persistence, communication, GUI presentation, or physical output.
3. Read complete functions and their callers and callees. Do not form a plan from a single matching line or screenshot.
4. Find all definitions, declarations, call sites, array capacities, structure users, initialization, validation, copy, save, load, serialization, and cleanup paths touched by the data.
5. Compare the target with mature pages, modules, APIs, templates, and historical behavior in the current repository. Use Git history as supporting evidence, not as authority over current requirements and code.
6. Identify task, interrupt, timer, callback, DMA, GUI lifecycle, and external-device contexts that own or access affected state.

## Decide What to Reuse

1. Search for existing APIs and mechanisms before proposing a new function, state variable, timer, dialog, save path, parser, or common abstraction.
2. Record each relevant existing mechanism and conclude whether it can be reused directly, reused with a minimal extension, or cannot satisfy the requirement.
3. Give a concrete reason when an existing interface cannot be used.
4. Preserve established callback order, page navigation, selection, refresh timing, cache and save semantics, error handling, control priority, and protocol behavior unless the requirement explicitly changes them.
5. Separate necessary feature changes from incidental defects or refactoring ideas. Do not include unrelated work without user confirmation.

## Produce the Minimum-Change Plan

Present the plan in this order:

1. **Requirement conclusion**: confirmed behavior, preconditions, boundaries, and material open questions.
2. **Current implementation**: entry points, call chain, data flow, state ownership, and existing behavior with file and function evidence.
3. **Reuse inventory**: existing APIs, templates, save paths, timers, dialogs, protocol handlers, and comparable implementations, with reuse conclusions.
4. **Required changes**: exact files, functions, data, and behavior to modify, ordered by implementation dependency.
5. **Unchanged scope**: structures, protocols, public templates, callbacks, persistence, hardware outputs, and other behavior that must remain untouched.
6. **Compatibility and failure behavior**: old configuration, old platform or external firmware, restart, cancellation, save failure, timeout, concurrency, and safe-state handling where applicable.
7. **Validation plan**: static checks, unit tests, host build, simulator, target toolchain, target board, platform, and joint-device coverage, distinguishing what can and cannot be verified locally.
8. **Implementation and submission breakdown**: safe coding order, independently reviewable units, documentation impact, and suggested commit boundaries without staging or committing.

Use concrete repository names only when supported by the current code. Prefer a small plan that reuses proven mechanisms over a generalized redesign.

## End at the Confirmation Gate

- Mark the result as `Ready for confirmation` only when the call chain, reuse conclusion, impact surface, compatibility, and validation needs are sufficiently established.
- Mark it as `Blocked for clarification` when a material product, safety, storage, protocol, or external dependency decision remains unresolved.
- List assumptions and residual uncertainty explicitly.
- Stop after presenting the plan. Wait for explicit implementation authorization before changing any file.
