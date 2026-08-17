# Code Smell Audit Report: main branch

**Target File**: `deepsnake.cpp`

## Summary of Findings

| Smell | Category | Severity | Confidence | Location | Description |
| --- | --- | --- | --- | --- | --- |
| Primitive Obsession | Bloaters | HIGH | C4 — Certain | `deepsnake.cpp:48` | Global primitive variables (`x`, `y`, `fruitX`, `fruitY`, `score`, `dir`, `tailX`, `tailY`, `obsX`, `obsY`) used instead of structured objects (`Point`, `Snake`, `Obstacle`, `Board`). |
| Long Method | Bloaters | HIGH | C4 — Certain | `deepsnake.cpp:109` | `Draw()` spans 51 lines performing screen clearing, wall rendering, grid item searching, and score display. |
| Long Method | Bloaters | HIGH | C4 — Certain | `deepsnake.cpp:178` | `logic()` spans 45 lines handling movement, body segment shifting, wall collision, obstacle collision, self collision, and food eating. |
| Data Clumps | Bloaters | MEDIUM | C3 — High | `deepsnake.cpp:52` | `tailX[100]` and `tailY[100]` are always passed and operated on together, indicating a missing coordinate/segment abstraction. |
| Duplicate Code | Dispensables | MEDIUM | C3 — High | `deepsnake.cpp:113` | Top wall rendering loop (`deepsnake.cpp:113-115`) and bottom wall rendering loop (`deepsnake.cpp:153-155`) are identical duplicate blocks. |
| Switch Statements | OO Abusers | MEDIUM | C3 — High | `deepsnake.cpp:163` | Switch statement in `Input()` on key characters to mutate raw integer direction states. |
| Switch Statements | OO Abusers | MEDIUM | C3 — High | `deepsnake.cpp:194` | Switch statement in `logic()` on direction integer codes to update snake head coordinates. |
| Global / Inappropriate Intimacy | Couplers | HIGH | C4 — Certain | `deepsnake.cpp:48` | All primary functions (`setup()`, `Draw()`, `Input()`, `logic()`, `loadHighScore()`, `saveHighScore()`) tightly couple to global mutable state. |
| Dead Code | Dispensables | LOW | C2 — Medium | `deepsnake.cpp:42` | `#define STOP 0` direction macro initialized in `setup()` but cannot be re-triggered by user controls during gameplay. |
