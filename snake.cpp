#include <iostream>
#include <conio.h>      // for _kbhit() and _getch() on Windows
#include <windows.h>    // for Sleep()
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

#define STOP 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int dir;

// Obstacles
int obsX[20], obsY[20];
int nObs = 5;

// High score file
int highscore = 0;

void loadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highscore;
        file.close();
    } else {
        highscore = 0;
    }
}

void saveHighScore() {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highscore;
        file.close();
    }
}

void setup() {
    srand(time(0));
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;

    // Generate random obstacles
    for (int i = 0; i < nObs; i++) {
        obsX[i] = rand() % width;
        obsY[i] = rand() % height;

        // Avoid spawning obstacles on player or fruit
        if ((obsX[i] == x && obsY[i] == y) || (obsX[i] == fruitX && obsY[i] == fruitY)) {
            i--;
        }
    }
}

void Draw() {
    system("cls");

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";

            if (i == y && j == x)
                cout << "O"; // Snake head
            else if (i == fruitY && j == fruitX)
                cout << "*"; // Fruit
            else {
                bool printed = false;

                // Draw obstacles
                for (int o = 0; o < nObs; o++) {
                    if (obsX[o] == j && obsY[o] == i) {
                        cout << "X";
                        printed = true;
                        break;
                    }
                }

                // Draw tail
                if (!printed) {
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o";
                            printed = true;
                            break;
                        }
                    }
                }

                if (!printed)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << "   High Score: " << highscore << endl;
    cout << "Controls: W/A/S/D  |  X = Exit" << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
        case 'A':
            dir = LEFT;
            break;
        case 'd':
        case 'D':
            dir = RIGHT;
            break;
        case 'w':
        case 'W':
            dir = UP;
            break;
        case 's':
        case 'S':
            dir = DOWN;
            break;
        case 'x':
        case 'X':
            gameOver = true;
            break;
        }
    }
}

void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:  x--; break;
    case RIGHT: x++; break;
    case UP:    y--; break;
    case DOWN:  y++; break;
    }

    // Wall collision
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    // Obstacle collision
    for (int o = 0; o < nObs; o++)
        if (x == obsX[o] && y == obsY[o])
            gameOver = true;

    // Tail collision
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    // Eat fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    loadHighScore();
    char choice;

    do {
        setup();

        while (!gameOver) {
            Draw();
            Input();
            logic();

            // Dynamic speed increases with score
            int speed = max(50, 150 - (score / 5));
            Sleep(speed);
        }

        system("cls");
        cout << "\nGame Over! Final Score = " << score << endl;

        if (score > highscore) {
            highscore = score;
            saveHighScore();
            cout << "New High Score!" << endl;
        }

        cout << "\nPress (R) to Replay or (Q) to Quit: ";
        cin >> choice;
        choice = tolower(choice);

    } while (choice == 'r');

    cout << "\nThanks for playing Snake Game!\n";
    return 0;
}
