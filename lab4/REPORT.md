# Lab 4 — Group SnakeGame_Project

| | |
|---|---|
| Repository | DeepSoni04/SnakeGame_Project |
| Base tag | `lab4-base` at commit `2807e4c6bf25e6b723d3a7a8ca4d828a3587541c` |
| Pull request | https://github.com/DeepSoni04/SnakeGame_Project/pull/2 |

---

## 1. Five rules — [5]

Written before opening the source. Behaviour, with an observable outcome.

| # | Rule |
|---|---|
| 1 | Moving the snake head into any outer boundary wall causes game over (`gameOver = true`). |
| 2 | Moving the snake head into any segment of its own body/tail causes game over (`gameOver = true`). |
| 3 | Pressing directional control keys ('a'/'A', 'd'/'D', 'w'/'W', 's'/'S') updates the snake's heading direction accordingly. |
| 4 | Eating fruit increases the player's score by 10 points and increases the snake tail length by 1. |
| 5 | When the snake dies and the score exceeds the recorded high score, the game persists the new high score to `highscore.txt` and prompts the player to replay or quit. |

If you could not state one of your own game's rules without going to look, say which and why. It costs no marks.

Rule 4 point value: I originally assumed eating fruit awarded 1 point (following standard classic snake rules); checking the implementation revealed it awards 10 points per fruit.

---

## 2. What you could test, and what stopped you — [10]

No source changes in this part. Every `file:line` below is a line in `lab4-base`.

| # | Rule | Test written? | Blocking dependency (`file:line` + what it is) |
|---|---|---|---|
| 1 | Wall Collision | Yes | None (`logic()` contains pure coordinate bounds check; testable by setting out-of-bounds coordinates). |
| 2 | Self Collision | Yes | None (`logic()` checks `tailX`/`tailY` collision against head; testable by pre-populating tail array and moving into it). |
| 3 | Directional Input | No | `snake.cpp:130` — `_getch()` (and `_kbhit()` at `snake.cpp:129`) called directly inside `Input()`, blocking on hardware console keyboard buffer with no way to supply keys programmatically. |
| 4 | Fruit Consumption & Deterministic Placement | No | `snake.cpp:195-196` — `rand()` is called directly inside `logic()` without dependency injection, producing non-deterministic fruit coordinates and lacking validation against snake body cells. |
| 5 | Game Over & Replay Loop | No | `snake.cpp:215` & `snake.cpp:228` — `Sleep(speed)` at line 215 freezes execution inside the game loop, and `cin >> choice` at line 228 blocks indefinitely awaiting interactive terminal input. |

> **Rules testable without modifying the source: 2 / 5**

"It needs user input" is not a blocking dependency. `main.cpp:214 — getch() called inside the game loop` is.

In `lab4-base`, `snake.cpp` was written as a procedural script where game loop orchestration, console hardware polling, timing delays, and random number generation are hardwired directly into top-level functions without seams or dependency injection. While raw movement and collision assertions could be executed by manually configuring global variables and invoking `logic()`, any rule depending on keyboard input (`snake.cpp:130`), non-deterministic RNG (`snake.cpp:195-196`), or loop lifecycle (`snake.cpp:215`, `snake.cpp:228`) cannot be tested without modifying the source.

---

## 3. Coverage, and what it missed — [6]

| | |
|---|---|
| Line coverage | 18.05 % |
| Branch coverage | 20.86 % |
| Command used | `g++ --coverage -O0 -g -Dmain=game_main -c snake.cpp -o snake.o && g++ --coverage -O0 -g -c tests.cpp -o tests.o && g++ --coverage snake.o tests.o -o tests.exe && ./tests.exe && gcov -b snake.cpp` |

**One rule that is executed by the suite but not verified by it:**

| | |
|---|---|
| Rule | Snake body segment propagation (tail shifting to follow the head on each tick) |
| Line that runs | `snake.cpp:165-166` (`tailX[i] = prevX; tailY[i] = prevY;`) |
| The assertion that is missing | `assert(tailX[1] == initialHeadX && tailY[1] == initialHeadY);` |

---

## 4. The seam — [10]

| | |
|---|---|
| Rule made testable | Rule 3: Pressing directional control keys updates the snake's heading direction accordingly |
| Commit 1 (seam) | `bbba797` |
| Commit 2 (test) | `270d844` |
| Seam kind | object |
| Enabling point | `setInputHandler(InputHandler* handler)` / global `g_input` pointer in `snake.cpp` |
| What production code gave up | Nothing |

The last row is graded. If the honest answer is "nothing", write that and say why the
seam cost nothing here.

The seam cost nothing because production code retains `defaultInputHandler` which calls `_kbhit()` and `_getch()` directly on Windows terminals, preserving 100% identical gameplay, keyboard responsiveness, frame timings, and user experience. The only theoretical overhead is an imperceptible virtual table dispatch (a few nanoseconds per frame) which has zero effect on the game.

---

## 5. The double — [4]

| | |
|---|---|
| What you passed through the seam | stub |
| The method under test | `Input()` |

Two sentences: was the collaborator asked a question or told to do something, and why
does that decide the answer above?

The collaborator (`InputHandler`) was **asked a question** (`hasKey()` and `getKey()`), querying whether a keystroke is pending and which character was entered, rather than being told to perform an action. Because the double supplies canned query answers (indirect inputs to `Input()`) rather than receiving void command invocations whose occurrence must be verified, it is classified as a stub.

---

## 6. Two smells in your own tests — [5]

| | Smell | `file:line` | One-line fix |
|---|---|---|---|
| 1 | General fixture | `tests.cpp:37` | Encapsulate game state in a `GameState` struct passed as a parameter rather than resetting global variables between tests. |
| 2 | Assertion roulette | `tests.cpp:62` | Replace bare `assert()` statements with descriptive assertion failure messages or a test framework like Google Test (`EXPECT_TRUE`). |

Fixing them is optional. Finding them is not.

---
