#include <iostream>
#include <ncurses.h>
#include <string>
#include <utility>
#include <vector>

#include "argumentParser.h"
#include "move.h"
#include "tile.h"

void initColors();

int main(int argc, const char* argv[]) {
    // variables initialization
    // those  non-const varables can be changed using commandline arguments, so this assigment set the defaluts
    int playRows = 4;
    int playCollumns = 4;
    bool useColor;

    const int tileHeigth = 3;
    const int tileWidth = 7;
    const int boardStartingRow = 2;             // y, starting row on the screen
    const int boardStartingCollumn = 1;         // x, starting collumn on the screen
    const unsigned char startupProbability = 6; // pobability of new "2" appering on starup
    const unsigned char moveProbability = 12;   // and every move

    // commandline arguments
    argumentParser argParser(argc, argv);

    if (!argParser.setIntToOption("-r", &playRows, {3, 100}))
        return 0;
    if (!argParser.setIntToOption("-c", &playCollumns, {3, 100}))
        return 0;

    // ncurses initialization
    initscr();
    noecho();
    keypad(stdscr, TRUE);

    if (argParser.optionExists("--no-color")) {
        useColor = false;
    } else {
        useColor = has_colors();
    }

    // terminal color init
    if (useColor) {
        start_color();
        use_default_colors();

        initColors();
    }

    mvprintw(0, 1, "THE BEST TERMINAL BASED 2048");
    mvprintw(tileHeigth * playRows + boardStartingRow + 1, 1, "How to play: ");
    mvprintw(tileHeigth * playRows + boardStartingRow + 2, 1, "press 'c' to exit the game");
    refresh();

    std::vector<std::vector<Tile>> board(playRows, std::vector<Tile>(playCollumns));

    for (int row = 0; row < playRows; row++) {
        for (int coll = 0; coll < playCollumns; coll++) {

            board[row][coll].window = newwin(tileHeigth, tileWidth, (row * tileHeigth) + boardStartingRow, (coll * tileWidth) + boardStartingCollumn);
            board[row][coll].width = tileWidth;
            board[row][coll].heigth = tileHeigth;
            board[row][coll].setValue(0, true);

            Move::addRandTwos(&board[row][coll], startupProbability);

            if (useColor) {
                wbkgd(board[row][coll].window,
                      COLOR_PAIR((board[row][coll].value <= 128) ? board[row][coll].value : 128));
            } else {
                box(board[row][coll].window, 0, 0);
            }

            wrefresh(board[row][coll].window);
        }
    }
    refresh();

    bool gameIsRunning = 1;
    while (gameIsRunning) // game loop
    {
        int action = getch();
        Move::Return returnMsg;
        switch (action) {
            case (int)'c':
            case (int)'C':
                gameIsRunning = 0;
                endwin();
                return 0;
                break;

            case KEY_UP:
            case (char)'w':
                mvprintw(1, 1, "up   ");
                returnMsg = Move::Move(&board, Move::Direction::Up);
                break;

            case KEY_DOWN:
            case (char)'s':
                mvprintw(1, 1, "down ");
                returnMsg = Move::Move(&board, Move::Direction::Down);
                break;

            case KEY_RIGHT:
            case (char)'d':
                mvprintw(1, 1, "right");
                returnMsg = Move::Move(&board, Move::Direction::Right);
                break;

            case KEY_LEFT:
            case (char)'a':
                mvprintw(1, 1, "left ");
                returnMsg = Move::Move(&board, Move::Direction::Left);
                break;
        }
        if (returnMsg == Move::Return::Ok) {
            for (int row = 0; row < board.size(); row++) {
                for (int coll = 0; coll < board.at(row).size(); coll++) {
                    Move::addRandTwos(&board.at(row).at(coll), moveProbability);
                }
            }
        }
        refresh();
    }

    endwin();
    return 0;
}

void initColors() {
    init_pair(2, -1, COLOR_WHITE);
    init_pair(4, -1, COLOR_YELLOW);
    init_pair(8, -1, COLOR_GREEN);
    init_pair(16, -1, COLOR_CYAN);
    init_pair(32, -1, COLOR_BLUE);
    init_pair(64, -1, COLOR_MAGENTA);
    init_pair(128, -1, COLOR_RED);
}