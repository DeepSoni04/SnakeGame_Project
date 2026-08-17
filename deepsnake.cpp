#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
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
    tcsetattr(0, TCSANOW, &term);
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    tcsetattr(0, TCSANOW, &term2);
    return byteswaiting > 0;
}
char _getch() {
    char buf = 0;
    termios old = {0};
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
    return buf;
}
void Sleep(int ms) { usleep(ms * 1000); }
#endif

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class Snake {
private:
    Position head;
    vector<Position> body;
    Direction dir;
    int score;
    bool alive;

public:
    Snake(int startX, int startY) {
        head = {startX, startY};
        dir = STOP;
        score = 0;
        alive = true;
    }

    Position getHead() const { return head; }
    const vector<Position>& getBody() const { return body; }
    Direction getDirection() const { return dir; }
    int getScore() const { return score; }
    bool isAlive() const { return alive; }

    void setDirection(Direction newDir) {
        if (body.size() > 0) {
            if ((dir == LEFT && newDir == RIGHT) || (dir == RIGHT && newDir == LEFT) ||
                (dir == UP && newDir == DOWN) || (dir == DOWN && newDir == UP)) {
                return;
            }
        }
        dir = newDir;
    }

    void kill() { alive = false; }
    void addScore(int pts) { score += pts; }

    void move() {
        if (dir == STOP || !alive) return;

        if (!body.empty()) {
            for (size_t i = body.size() - 1; i > 0; --i) {
                body[i] = body[i - 1];
            }
            body[0] = head;
        }

        switch (dir) {
        case LEFT:  head.x--; break;
        case RIGHT: head.x++; break;
        case UP:    head.y--; break;
        case DOWN:  head.y++; break;
        default: break;
        }
    }

    void grow() {
        if (body.empty()) {
            body.push_back(head);
        } else {
            body.push_back(body.back());
        }
    }

    bool checkSelfCollision() const {
        for (const auto& segment : body) {
            if (head == segment) return true;
        }
        return false;
    }

    bool occupies(const Position& pos) const {
        if (head == pos) return true;
        for (const auto& segment : body) {
            if (segment == pos) return true;
        }
        return false;
    }
};

class HighScoreManager {
private:
    string filename;
    int highscore;

public:
    HighScoreManager(const string& fname = "highscore.txt") : filename(fname), highscore(0) {
        load();
    }

    int getHighScore() const { return highscore; }

    void load() {
        ifstream file(filename);
        if (file.is_open()) {
            file >> highscore;
            file.close();
        } else {
            highscore = 0;
        }
    }

    void updateIfHigher(int score) {
        if (score > highscore) {
            highscore = score;
            save();
        }
    }

    void save() {
        ofstream file(filename);
        if (file.is_open()) {
            file << highscore;
            file.close();
        }
    }
};

class Game {
private:
    const int width;
    const int height;
    Snake snake;
    Position fruit;
    vector<Position> obstacles;
    HighScoreManager highScoreMgr;
    bool gameOver;

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void generateFruit() {
        do {
            fruit.x = rand() % width;
            fruit.y = rand() % height;
        } while (isOccupied(fruit, false));
    }

    void generateObstacles(int count) {
        obstacles.clear();
        for (int i = 0; i < count; i++) {
            Position obs;
            do {
                obs.x = rand() % width;
                obs.y = rand() % height;
            } while (obs == snake.getHead() || obs == fruit || isObstacleAt(obs));
            obstacles.push_back(obs);
        }
    }

    bool isObstacleAt(const Position& pos) const {
        for (const auto& obs : obstacles) {
            if (obs == pos) return true;
        }
        return false;
    }

    bool isOccupied(const Position& pos, bool checkFruit = true) const {
        if (snake.occupies(pos)) return true;
        if (isObstacleAt(pos)) return true;
        if (checkFruit && fruit == pos) return true;
        return false;
    }

public:
    Game(int w = 20, int h = 20) : width(w), height(h), snake(w / 2, h / 2), gameOver(false) {}

    void setup() {
        srand(static_cast<unsigned int>(time(0)));
        snake = Snake(width / 2, height / 2);
        gameOver = false;
        generateFruit();
        generateObstacles(4);
    }

    void draw() {
        clearScreen();

        for (int i = 0; i < width + 2; i++) cout << "⬛";
        cout << endl;

        for (int y = 0; y < height; y++) {
            cout << "⬛";
            for (int x = 0; x < width; x++) {
                Position current = {x, y};
                if (current == snake.getHead()) {
                    cout << "🐍";
                } else if (current == fruit) {
                    cout << "🍎";
                } else if (isObstacleAt(current)) {
                    cout << "🧱";
                } else {
                    bool isBody = false;
                    for (const auto& seg : snake.getBody()) {
                        if (seg == current) {
                            cout << "🟩";
                            isBody = true;
                            break;
                        }
                    }
                    if (!isBody) cout << "  ";
                }
            }
            cout << "⬛" << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "⬛";
        cout << endl;

        cout << "\nScore: " << snake.getScore() << "   High Score: " << highScoreMgr.getHighScore() << endl;
        cout << "Controls: W/A/S/D  |  X = Exit" << endl;
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
            case 'a': case 'A': snake.setDirection(LEFT); break;
            case 'd': case 'D': snake.setDirection(RIGHT); break;
            case 'w': case 'W': snake.setDirection(UP); break;
            case 's': case 'S': snake.setDirection(DOWN); break;
            case 'x': case 'X': gameOver = true; break;
            }
        }
    }

    void updateLogic() {
        snake.move();
        Position head = snake.getHead();

        // Boundary collision
        if (head.x < 0 || head.x >= width || head.y < 0 || head.y >= height) {
            gameOver = true;
            snake.kill();
        }

        // Obstacle collision
        if (isObstacleAt(head)) {
            gameOver = true;
            snake.kill();
        }

        // Self collision
        if (snake.checkSelfCollision()) {
            gameOver = true;
            snake.kill();
        }

        // Fruit eating
        if (head == fruit) {
            snake.addScore(10);
            snake.grow();
            generateFruit();
        }
    }

    void run() {
        highScoreMgr.load();
        char choice;
        do {
            setup();
            while (!gameOver) {
                draw();
                handleInput();
                updateLogic();
                int speed = max(60, 150 - (snake.getScore() / 5));
                Sleep(speed);
            }

            clearScreen();
            cout << "\n💀 Game Over! Final Score = " << snake.getScore() << endl;
            if (snake.getScore() > highScoreMgr.getHighScore()) {
                highScoreMgr.updateIfHigher(snake.getScore());
                cout << "🏆 New High Score!" << endl;
            }

            cout << "\nPress (R) to Replay or (Q) to Quit: ";
            cin >> choice;
            choice = tolower(choice);
        } while (choice == 'r');

        cout << "\n🐍 Thanks for playing Snake Game!\n";
    }
};

int main() {
    Game game(20, 20);
    game.run();
    return 0;
}