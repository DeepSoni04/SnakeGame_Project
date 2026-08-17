# Lab 2_3 — Group SnakeGame_Project

---

## 1. Tool and install route — [3]

| | |
|---|---|
| Agent used for run 2 | Gemini 3.6 Flash (High) / Antigravity |
| `ubiquitous-language` install route | pasted SKILL.md (it643-content/skills/ubiquitous-language) |
| `refactoring/` pack install route | pasted SKILL.md (it643-content/skills/refactoring) |

Installed skills directly from vendor repository content in `it643-content/skills` during assistant execution session.

---

## 2. What I changed in the glossary — [4]

The generated file is at `lab2_3/UBIQUITOUS_LANGUAGE.md`. Corrected 'tail' in `tailX`/`tailY` to the canonical domain term **Snake Body**, clarifying that 'Tail' in domain language refers only to the trailing end segment. Standardized **Fruit** over the ambiguous 'red food' term found in README.md. Clarified the domain concept of **Direction** over raw integer macros, and added code drift entries mapping primitive field names (`dir`, `obsX`) to domain concepts.

---

## 3. Smell delta — [6]

Reports: `lab2_3/audits/main.md` (the code as you received it) and `lab2_3/audits/lab1-head.md` (after your Lab-1 PR).

| | count | representative site (`file:line`) |
|---|---|---|
| Smells my Lab-1 PR **introduced** | 3 | `deepsnake_multiplayer.cpp:78` |
| Smells my Lab-1 PR **left untouched** | 5 | `deepsnake.cpp:48` |
| Smells my Lab-1 PR **removed** | 0 | — |

---

## 4. Rejected candidates — [6]

At least three things the agent reported that are *not* real findings on this codebase.

| smell reported | `file:line` | why it does not hold |
|---|---|---|
| Lazy Class | `deepsnake.cpp:48` | `Position` struct groups x and y coordinate data clumps into a domain value object rather than being a redundant class. |
| Speculative Generality | `deepsnake.cpp:13` | Cross-platform terminal I/O helpers (`_kbhit()`, `_getch()`, `Sleep()`) handle OS portability and are actively used runtime dependencies. |
| Data Class | `deepsnake.cpp:63` | `HighScoreManager` encapsulates disk persistence logic (`load()`, `save()`, `update()`) for high scores adhering to SRP. |

---

## 5. Commit map — [7]

| # | sha | subject | what it is |
|---|---|---|---|
| 1 | a89705a | Add ubiquitous language domain glossary | glossary |
| 2 | 23568ac | Add code smell audit reports for main and lab1-head | smell report |
| 3 | cbcab36 | Refactor Snake game architecture into modular classes | the refactor alone |
| 4 | 044b30d | Add multiplayer mode to Snake game | the feature alone |

---

## 6. Two-run measurement — [4]

Run 1 is your Lab-1 branch — the numbers you already reported. Run 2 is commit 4 alone.

| | Run 1 (Lab 1) | Run 2 (commit 4) |
|---|---|---|
| Smells introduced | 3 | 0 |
| Lines changed, `git diff --shortstat -w` | 1 file changed, 978 insertions(+) | 1 file changed, 108 insertions(+), 65 deletions(-) |
| Lines changed, **raw** (no `-w`) | 1 file changed, 978 insertions(+) | 1 file changed, 109 insertions(+), 66 deletions(-) |
| Functions reached | 6 | 4 |
| Prompts to working code | 2 | 1 |
| Wall-clock time | 25 mins | 10 mins |

Commit 3 (the refactor) on its own: 1 file changed, 229 insertions(+), 137 deletions(-) `-w`, 1 file changed, 346 insertions(+), 254 deletions(-) raw.

---

## 7. Analysis Q1–Q2 — [5]

**Q1. Which smell did commit 3 actually fix?**
Commit 3 fixed Primitive Obsession (`deepsnake.cpp:48`) and Inappropriate Intimacy/Global Coupling across functions. Before the refactor, snake state, coordinates, fruit, obstacles, and scores were stored in 14 raw global primitives. Adding any field or entity required modifying variable declarations, `setup()`, `Draw()`, `Input()`, `logic()`, and file persistence in parallel across 5 distinct places. Post-refactor, `Position`, `Snake`, `HighScoreManager`, and `Game` encapsulate their respective states. Adding or updating snake behavior now costs editing a single encapsulated `Snake` class method instead of cascading global edits across the entire program.

**Q2. Compare commit 4 to your Lab-1 diff.**
In Lab 1, adding multiplayer without prior restructuring resulted in duplicating 14 raw global variables (`p1X`, `p2X`, `p1TailX`, `p2TailX`, etc.) and writing 978 raw lines of code in a monolithic `deepsnake_multiplayer.cpp`. In commit 4, building on the refactored OOP architecture required only 109 raw insertions (108 insertions `-w`) because adding Player 2 simply meant instantiating a second `Snake` object (`player2`) inside `Game`. The cost dropped dramatically from 978 line edits to 173 total line changes, while eliminating code duplication, shotgun surgery, and new code smells entirely.

---

## 8. Analysis Q3–Q4 — [5]

**Q3. Go back through your Lab-1 `LLM-LOG.md`. Did the assistant ever suggest restructuring before adding the feature?**
In Lab 1, the LLM immediately generated a brand new monolithic multiplayer script (`deepsnake_multiplayer.cpp`) by copy-pasting and duplicating existing global variables without suggesting architectural refactoring first. The LLM was prompted purely for functional feature addition ("make it multiplayer"), so it optimized for immediate output speed rather than design health. To trigger pre-feature restructuring, the prompt would need explicit design directives: "First analyze code smells and refactor global state into modular classes before implementing the multiplayer feature."

**Q4. How do you know commit 3 did not change behaviour?**
To be completely honest, we cannot mathematically prove behavior preservation because the codebase lacks automated unit or integration tests. We verified commit 3 manually by compiling and running single-player gameplay, checking movement, collision detection, fruit spawning, score incrementing, and high score persistence to verify visual equivalence. To guarantee behavior preservation during refactoring, we would need a comprehensive automated test suite (e.g., unit tests for `Snake::move()` and `Game::updateLogic()`) along with regression testing before making structural edits.

---

Sections 7 and 8 together: **380 words.**
