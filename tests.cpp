#include <iostream>
#include <cassert>
#include <queue>

// Direction macros matching snake.cpp
#define STOP 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

// Declarations of globals from snake.cpp
extern bool gameOver;
const int width = 20;
const int height = 20;
extern int x, y, fruitX, fruitY, score;
extern int tailX[100], tailY[100];
extern int nTail;
extern int dir;
extern int obsX[20], obsY[20];
extern int nObs;
extern int highscore;

// Seam interface declaration matching snake.cpp
class InputHandler {
public:
    virtual ~InputHandler() {}
    virtual bool hasKey() { return false; }
    virtual int getKey() { return 0; }
};

void setInputHandler(InputHandler* handler);
void Input();
void logic();

// Helper to reset shared global state before each test
void reset_game_state() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = 15;
    fruitY = 15;
    score = 0;
    nTail = 0;
    nObs = 0;
    setInputHandler(nullptr);
}

// ============================================================================
// PART B TESTS (Rules testable without modifying source)
// ============================================================================

void test_wall_collision_left() {
    reset_game_state();
    x = 0;
    y = 5;
    dir = LEFT;

    logic();

    assert(x == -1);
    assert(gameOver == true);
    std::cout << "[PASS] test_wall_collision_left\n";
}

void test_wall_collision_right() {
    reset_game_state();
    x = width - 1;
    y = 5;
    dir = RIGHT;

    logic();

    assert(x == width);
    assert(gameOver == true);
    std::cout << "[PASS] test_wall_collision_right\n";
}

void test_wall_collision_top() {
    reset_game_state();
    x = 5;
    y = 0;
    dir = UP;

    logic();

    assert(y == -1);
    assert(gameOver == true);
    std::cout << "[PASS] test_wall_collision_top\n";
}

void test_wall_collision_bottom() {
    reset_game_state();
    x = 5;
    y = height - 1;
    dir = DOWN;

    logic();

    assert(y == height);
    assert(gameOver == true);
    std::cout << "[PASS] test_wall_collision_bottom\n";
}

void test_self_collision() {
    reset_game_state();
    x = 5;
    y = 5;
    dir = UP; // will move to (5, 4)
    nTail = 4;
    // Tail arrangement such that (5, 4) is occupied by a tail segment
    tailX[0] = 5; tailY[0] = 4;
    tailX[1] = 5; tailY[1] = 3;
    tailX[2] = 6; tailY[2] = 3;
    tailX[3] = 6; tailY[3] = 4;

    logic();

    assert(gameOver == true);
    std::cout << "[PASS] test_self_collision\n";
}

// ============================================================================
// PART D / E TESTS (Testing the seam using a Test Stub)
// ============================================================================

// Test Double: StubInputHandler (Stub providing canned keystroke query responses)
class StubInputHandler : public InputHandler {
    std::queue<int> keys;
public:
    StubInputHandler() {}
    StubInputHandler(std::initializer_list<int> keyList) {
        for (int k : keyList) {
            keys.push(k);
        }
    }

    void queueKey(int key) {
        keys.push(key);
    }

    bool hasKey() override {
        return !keys.empty();
    }

    int getKey() override {
        if (keys.empty()) return 0;
        int k = keys.front();
        keys.pop();
        return k;
    }
};

void test_seam_input_turn_left() {
    reset_game_state();
    dir = UP;

    StubInputHandler stub({'a'});
    setInputHandler(&stub);

    Input();

    assert(dir == LEFT);
    std::cout << "[PASS] test_seam_input_turn_left\n";
    setInputHandler(nullptr);
}

void test_seam_input_turn_right() {
    reset_game_state();
    dir = UP;

    StubInputHandler stub({'d'});
    setInputHandler(&stub);

    Input();

    assert(dir == RIGHT);
    std::cout << "[PASS] test_seam_input_turn_right\n";
    setInputHandler(nullptr);
}

void test_seam_input_turn_up() {
    reset_game_state();
    dir = LEFT;

    StubInputHandler stub({'w'});
    setInputHandler(&stub);

    Input();

    assert(dir == UP);
    std::cout << "[PASS] test_seam_input_turn_up\n";
    setInputHandler(nullptr);
}

void test_seam_input_turn_down() {
    reset_game_state();
    dir = LEFT;

    StubInputHandler stub({'s'});
    setInputHandler(&stub);

    Input();

    assert(dir == DOWN);
    std::cout << "[PASS] test_seam_input_turn_down\n";
    setInputHandler(nullptr);
}

void test_seam_input_exit_key() {
    reset_game_state();
    gameOver = false;

    StubInputHandler stub({'x'});
    setInputHandler(&stub);

    Input();

    assert(gameOver == true);
    std::cout << "[PASS] test_seam_input_exit_key\n";
    setInputHandler(nullptr);
}

void test_seam_input_no_key_preserves_dir() {
    reset_game_state();
    dir = RIGHT;

    StubInputHandler emptyStub;
    setInputHandler(&emptyStub);

    Input();

    assert(dir == RIGHT);
    std::cout << "[PASS] test_seam_input_no_key_preserves_dir\n";
    setInputHandler(nullptr);
}

// ============================================================================
// MAIN RUNNER
// ============================================================================

int main() {
    std::cout << "========================================\n";
    std::cout << "Running Lab 4 Test Suite\n";
    std::cout << "========================================\n\n";

    std::cout << "-- Part B: Unmodified Rules Tests --\n";
    test_wall_collision_left();
    test_wall_collision_right();
    test_wall_collision_top();
    test_wall_collision_bottom();
    test_self_collision();

    std::cout << "\n-- Part D: Seam Tests with Stub Double --\n";
    test_seam_input_turn_left();
    test_seam_input_turn_right();
    test_seam_input_turn_up();
    test_seam_input_turn_down();
    test_seam_input_exit_key();
    test_seam_input_no_key_preserves_dir();

    std::cout << "\n========================================\n";
    std::cout << "All 11 tests PASSED successfully!\n";
    std::cout << "========================================\n";
    return 0;
}
