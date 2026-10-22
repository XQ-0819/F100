# Feature Planning Execution Contract

## Evidence Priority

Resolve conflicts using the repository's own priority rules. In their absence, prefer:

1. The user's latest explicit conclusion.
2. Current branch code, project configuration, and reproducible behavior.
3. Current approved requirement documents.
4. Project base and design documents.
5. Team templates, historical commits, and old conversation context.

Report conflicts rather than silently choosing an implementation that changes business behavior.

## Investigation by Change Type

### GUI

Inspect the parent window, page template, initialization, refresh, input and event callbacks, cache, save, cancel, navigation, repeated entry, destruction, timers, animations, language resources, fonts, and a mature reference page. Confirm coordinate and style calculations are reused when appearance is in scope.

### Configuration and Persistence

Inspect the type definition, alignment, unit, default, valid range, page cache, formal configuration, comparison and copy paths, save trigger, storage key or address, read behavior, migration, restart behavior, and every protocol serialization user. Distinguish an absent record, short record, invalid value, and write failure.

### Control and Concurrency

Inspect input acquisition, calculated state, final control state, physical output, all activation and shutdown entries, automatic and manual priority, empty or disabled modes, calibration, alarms, communication loss, task ownership, interrupt access, timers, locks, critical sections, and fail-safe behavior.

### Protocol

Inspect request, response, active upload, message and device identifiers, field order, width, byte order, length, buffers, sequence ownership, ACK, timeout, retry, split packets, concurrency, platform behavior, external firmware, and rollout compatibility. Calculate worst-case packet size before proposing capacity changes.

### Build, Resources, and Cross-Target Changes

Inspect all target variants, project entries, compiler and packing options, generated inputs, source encoding, language resources, fonts, binary provenance, simulator copies, and cleanup or ignore rules.

## Existing-Capability Decision

For every proposed mechanism, record one decision:

| Decision | Meaning |
|---|---|
| Reuse directly | Existing semantics and lifecycle already satisfy the requirement |
| Extend minimally | Existing owner is correct, but a small additional parameter or branch is necessary |
| Add locally | No suitable API exists and the behavior belongs to one module or page |
| Add shared mechanism | Multiple proven consumers need the same stable semantics; identify ownership and regression scope |
| Do not change | Existing behavior is outside the confirmed requirement |

Do not select a shared abstraction merely because two code blocks look similar. Explain semantic ownership and expected consumers.

## Material Clarification Questions

Ask before finalizing the plan when the answer changes:

- Persistent structure, upgrade, defaulting, or rollback behavior.
- Wire protocol, field semantics, buffer sizing, or coordinated versions.
- Physical output safety, automatic and manual priority, or failure state.
- Authentication, authorization, local versus remote control, or audit behavior.
- Target products, hardware variants, or external system ownership.
- Acceptance environment or whether a required dependency is available.

Resolve minor naming, local layout, and implementation details from established repository patterns when the requirement permits it.

## Plan Quality Gate

A plan is ready for confirmation only when it provides:

- A requirement statement with explicit non-goals.
- A complete enough current call and data flow to support the design.
- Evidence that existing APIs and similar implementations were searched.
- Exact necessary and unchanged scope.
- State ownership, persistence, protocol, lifecycle, and compatibility conclusions where relevant.
- Failure and safe-state behavior.
- A layered validation matrix and explicit unavailable environments.
- A small implementation order and submission boundary.

If repository evidence contradicts the requested approach, show the evidence and propose the smallest compatible alternative rather than silently implementing the request literally.
