# Ubiquitous Language

## Game Domain Entities

| Term | Definition | Aliases to avoid | In code |
| --- | --- | --- | --- |
| **Snake** | The player-controlled entity moving continuously on the board | Worm, hero | `x`, `y`, `tailX`, `tailY` — `deepsnake.cpp:51` |
| **Snake Head** | The front segment of the snake controlling movement direction | Leader, front | `x`, `y` — `deepsnake.cpp:51` |
| **Snake Body** | The collection of body segments following the head that grow when eating fruit | Tail, length | `tailX`, `tailY`, `nTail` — `deepsnake.cpp:52` ⚠ |
| **Fruit** | A collectible item on the grid that increases score and snake length | Food, apple, bonus | `fruitX`, `fruitY` — `deepsnake.cpp:51` |
| **Obstacle** | A fixed barrier on the grid causing collision if touched | Block, wall, rock | `obsX`, `obsY`, `nObs` — `deepsnake.cpp:57` ⚠ |
| **Board** | The 2D grid playing surface enclosed by outer walls | Grid, map, arena | `width`, `height` — `deepsnake.cpp:49` |
| **Score** | The points accumulated by the player during a game session | Points, total | `score` — `deepsnake.cpp:51` |
| **High Score** | The highest score achieved across all game sessions, saved to disk | Best score, record | `highscore` — `deepsnake.cpp:61` |
| **Direction** | The current movement heading of the snake (UP, DOWN, LEFT, RIGHT, STOP) | Heading, orientation | `dir` — `deepsnake.cpp:54` ⚠ |
| **Collision** | An event where the snake head hits a wall, obstacle, or body segment | Crash, hit, death | `gameOver` check — `deepsnake.cpp:202` |

## Relationships

- A **Board** contains one or more **Snakes**, one **Fruit**, and zero or more **Obstacles**
- A **Snake** consists of one **Snake Head** and zero or more **Snake Body** segments
- Eating a **Fruit** increases the **Score** and appends a segment to the **Snake Body**
- A **Collision** immediately ends the game session and checks for a new **High Score**

## Example dialogue

> **Dev:** "When a **Snake** moves into a cell containing a **Fruit**, do we update the **Score** before generating the next **Fruit**?"
> **Domain expert:** "Yes — the **Score** increases immediately, the **Snake Body** grows by one segment, and a new **Fruit** spawns at an unoccupied location on the **Board**."
> **Dev:** "What happens if the **Snake Head** enters a cell occupied by an **Obstacle** or a **Snake Body** segment?"
> **Domain expert:** "That causes a **Collision**, which triggers Game Over and checks if the current **Score** exceeds the stored **High Score**."

## Flagged ambiguities

- **"Tail" vs. "Snake Body"**: The codebase uses `tailX` and `tailY` to store all non-head body segments. In domain terminology, "Tail" specifically means the single trailing tip of the snake, while "Snake Body" describes the entire body sequence.
- **"Fruit" vs. "Food"**: The code comments use "Fruit" (`fruitX`, `fruitY`, `deepsnake.cpp:51`), whereas `README.md:76` calls it "red food". Canonical term is **Fruit**.
- **"Direction" representation**: `#define` macros `LEFT`, `RIGHT`, `UP`, `DOWN`, `STOP` (`deepsnake.cpp:42-46`) use raw integers instead of a strongly typed enumeration.

## Code drift

| Canonical term | Called in code | Location | Note |
| --- | --- | --- | --- |
| **Snake Body** | `tailX`, `tailY`, `nTail` | `deepsnake.cpp:52-53` | Code uses "tail" for the entire body structure |
| **Obstacle** | `obsX`, `obsY`, `nObs` | `deepsnake.cpp:57-58` | Abbreviated name used for obstacles array |
| **Direction** | `dir` | `deepsnake.cpp:54` | Abbreviated name and raw integer primitive |
