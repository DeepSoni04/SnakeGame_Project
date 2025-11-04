#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

bool gameOver;
const int width = 20;
const int height = 17;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coord;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw() {
    coord.X = 0; coord.Y = 0;
    SetConsoleCursorPosition(hConsole, coord);

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";
            if (i == y && j == x) cout << "O";        // snake head
            else if (i == fruitY && j == fruitX) cout << "F"; // fruit
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        print = true;
                        break;
                    }
                }
                if (!print) cout << " ";
            }
            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
    cout << "Controls: W = Up | S = Down | A = Left | D = Right | X = Exit" << endl;
}

void Input() {
    if (_kbhit()) {
        char c = _getch();
        switch (c) {
            case 'a': case 'A': dir = LEFT; break;
            case 'd': case 'D': dir = RIGHT; break;
            case 'w': case 'W': dir = UP; break;
            case 's': case 'S': dir = DOWN; break;
            case 'x': case 'X': gameOver = true; break;
        }
    }
}

void Logic() {
    int prevX = tailX[0], prevY = tailY[0], prev2X, prev2Y;
    tailX[0] = x; tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i]; prev2Y = tailY[i];
        tailX[i] = prevX; tailY[i] = prevY;
        prevX = prev2X; prevY = prev2Y;
    }

    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
    }

    // Wall collision stops game (classic snake)
    if (x < 0 || x >= width || y < 0 || y >= height) gameOver = true;

    // Self collision
    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y) gameOver = true;

    // Fruit collision
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // control game speed
    }
    cout << "Game Over! Final Score: " << score << endl;
    return 0;
}
