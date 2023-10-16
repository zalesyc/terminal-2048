#include <iostream>
#include <ncurses.h>
#include <string>
#include <utility>
#include <vector>

#include "argumentParser.hpp"
#include "board.hpp"
#include "game.hpp"
#include "popup.hpp"
#include "tile.hpp"

int main(int argc, const char* argv[]) {
    // variables initialization
    App app;

    // commandline arguments
    argumentParser argParser(argc, argv); // I cannot declare it in the if statement, because it is a different scope
    if (argc > 1) {
        if (!argParser.setIntToOption("-r", &app.playRows, {3, 100}))
            return 0;
        if (!argParser.setIntToOption("--rows", &app.playRows, {3, 100}))
            return 0;
        if (!argParser.setIntToOption("-c", &app.playCollumns, {3, 100}))
            return 0;
        if (!argParser.setIntToOption("--columns", &app.playCollumns, {3, 100}))
            return 0;
        if (!argParser.setIntToOption("--tile-width", &app.tileWidth, {5, 15}))
            return 0;
        if (!argParser.setIntToOption("--tile-height", &app.tileHeigth, {3, 10}))
            return 0;

        if (argParser.optionExists("--help")) {
            std::cout << "Usage: terminal-2048 [options] \n\nOptions: \n"
                      << " -r --rows      NUMBER  Set How many rows the board has, range: 3, 100\n"
                      << " -c --columns   NUMBER  Set How many columns the board has, range: 3, 100\n"
                      << " --tile-width   NUMBER  Set How wide is single tile on the board in characters, range: 5, 15\n"
                      << " --tile-height  NUMBER  Set How high is single tile on the board in rows, range: 3, 10\n"
                      << " --no-color             Don't use colors\n"
                      << " --skip-welcome         Don't whow the welcome screen\n"
                      << " --help                 Show this help\n";
            return 0;
        }
    }

    // ncurses initialization
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    if (argc > 0 && argParser.optionExists("--no-color")) {
        app.useColor = false;
    } else {
        app.useColor = has_colors();
    }

    // terminal color init
    if (app.useColor) {
        start_color();
        use_default_colors();
        colors();
    }
    // end of inits

    // welcome screen
    if (!argParser.optionExists("--skip-welcome")) {
        if (welcomeScreen(&app) == welcomeScreenReturn::Exit) {
            endwin();
            return 0;
        }
    }

    // create and draw board
    mvprintw(0, 1, "THE BEST TERMINAL BASED 2048");
    mvprintw(app.tileHeigth * app.playRows + app.boardStartingRow + 1, 1, "How to play: ");
    mvprintw(app.tileHeigth * app.playRows + app.boardStartingRow + 2, 1, "press 'c' to exit the game");
    refresh();

    Board board(app.playRows, app.playCollumns);
    boardInit(&board, &app);

    // game loop
    bool alreadyWon = false;
    while (true) {
        int action = getch();
        Board::moveReturn returnMsg;
        switch (action) {
            case 'c':
            case 'C':
            case 'Q':
            case 'q':
                endwin();
                return 0;
                break;

            case KEY_UP:
            case (char)'w':
                mvprintw(1, 1, "up   ");
                returnMsg = board.moveUp();
                break;

            case KEY_DOWN:
            case (char)'s':
                mvprintw(1, 1, "down ");
                returnMsg = board.moveDown();
                break;

            case KEY_RIGHT:
            case (char)'d':
                mvprintw(1, 1, "right");
                returnMsg = board.moveRight();
                break;

            case KEY_LEFT:
            case (char)'a':
                mvprintw(1, 1, "left ");
                returnMsg = board.moveLeft();
                break;
        }

        if (returnMsg == Board::moveReturn::Ok) {
            bool noZeroInBoard = true;
            for (auto& row : board) {
                for (Tile& tile : row) {
                    tile.addRandTwos(app.moveProbability);
                    if (tile.value == 0) {
                        noZeroInBoard = false;
                    } else if (tile.value == 2048 && !alreadyWon) {
                        // win popup
                        Popup winPopup(2, 2, 10, 40);
                        winPopup.setText("YOU WON !");
                        int option = SelectMenu::horizontalMenu(winPopup.m_win, winPopup.m_winHeight - 2, 1, {"Exit", "Continue Playing"}, 0);
                        if (option == 0) {
                            clear();
                            refresh();
                            endwin();
                            return 0;
                        }
                        winPopup.delWindow();
                        board.redrawAll();
                        alreadyWon = true;
                        continue;
                    }
                }
            }

            if (noZeroInBoard) {
                if (gameLost(&board)) {
                    // lose popup
                    Popup lostPopup(2, 2, 10, 40);
                    lostPopup.setText("YOU LOST !");
                    while (getch() != 'c') {
                    }
                    endwin();
                    return 0;
                }
            }
        }
        refresh();
    }

    endwin();
    return 0;
}
