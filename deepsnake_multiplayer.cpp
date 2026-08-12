#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cctype>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

bool _kbhit() {
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;

    tcsetattr(0, TCSANOW, &term);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term2);

    return byteswaiting > 0;
}

char _getch() {
    char buf = 0;

    termios old = {0};

    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");

    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;

    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");

    if (read(0, &buf, 1) < 0)
        perror("read()");

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");

    return buf;
}

void Sleep(int ms) {
    usleep(ms * 1000);
}
#endif

using namespace std;

// Directions
#define STOP  0
#define LEFT  1
#define RIGHT 2
#define UP    3
#define DOWN  4

// ======================================================
// GAME SETTINGS
// ======================================================

const int width = 30;
const int height = 25;

const int MAX_TAIL = 200;
const int MAX_OBS = 12;

// ======================================================
// PLAYER 1
// ======================================================

int p1X, p1Y;
int p1TailX[MAX_TAIL];
int p1TailY[MAX_TAIL];
int p1Tail = 0;
int p1Dir = RIGHT;
int p1Score = 0;

// ======================================================
// PLAYER 2
// ======================================================

int p2X, p2Y;
int p2TailX[MAX_TAIL];
int p2TailY[MAX_TAIL];
int p2Tail = 0;
int p2Dir = LEFT;
int p2Score = 0;

// ======================================================
// FRUIT
// ======================================================

int fruitX, fruitY;

// ======================================================
// OBSTACLES
// ======================================================

int obsX[MAX_OBS];
int obsY[MAX_OBS];
int nObs = 12;

// ======================================================
// GAME
// ======================================================

bool gameOver = false;

int highScore1 = 0;
int highScore2 = 0;

// ======================================================
// HIGH SCORE
// ======================================================

void loadHighScore() {

    ifstream file("highscore.txt");

    if (file.is_open()) {

        file >> highScore1;
        file >> highScore2;

        file.close();

    } else {

        highScore1 = 0;
        highScore2 = 0;
    }
}

void saveHighScore() {

    ofstream file("highscore.txt");

    if (file.is_open()) {

        file << highScore1 << endl;
        file << highScore2 << endl;

        file.close();
    }
}

// ======================================================
// CHECK WHETHER POSITION IS OCCUPIED
// ======================================================

bool positionOccupied(int px, int py) {

    // Player 1 head
    if (px == p1X && py == p1Y)
        return true;

    // Player 2 head
    if (px == p2X && py == p2Y)
        return true;

    // Player 1 body
    for (int i = 0; i < p1Tail; i++) {

        if (px == p1TailX[i] &&
            py == p1TailY[i])
            return true;
    }

    // Player 2 body
    for (int i = 0; i < p2Tail; i++) {

        if (px == p2TailX[i] &&
            py == p2TailY[i])
            return true;
    }

    // Obstacles
    for (int i = 0; i < nObs; i++) {

        if (px == obsX[i] &&
            py == obsY[i])
            return true;
    }

    return false;
}

// ======================================================
// GENERATE FRUIT
// ======================================================

void generateFruit() {

    do {

        fruitX = rand() % width;
        fruitY = rand() % height;

    } while (positionOccupied(fruitX, fruitY));
}

// ======================================================
// SETUP
// ======================================================

void setup() {

    gameOver = false;

    // --------------------------
    // PLAYER 1
    // --------------------------

    p1X = width / 4;
    p1Y = height / 2;

    p1Tail = 0;
    p1Dir = RIGHT;
    p1Score = 0;

    // --------------------------
    // PLAYER 2
    // --------------------------

    p2X = (width * 3) / 4;
    p2Y = height / 2;

    p2Tail = 0;
    p2Dir = LEFT;
    p2Score = 0;

    // --------------------------
    // OBSTACLES
    // --------------------------

    for (int i = 0; i < nObs; i++) {

        bool valid = false;

        while (!valid) {

            obsX[i] = rand() % width;
            obsY[i] = rand() % height;

            valid = true;

            // Avoid Player 1
            if (obsX[i] == p1X &&
                obsY[i] == p1Y)
                valid = false;

            // Avoid Player 2
            if (obsX[i] == p2X &&
                obsY[i] == p2Y)
                valid = false;

            // Avoid other obstacles
            for (int j = 0; j < i; j++) {

                if (obsX[i] == obsX[j] &&
                    obsY[i] == obsY[j])
                    valid = false;
            }
        }
    }

    generateFruit();
}

// ======================================================
// CLEAR SCREEN
// ======================================================

void clearScreen() {

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ======================================================
// DRAW GAME
// ======================================================

void Draw() {

    clearScreen();

    // Top wall
    for (int i = 0; i < width + 2; i++)
        cout << "##";

    cout << endl;

    // Game area
    for (int i = 0; i < height; i++) {

        cout << "##";

        for (int j = 0; j < width; j++) {

            bool printed = false;

            // --------------------------------
            // PLAYER 1 HEAD
            // --------------------------------

            if (j == p1X && i == p1Y) {

                cout << "🟢";
                printed = true;
            }

            // --------------------------------
            // PLAYER 2 HEAD
            // --------------------------------

            else if (j == p2X && i == p2Y) {

                cout << "🔵";
                printed = true;
            }

            // --------------------------------
            // FRUIT
            // --------------------------------

            else if (j == fruitX &&
                     i == fruitY) {

                cout << "🍎";
                printed = true;
            }

            // --------------------------------
            // OBSTACLES
            // --------------------------------

            if (!printed) {

                for (int o = 0; o < nObs; o++) {

                    if (obsX[o] == j &&
                        obsY[o] == i) {

                        cout << "🧱";
                        printed = true;
                        break;
                    }
                }
            }

            // --------------------------------
            // PLAYER 1 BODY
            // --------------------------------

            if (!printed) {

                for (int k = 0; k < p1Tail; k++) {

                    if (p1TailX[k] == j &&
                        p1TailY[k] == i) {

                        cout << "🟩";
                        printed = true;
                        break;
                    }
                }
            }

            // --------------------------------
            // PLAYER 2 BODY
            // --------------------------------

            if (!printed) {

                for (int k = 0; k < p2Tail; k++) {

                    if (p2TailX[k] == j &&
                        p2TailY[k] == i) {

                        cout << "🟦";
                        printed = true;
                        break;
                    }
                }
            }

            // --------------------------------
            // EMPTY
            // --------------------------------

            if (!printed)
                cout << "  ";
        }

        cout << "##" << endl;
    }

    // Bottom wall
    for (int i = 0; i < width + 2; i++)
        cout << "##";

    cout << endl;

    // Scores
    cout << "\n";
    cout << "🟢 Player 1 Score: " << p1Score;
    cout << "     🔵 Player 2 Score: " << p2Score << endl;

    cout << "🏆 P1 High Score: " << highScore1;
    cout << "     🏆 P2 High Score: " << highScore2 << endl;

    cout << "\n";
    cout << "PLAYER 1: W A S D";
    cout << "     PLAYER 2: Arrow Keys" << endl;

    cout << "Press X to Exit" << endl;
}

// ======================================================
// INPUT
// ======================================================

void Input() {

    if (!_kbhit())
        return;

#ifdef _WIN32

    int key = _getch();

    // --------------------------
    // PLAYER 1
    // --------------------------

    if (key == 'w' || key == 'W') {

        if (p1Dir != DOWN)
            p1Dir = UP;
    }

    else if (key == 's' || key == 'S') {

        if (p1Dir != UP)
            p1Dir = DOWN;
    }

    else if (key == 'a' || key == 'A') {

        if (p1Dir != RIGHT)
            p1Dir = LEFT;
    }

    else if (key == 'd' || key == 'D') {

        if (p1Dir != LEFT)
            p1Dir = RIGHT;
    }

    // --------------------------
    // EXIT
    // --------------------------

    else if (key == 'x' || key == 'X') {

        gameOver = true;
    }

    // --------------------------
    // PLAYER 2 ARROW KEYS
    // --------------------------

    else if (key == 0 || key == 224) {

        int arrow = _getch();

        switch (arrow) {

        case 72: // UP

            if (p2Dir != DOWN)
                p2Dir = UP;

            break;

        case 80: // DOWN

            if (p2Dir != UP)
                p2Dir = DOWN;

            break;

        case 75: // LEFT

            if (p2Dir != RIGHT)
                p2Dir = LEFT;

            break;

        case 77: // RIGHT

            if (p2Dir != LEFT)
                p2Dir = RIGHT;

            break;
        }
    }

#else

    // Linux / macOS terminals

    char key = _getch();

    // --------------------------
    // PLAYER 1
    // --------------------------

    if (key == 'w' || key == 'W') {

        if (p1Dir != DOWN)
            p1Dir = UP;
    }

    else if (key == 's' || key == 'S') {

        if (p1Dir != UP)
            p1Dir = DOWN;
    }

    else if (key == 'a' || key == 'A') {

        if (p1Dir != RIGHT)
            p1Dir = LEFT;
    }

    else if (key == 'd' || key == 'D') {

        if (p1Dir != LEFT)
            p1Dir = RIGHT;
    }

    else if (key == 'x' || key == 'X') {

        gameOver = true;
    }

    // --------------------------
    // ARROW KEYS
    // --------------------------

    else if (key == '\033') {

        char c1 = _getch();
        char c2 = _getch();

        if (c1 == '[') {

            switch (c2) {

            case 'A': // UP

                if (p2Dir != DOWN)
                    p2Dir = UP;

                break;

            case 'B': // DOWN

                if (p2Dir != UP)
                    p2Dir = DOWN;

                break;

            case 'C': // RIGHT

                if (p2Dir != LEFT)
                    p2Dir = RIGHT;

                break;

            case 'D': // LEFT

                if (p2Dir != RIGHT)
                    p2Dir = LEFT;

                break;
            }
        }
    }

#endif
}

// ======================================================
// MOVE PLAYER 1
// ======================================================

void movePlayer1() {

    // Move body
    if (p1Tail > 0) {

        for (int i = p1Tail - 1; i > 0; i--) {

            p1TailX[i] = p1TailX[i - 1];
            p1TailY[i] = p1TailY[i - 1];
        }

        p1TailX[0] = p1X;
        p1TailY[0] = p1Y;
    }

    // Move head
    switch (p1Dir) {

    case LEFT:
        p1X--;
        break;

    case RIGHT:
        p1X++;
        break;

    case UP:
        p1Y--;
        break;

    case DOWN:
        p1Y++;
        break;
    }
}

// ======================================================
// MOVE PLAYER 2
// ======================================================

void movePlayer2() {

    // Move body
    if (p2Tail > 0) {

        for (int i = p2Tail - 1; i > 0; i--) {

            p2TailX[i] = p2TailX[i - 1];
            p2TailY[i] = p2TailY[i - 1];
        }

        p2TailX[0] = p2X;
        p2TailY[0] = p2Y;
    }

    // Move head
    switch (p2Dir) {

    case LEFT:
        p2X--;
        break;

    case RIGHT:
        p2X++;
        break;

    case UP:
        p2Y--;
        break;

    case DOWN:
        p2Y++;
        break;
    }
}

// ======================================================
// CHECK PLAYER 1 COLLISION
// ======================================================

void checkPlayer1Collision() {

    // Wall
    if (p1X < 0 ||
        p1X >= width ||
        p1Y < 0 ||
        p1Y >= height) {

        gameOver = true;
    }

    // Obstacles
    for (int i = 0; i < nObs; i++) {

        if (p1X == obsX[i] &&
            p1Y == obsY[i]) {

            gameOver = true;
        }
    }

    // Own body
    for (int i = 0; i < p1Tail; i++) {

        if (p1X == p1TailX[i] &&
            p1Y == p1TailY[i]) {

            gameOver = true;
        }
    }

    // Player 2 body
    for (int i = 0; i < p2Tail; i++) {

        if (p1X == p2TailX[i] &&
            p1Y == p2TailY[i]) {

            gameOver = true;
        }
    }

    // Player 2 head
    if (p1X == p2X &&
        p1Y == p2Y) {

        gameOver = true;
    }
}

// ======================================================
// CHECK PLAYER 2 COLLISION
// ======================================================

void checkPlayer2Collision() {

    // Wall
    if (p2X < 0 ||
        p2X >= width ||
        p2Y < 0 ||
        p2Y >= height) {

        gameOver = true;
    }

    // Obstacles
    for (int i = 0; i < nObs; i++) {

        if (p2X == obsX[i] &&
            p2Y == obsY[i]) {

            gameOver = true;
        }
    }

    // Own body
    for (int i = 0; i < p2Tail; i++) {

        if (p2X == p2TailX[i] &&
            p2Y == p2TailY[i]) {

            gameOver = true;
        }
    }

    // Player 1 body
    for (int i = 0; i < p1Tail; i++) {

        if (p2X == p1TailX[i] &&
            p2Y == p1TailY[i]) {

            gameOver = true;
        }
    }

    // Player 1 head
    if (p2X == p1X &&
        p2Y == p1Y) {

        gameOver = true;
    }
}

// ======================================================
// CHECK HEAD-TO-HEAD COLLISION
// ======================================================

void checkHeadCollision() {

    if (p1X == p2X &&
        p1Y == p2Y) {

        gameOver = true;
    }
}

// ======================================================
// EAT FRUIT
// ======================================================

void eatFruit() {

    // Player 1 eats fruit
    if (p1X == fruitX &&
        p1Y == fruitY) {

        p1Score += 10;

        if (p1Tail < MAX_TAIL)
            p1Tail++;

        generateFruit();
    }

    // Player 2 eats fruit
    else if (p2X == fruitX &&
             p2Y == fruitY) {

        p2Score += 10;

        if (p2Tail < MAX_TAIL)
            p2Tail++;

        generateFruit();
    }
}

// ======================================================
// GAME LOGIC
// ======================================================

void logic() {

    // Move both players
    movePlayer1();
    movePlayer2();

    // Check collisions
    checkPlayer1Collision();
    checkPlayer2Collision();
    checkHeadCollision();

    // Fruit
    if (!gameOver)
        eatFruit();
}

// ======================================================
// MAIN
// ======================================================

int main() {

    srand(static_cast<unsigned int>(time(0)));

    loadHighScore();

    char choice;

    do {

        setup();

        while (!gameOver) {

            Draw();

            Input();

            if (gameOver)
                break;

            logic();

            // Speed increases as players score
            int highestScore = max(p1Score, p2Score);

            int speed = max(60, 150 - (highestScore / 5));

            Sleep(speed);
        }

        // --------------------------
        // GAME OVER SCREEN
        // --------------------------

        clearScreen();

        cout << "\n";
        cout << "========================================\n";
        cout << "             💀 GAME OVER 💀\n";
        cout << "========================================\n";

        cout << "\n🟢 Player 1 Final Score: "
             << p1Score << endl;

        cout << "🔵 Player 2 Final Score: "
             << p2Score << endl;

        // --------------------------
        // WINNER
        // --------------------------

        if (p1Score > p2Score) {

            cout << "\n🏆 PLAYER 1 WINS!\n";
        }

        else if (p2Score > p1Score) {

            cout << "\n🏆 PLAYER 2 WINS!\n";
        }

        else {

            cout << "\n🤝 IT'S A DRAW!\n";
        }

        // --------------------------
        // HIGH SCORES
        // --------------------------

        if (p1Score > highScore1) {

            highScore1 = p1Score;

            cout << "\n🏆 New Player 1 High Score!\n";
        }

        if (p2Score > highScore2) {

            highScore2 = p2Score;

            cout << "🏆 New Player 2 High Score!\n";
        }

        saveHighScore();

        cout << "\n----------------------------------------\n";

        cout << "Player 1 High Score: "
             << highScore1 << endl;

        cout << "Player 2 High Score: "
             << highScore2 << endl;

        cout << "----------------------------------------\n";

        cout << "\nPress (R) to Replay or (Q) to Quit: ";

        cin >> choice;

        choice = tolower(choice);

    } while (choice == 'r');

    cout << "\n🐍 Thanks for playing Multiplayer Snake!\n";

    return 0;
}