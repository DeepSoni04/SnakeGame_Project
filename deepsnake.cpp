#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

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
    string name;
    Position head;
    vector<Position> body;
    Direction dir;
    int score;
    bool alive;

public:
    Snake(string pName, int startX, int startY, Direction initDir)
        : name(pName), head({startX, startY}), dir(initDir), score(0), alive(true) {}

    string getName() const { return name; }
    Position getHead() const { return head; }
    const vector<Position>& getBody() const { return body; }
    Direction getDirection() const { return dir; }
    int getScore() const { return score; }
    bool isAlive() const { return alive; }

    void setDirection(Direction newDir) {
        if (!body.empty()) {
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

    bool checkBodyCollision(const Position& pos) const {
        for (const auto& segment : body) {
            if (segment == pos) return true;
        }
        return false;
    }

    bool occupies(const Position& pos) const {
        if (head == pos) return true;
        return checkBodyCollision(pos);
    }
};

class HighScoreManager {
private:
    string filename;
    int highscoreP1;
    int highscoreP2;

public:
    HighScoreManager(const string& fname = "highscore.txt")
        : filename(fname), highscoreP1(0), highscoreP2(0) {
        load();
    }

    int getHighScoreP1() const { return highscoreP1; }
    int getHighScoreP2() const { return highscoreP2; }

    void load() {
        ifstream file(filename);
        if (file.is_open()) {
            file >> highscoreP1 >> highscoreP2;
            file.close();
        } else {
            highscoreP1 = 0;
            highscoreP2 = 0;
        }
    }

    void update(int s1, int s2) {
        if (s1 > highscoreP1) highscoreP1 = s1;
        if (s2 > highscoreP2) highscoreP2 = s2;
        save();
    }

    void save() {
        ofstream file(filename);
        if (file.is_open()) {
            file << highscoreP1 << " " << highscoreP2;
            file.close();
        }
    }
};

class Game {
private:
    const int width;
    const int height;
    Snake player1;
    Snake player2;
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
            } while (player1.occupies(obs) || player2.occupies(obs) || obs == fruit || isObstacleAt(obs));
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
        if (player1.occupies(pos) || player2.occupies(pos)) return true;
        if (isObstacleAt(pos)) return true;
        if (checkFruit && fruit == pos) return true;
        return false;
    }

public:
    Game(int w = 25, int h = 20)
        : width(w), height(h),
          player1("Player 1", w / 4, h / 2, RIGHT),
          player2("Player 2", (3 * w) / 4, h / 2, LEFT),
          gameOver(false) {}

    void setup() {
        srand(static_cast<unsigned int>(time(0)));
        player1 = Snake("Player 1", width / 4, height / 2, RIGHT);
        player2 = Snake("Player 2", (3 * width) / 4, height / 2, LEFT);
        gameOver = false;
        generateFruit();
        generateObstacles(6);
    }

    void draw() {
        clearScreen();

        for (int i = 0; i < width + 2; i++) cout << "⬛";
        cout << endl;

        for (int y = 0; y < height; y++) {
            cout << "⬛";
            for (int x = 0; x < width; x++) {
                Position current = {x, y};
                if (current == player1.getHead()) {
                    cout << "🐍";
                } else if (current == player2.getHead()) {
                    cout << "🐲";
                } else if (current == fruit) {
                    cout << "🍎";
                } else if (isObstacleAt(current)) {
                    cout << "🧱";
                } else if (player1.checkBodyCollision(current)) {
                    cout << "🟩";
                } else if (player2.checkBodyCollision(current)) {
                    cout << "🟦";
                } else {
                    cout << "  ";
                }
            }
            cout << "⬛" << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "⬛";
        cout << endl;

        cout << "\n🎮 MULTIPLAYER MODE" << endl;
        cout << "P1 (🐍/🟩) Score: " << player1.getScore() << " (Best: " << highScoreMgr.getHighScoreP1() << ")" << endl;
        cout << "P2 (🐲/🟦) Score: " << player2.getScore() << " (Best: " << highScoreMgr.getHighScoreP2() << ")" << endl;
        cout << "Controls: P1 [W/A/S/D] | P2 [I/J/K/L] | X = Exit" << endl;
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
            // Player 1
            case 'a': case 'A': player1.setDirection(LEFT); break;
            case 'd': case 'D': player1.setDirection(RIGHT); break;
            case 'w': case 'W': player1.setDirection(UP); break;
            case 's': case 'S': player1.setDirection(DOWN); break;

            // Player 2
            case 'j': case 'J': player2.setDirection(LEFT); break;
            case 'l': case 'L': player2.setDirection(RIGHT); break;
            case 'i': case 'I': player2.setDirection(UP); break;
            case 'k': case 'K': player2.setDirection(DOWN); break;

            case 'x': case 'X': gameOver = true; break;
            }
        }
    }

    void updateLogic() {
        player1.move();
        player2.move();

        Position h1 = player1.getHead();
        Position h2 = player2.getHead();

        // P1 Boundary & Obstacle & Self Collision
        if (h1.x < 0 || h1.x >= width || h1.y < 0 || h1.y >= height ||
            isObstacleAt(h1) || player1.checkSelfCollision() || player2.occupies(h1)) {
            player1.kill();
        }

        // P2 Boundary & Obstacle & Self Collision
        if (h2.x < 0 || h2.x >= width || h2.y < 0 || h2.y >= height ||
            isObstacleAt(h2) || player2.checkSelfCollision() || player1.occupies(h2)) {
            player2.kill();
        }

        // Head-on collision
        if (h1 == h2) {
            player1.kill();
            player2.kill();
        }

        if (!player1.isAlive() || !player2.isAlive()) {
            gameOver = true;
        }

        // P1 Fruit
        if (h1 == fruit) {
            player1.addScore(10);
            player1.grow();
            generateFruit();
        }

        // P2 Fruit
        if (h2 == fruit) {
            player2.addScore(10);
            player2.grow();
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
                int maxScore = max(player1.getScore(), player2.getScore());
                int speed = max(60, 150 - (maxScore / 5));
                Sleep(speed);
            }

            clearScreen();
            cout << "\n💀 Game Over!" << endl;
            cout << "P1 Final Score: " << player1.getScore() << endl;
            cout << "P2 Final Score: " << player2.getScore() << endl;

            if (!player1.isAlive() && !player2.isAlive()) {
                cout << "🤝 It's a draw!" << endl;
            } else if (player1.isAlive()) {
                cout << "🏆 Player 1 Wins!" << endl;
            } else {
                cout << "🏆 Player 2 Wins!" << endl;
            }

            highScoreMgr.update(player1.getScore(), player2.getScore());

            cout << "\nPress (R) to Replay or (Q) to Quit: ";
            cin >> choice;
            choice = tolower(choice);
        } while (choice == 'r');

        cout << "\n🐍 Thanks for playing Multiplayer Snake Game!\n";
    }
};

int main() {
    Game game(25, 20);
    game.run();
    return 0;
}