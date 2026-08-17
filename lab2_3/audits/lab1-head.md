# Code Smell Audit Report: lab1-head (features branch)

**Target File**: `deepsnake_multiplayer.cpp`

## Summary of Findings

| Smell | Category | Severity | Confidence | Location | Description |
| --- | --- | --- | --- | --- | --- |
| Duplicate Code | Dispensables | HIGH | C4 — Certain | `deepsnake_multiplayer.cpp:78` | Player 1 state (`p1X`, `p1Y`, `p1TailX`, `p1TailY`, `p1Tail`, `p1Dir`, `p1Score`) and Player 2 state (`p2X`, `p2Y`, `p2TailX`, `p2TailY`, `p2Tail`, `p2Dir`, `p2Score`) duplicated as raw primitives. |
| Primitive Obsession | Bloaters | HIGH | C4 — Certain | `deepsnake_multiplayer.cpp:78` | 14 raw global arrays and primitive variables used to represent two players without a `Snake` encapsulation. |
| Long Method | Bloaters | HIGH | C4 — Certain | `deepsnake_multiplayer.cpp:273` | `Draw()` spans over 120 lines handling grid rendering, player 1 body checks, player 2 body checks, obstacle checks, and dual status display. |
| Long Method | Bloaters | HIGH | C4 — Certain | `deepsnake_multiplayer.cpp:420` | `logic()` spans over 150 lines handling movement, body shifting, wall collision, obstacle collision, and cross-player collisions for both snakes in a single monolithic method. |
| Data Clumps | Bloaters | MEDIUM | C3 — High | `deepsnake_multiplayer.cpp:79` | `p1TailX`/`p1TailY` and `p2TailX`/`p2TailY` coordinate pairs repeatedly passed together across collision check methods. |
| Divergent Change | Change Preventers | HIGH | C4 — Certain | `deepsnake_multiplayer.cpp:78` | Adding a 3rd player or modifying snake behavior requires modifying global definitions, `setup()`, `Input()`, `Draw()`, `logic()`, and collision functions simultaneously. |
| Shotgun Surgery | Change Preventers | HIGH | C4 — Certain | `deepsnake_multiplayer.cpp:167` | Changing snake movement or collision rules requires updating multiple independent loops across `positionOccupied()`, `setup()`, and `logic()`. |
| Switch Statements | OO Abusers | MEDIUM | C3 — High | `deepsnake_multiplayer.cpp:390` | Parallel switch statements in `Input()` processing WASD keys for Player 1 and IJKL keys for Player 2. |
| Global / Inappropriate Intimacy | Couplers | HIGH | C4 — Certain | `deepsnake_multiplayer.cpp:78` | Global mutable state quadrupled, making isolation testing and modular maintenance impossible. |
