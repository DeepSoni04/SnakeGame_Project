#include <bits/stdc++.h>
using namespace std;

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
void clear_screen() { system("cls"); }
void sleep_ms(int ms) { Sleep(ms); }
bool kbhit_custom() { return _kbhit(); }
int getch_custom() { return _getch(); }
void enable_ansi_colors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
}
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
void clear_screen() { write(STDOUT_FILENO, "\033[2J\033[H", 7); }
void sleep_ms(int ms) { usleep(ms * 1000); }
static struct termios orig;
bool raw_enabled = false;
void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig);
    struct termios raw = orig;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    raw_enabled = true;
}
void disable_raw_mode() {
    if (raw_enabled) tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}
bool kbhit_custom() {
    timeval tv{0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}
int getch_custom() {
    char c;
    if (read(STDIN_FILENO, &c, 1) < 0) return -1;
    return c;
}
void enable_ansi_colors() {}
#endif

struct Point { int x, y; bool operator==(const Point &o) const { return x==o.x && y==o.y; } };

enum Dir { UP, DOWN, LEFT, RIGHT };

class Snake {
public:
    deque<Point> body;
    Dir dir;
    bool grow;
    Snake(Point start) {
        dir = LEFT; grow = false;
        body = {start, {start.x, start.y+1}, {start.x, start.y+2}};
    }
    Point head() const { return body.front(); }
    void move() {
        Point h = head();
        if(dir==UP) h.x--;
        if(dir==DOWN) h.x++;
        if(dir==LEFT) h.y--;
        if(dir==RIGHT) h.y++;
        body.push_front(h);
        if(!grow) body.pop_back(); else grow = false;
    }
    void change_dir(Dir d) {
        if((dir==UP && d==DOWN)||(dir==DOWN && d==UP)||(dir==LEFT && d==RIGHT)||(dir==RIGHT && d==LEFT)) return;
        dir = d;
    }
    bool hits_self() const {
        Point h = head();
        return find(body.begin()+1, body.end(), h) != body.end();
    }
    bool is_on(Point p) const {
        for(auto &b: body) if(b==p) return true;
        return false;
    }
};

class Food {
public:
    Point pos;
    Food() { pos = {-1,-1}; }
};

class Game {
    int rows, cols, score, highscore, speed;
    bool game_over;
    string hs_file;
    Snake snake;
    Food food;
public:
    Game(int r=20, int c=40) : rows(r), cols(c), snake({r/2,c/2}) {
        score = 0; highscore = 0; speed = 150; game_over = false;
        hs_file = "highscore.txt";
        srand(time(NULL));
        load_highscore();
        spawn_food();
    }
    void load_highscore() {
        ifstream f(hs_file);
        if(f) f >> highscore;
    }
    void save_highscore() {
        if(score > highscore) {
            highscore = score;
            ofstream f(hs_file);
            f << highscore;
        }
    }
    void spawn_food() {
        do {
            food.pos = {rand()%rows, rand()%cols};
        } while(snake.is_on(food.pos));
    }
    void render() {
        cout << "\033[H";
        cout << "\033[1;34m+" << string(cols, '-') << "+\033[0m\n";
        for(int i=0;i<rows;i++) {
            cout << "\033[1;34m|\033[0m";
            for(int j=0;j<cols;j++) {
                Point p{i,j};
                if(snake.head()==p) cout << "\033[1;32mO\033[0m";
                else if(snake.is_on(p)) cout << "\033[0;32mo\033[0m";
                else if(food.pos==p) cout << "\033[1;31m*\033[0m";
                else cout << " ";
            }
            cout << "\033[1;34m|\033[0m\n";
        }
        cout << "\033[1;34m+" << string(cols, '-') << "+\033[0m\n";
        cout << "\033[1;33mScore: " << score << "   High: " << highscore << "\033[0m\n";
        cout.flush();
    }
    void update() {
        snake.move();
        Point h = snake.head();
        if(h.x<0||h.x>=rows||h.y<0||h.y>=cols||snake.hits_self()) game_over=true;
        if(h==food.pos) {
            score++;
            snake.grow = true;
            spawn_food();
            if(speed > 60) speed -= 5;
        }
    }
    void handle_input(int ch) {
        if(ch=='w'||ch=='W'||ch==72) snake.change_dir(UP);
        else if(ch=='s'||ch=='S'||ch==80) snake.change_dir(DOWN);
        else if(ch=='a'||ch=='A'||ch==75) snake.change_dir(LEFT);
        else if(ch=='d'||ch=='D'||ch==77) snake.change_dir(RIGHT);
    }
    void run() {
#ifdef _WIN32
        enable_ansi_colors();
#else
        enable_raw_mode();
#endif
        clear_screen();
        cout << "\033[?25l"; // hide cursor
        while(true) {
            score = 0; game_over = false; speed = 150;
            snake = Snake({rows/2, cols/2});
            spawn_food();
            while(!game_over) {
                if(kbhit_custom()) {
                    int c = getch_custom();
#ifdef _WIN32
                    if(c==0 || c==224) c=getch_custom();
#endif
                    handle_input(c);
                }
                update();
                render();
                sleep_ms(speed);
            }
            save_highscore();
            cout << "\033[?25h"; // show cursor
#ifdef _WIN32
            clear_screen();
#else
            clear_screen();
#endif
            cout << "\n\n\t\033[1;31mGAME OVER\033[0m\n\n";
            cout << "\t\033[1;33mYour Score:\033[0m " << score << "\n";
            cout << "\t\033[1;33mHigh Score:\033[0m " << highscore << "\n\n";
            cout << "\tPress \033[1;32mR\033[0m to Restart or \033[1;31mQ\033[0m to Quit\n";
            cout.flush();
            char ch;
            while(true) {
                if(kbhit_custom()) {
                    ch = getch_custom();
                    if(ch=='r'||ch=='R') break;
                    if(ch=='q'||ch=='Q') {
                        cout << "\033[?25h";
#ifndef _WIN32
                        disable_raw_mode();
#endif
                        return;
                    }
                }
                sleep_ms(100);
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Game g(20,40);
    g.run();
    return 0;
}
