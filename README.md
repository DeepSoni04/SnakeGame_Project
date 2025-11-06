
## 🐍 Emoji-Aligned Snake Game (C++ Terminal Edition)

A colorful, emoji-aligned *Snake Game* that runs directly in your *terminal, compatible with **Windows* and *Linux/macOS*.
Enjoy a modern twist on the classic Snake game — complete with *obstacles, **high scores, and **smooth emoji visuals*!

---

### ✨ Features

* 🧩 *Cross-platform*: Works on both Windows and Linux/macOS terminals
* 🕹 *Real-time input* (no need to press Enter)
* 🎨 *Emoji graphics* for smooth visual alignment
* 💾 *High score persistence* (saved in highscore.txt)
* 🚧 *Obstacles* that make each round unique
* ⚡ *Increasing speed* as you collect food
* 🔁 *Restart/Quit menu* after game over
* 💻 *No external libraries* required — just standard C++

---

### 🧱 Requirements

| Platform        | Requirements                                                   |
| --------------- | -------------------------------------------------------------- |
| *Windows*     | MinGW / Visual Studio / any compiler supporting C++17 or later |
| *Linux/macOS* | g++ with C++17+ support and a UTF-8 capable terminal         |

---

### ⚙ Build & Run Instructions

#### 🪟 On *Windows*:

1. Make sure you have a C++ compiler (like MinGW or Visual Studio Developer Command Prompt).
2. Open a terminal and compile:

   bash
   g++ snake.cpp -o snake
   
3. Run the game:

   bash
   snake
   

#### 🐧 On *Linux/macOS*:

1. Open a terminal and compile:

   bash
   g++ snake.cpp -o snake
   
2. Run the game:

   bash
   ./snake
   

---

### 🎮 Controls

| Key       | Action                  |
| --------- | ----------------------- |
| *W / ↑* | Move Up                 |
| *S / ↓* | Move Down               |
| *A / ←* | Move Left               |
| *D / →* | Move Right              |
| *R*     | Restart after Game Over |
| *Q*     | Quit the Game           |

---

### 🍏 Game Rules

* Eat the *red food (●)* to grow your snake and gain points.
* Avoid hitting:

  * The *walls*
  * Your *own body*
  * The *gray triangles (▲)* — obstacles
* Each time you eat food:

  * 🟢 Your snake grows longer
  * ⚡ The game speeds up slightly
* The *score* and *high score* are shown at the bottom of the screen.

---

### 📁 File Structure


snake.cpp          # Main source code
highscore.txt      # (Automatically created) stores your best score
README.md          # You are here


---

### 🧩 Technical Details

* Uses *ANSI escape codes* for colors and emoji rendering
* Enables *Virtual Terminal Processing* on Windows consoles for ANSI compatibility
* Uses non-blocking keyboard input (kbhit / termios + select)
* Cross-platform screen clearing and cursor hiding for smooth rendering
* Emoji alignment ensured with proper padding

---
### 💡 Customization

You can tweak a few options in the code:

| Variable         | Description                                           | Default |
| ---------------- | ----------------------------------------------------- | ------- |
| USE_EMOJI      | Use emoji-based visuals (set false for plain ASCII) | true  |
| Game g(15, 30) | Grid size (rows × columns)                            | 15×30 |
| speed          | Starting game speed in milliseconds                   | 150   |
