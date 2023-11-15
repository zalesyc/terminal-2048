#include <iostream>
#include <ncurses/curses.h>
#include <string>
#include <utility>
#include <vector>

#include "argumentParser.hpp"
#include "board.hpp"
#include "game.hpp"
#include "popup.hpp"
#include "tile.hpp"

App appConfig;
int score = 0;

int main(int argc, const char* argv[]) {

    // commandline arguments
    ArgumentParser argParser(argc, argv); // I cannot declare it in the if statement, because it is a different scope
    if (argc > 1) {
        if (!argParser.setIntToOption("-r", &appConfig.playRows, {3, 100})) {
            return 1;
        }
        if (!argParser.setIntToOption("--rows", &appConfig.playRows, {3, 100})) {
            return 1;
        }

        if (!argParser.setIntToOption("-c", &appConfig.playColumns, {3, 100})) {
            return 1;
        }

        if (!argParser.setIntToOption("--columns", &appConfig.playColumns, {3, 100})) {
            return 1;
        }

        if (!argParser.setIntToOption("--tile-width", &appConfig.tileWidth, {5, 15})) {
            return 1;
        }

        if (!argParser.setIntToOption("--tile-height", &appConfig.tileHeigth, {3, 10})) {
            return 1;
        }

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
        appConfig.useColor = false;
    } else {
        appConfig.useColor = has_colors();
    }

    // terminal color init
    if (appConfig.useColor) {
        start_color();
        use_default_colors();
        colors();
    }
    // end of inits

    // welcome screen
    if (!argParser.optionExists("--skip-welcome")) {
        if (welcomeScreen() == welcomeScreenReturn::Exit) {
            endwin();
            return 0;
        }
    }

    // create and draw board
    mvprintw(0, 1, "THE BEST TERMINAL BASED 2048");
    mvprintw(appConfig.tileHeigth * appConfig.playRows + appConfig.boardStartingRow + 1, 1, "How to play: ");
    mvprintw(appConfig.tileHeigth * appConfig.playRows + appConfig.boardStartingRow + 2, 1, "press 'c' to exit the game");
    refresh();

    Board board(appConfig.playRows, appConfig.playColumns);
    boardInit(&board);

    // game loop
    bool alreadyWon = false;
    while (true) {
        int action = getch();
        Board::moveReturn returnMsg = Board::moveReturn::NoneMoved;
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
                returnMsg = board.moveUp();
                break;

            case KEY_DOWN:
            case (char)'s':
                returnMsg = board.moveDown();
                break;

            case KEY_RIGHT:
            case (char)'d':
                returnMsg = board.moveRight();
                break;

            case KEY_LEFT:
            case (char)'a':
                returnMsg = board.moveLeft();
                break;
        }
        mvprintw(1, 1, "Score: %i", score);
        refresh();

        if (returnMsg == Board::moveReturn::Ok) {
            bool noZeroInBoard = true;
            for (auto& row : board) {
                for (Tile& tile : row) {
                    tile.addRandTwos(appConfig.moveProbability);
                    if (tile.value == 0) {
                        noZeroInBoard = false;
                    } else if (tile.value == 2048 && !alreadyWon) {
                        // win popup
                        Popup winPopup(2, 2, 10, 40);
                        winPopup.setTextCenteredAtRow(1, "YOU WON !");
                        winPopup.setTextCenteredAtRow(2, "Score: " + std::to_string(score));
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
                    lostPopup.setTextCenteredAtRow(1, "YOU LOST !");
                    lostPopup.setTextCenteredAtRow(2, "Score: " + std::to_string(score));
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
