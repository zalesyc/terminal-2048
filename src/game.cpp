#include <array>
#include <cctype>
#include <ncurses.h>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "board.hpp"
#include "game.hpp"
#include "popup.hpp"

void editField(std::string& name, int* variable, std::pair<int, int> range);
void editOptions(int row, int column);
void showHelp(int row, int column);
void showAbout(int row, int column);

void colors() {
    init_pair(2, -1, COLOR_WHITE);
    init_pair(4, -1, COLOR_YELLOW);
    init_pair(8, -1, COLOR_GREEN);
    init_pair(16, -1, COLOR_CYAN);
    init_pair(32, -1, COLOR_BLUE);
    init_pair(64, -1, COLOR_MAGENTA);
    init_pair(128, -1, COLOR_RED);
}

void boardInit(Board* board) {
    for (int row = 0; row < appConfig.playRows; row++) {
        for (int col = 0; col < appConfig.playColumns; col++) {
            board->at(row).at(col).window = newwin(appConfig.tileHeigth, appConfig.tileWidth, (row * appConfig.tileHeigth) + appConfig.boardStartingRow, (col * appConfig.tileWidth) + appConfig.boardStartingColumn);
            board->at(row).at(col).width = appConfig.tileWidth;
            board->at(row).at(col).heigth = appConfig.tileHeigth;
            board->at(row).at(col).setValue(0, true);

            if (appConfig.useColor) {
                wbkgd(board->at(row).at(col).window,
                      COLOR_PAIR((board->at(row).at(col).value <= 128) ? board->at(row).at(col).value : 128));
            } else {
                box(board->at(row).at(col).window, 0, 0);
            }

            wrefresh(board->at(row).at(col).window);
        }
    }
    board->populateWithRandomTwos();
    mvprintw(1, 1, "Score: %i", score);
    refresh();
}

welcomeScreenReturn welcomeScreen() {
    clear();
    const std::array<std::string, 6> asciiText = {
        R"( ___   ___  _  _   ___  )",
        R"(|__ \ / _ \| || | / _ \ )",
        R"(   ) | | | | || || (_) |)",
        R"(  / /| | | |__   _> _ < )",
        R"( / /_| |_| |  | || (_) |)",
        R"(|____|\___/   |_| \___/ )"};

    for (int i = 0; i < asciiText.size(); i++) {
        mvprintw(i, 0, "%s", asciiText.at(i).c_str());
    }
    mvprintw(15, 1, "Use arrow keys to move and enter to select the options");
    refresh();
    while (true) {
        const int selectedOption = SelectMenu::verticalMenu(stdscr, 8, 1, {"Set options", "Help", "About", "", "Start Playing", "Exit"}, 4);

        switch (selectedOption) {
            case 0:
                editOptions(7, 1);
                break;

            case 1:
                showHelp(7, 1);
                break;

            case 2:
                showAbout(7, 1);
                break;

            case 4:
                clear();
                refresh();
                return welcomeScreenReturn::Play;

            case 5:
                return welcomeScreenReturn::Exit;
        }
    }
    return welcomeScreenReturn::Play;
}

// min, max both inclusive
int randomNumber(const int min, const int max) {
    std::random_device random_device;
    std::default_random_engine eng(random_device());
    std::uniform_int_distribution<int> uniform_dist(min, max);
    return uniform_dist(eng);
}

bool gameLost(Board* board) {
    for (int row = 0; row < board->size(); row++) {
        for (int col = 0; col < board->at(row).size(); col++) {
            if (col != board->at(row).size() - 1 && board->at(row).at(col + 1).value == board->at(row).at(col).value) {
                return false;
            }

            if (col != 0 && board->at(row).at(col - 1).value == board->at(row).at(col).value) {
                return false;
            }

            if (row != board->size() - 1 && board->at(row + 1).at(col).value == board->at(row).at(col).value) {
                return false;
            }

            if (row != 0 && board->at(row - 1).at(col).value == board->at(row).at(col).value) {
                return false;
            }
        }
    }
    return true;
}

void editField(const std::string& name, int* variable, const std::pair<int, int> range) {
    Popup field = Popup(16, 1, 4, 22);
    field.setTitle("Set " + name);
    mvwprintw(field.m_win, 1, 1, "currently: %d", *variable);
    curs_set(true);
    echo();
    wmove(field.m_win, 2, 1);
    wrefresh(field.m_win);
    char answer[50];
    wgetstr(field.m_win, answer);
    curs_set(false);
    noecho();

    const std::string strAnswer = answer;

    // check if the answer is valid
    for (char singleChar : strAnswer) {
        if (!std::isdigit(singleChar)) {
            return;
        }
    }
    if (strAnswer.empty()) {
        return;
    }

    const int intAnswer = std::stoi(strAnswer);
    if (intAnswer < range.first || intAnswer > range.second) {
        return;
    }

    *variable = intAnswer;
}

void editOptions(const int row, const int column) {
    const SelectMenu::Option BoardRows = {"Board Rows", true, &appConfig.playRows};
    const SelectMenu::Option BoardColumns = {"Board Columns", true, &appConfig.playColumns};
    const SelectMenu::Option TileHeight = {"Tile Height", true, &appConfig.tileHeigth};
    const SelectMenu::Option TileWidth = {"Tile Width", true, &appConfig.tileWidth};
    const SelectMenu::Option EmptyLine = {"", false, nullptr};
    const SelectMenu::Option Exit = {"End Editing", false, nullptr};

    Popup popup = Popup(row, column, 8, 22);
    popup.setTitle("Set Options");

    while (true) {
        const int optionToEdit = SelectMenu::verticalMenu(popup.m_win, 1, 1, {BoardRows, BoardColumns, TileHeight, TileWidth, EmptyLine, Exit}, 5);
        switch (optionToEdit) {
            case 0:
                editField("Board Rows", &appConfig.playRows, {3, 100});
                break;
            case 1:
                editField("Board Columns", &appConfig.playColumns, {3, 100});
                break;
            case 2:
                editField("Tile Height", &appConfig.tileHeigth, {3, 10});
                break;
            case 3:
                editField("Tile Width", &appConfig.tileWidth, {5, 15});
                break;
            case 5:
                return;
        }

        wrefresh(popup.m_win);
    }
}

void showHelp(const int row, const int column) {
    Popup popup = Popup(row, column, 17, 55);
    popup.setTitle("2048 Help");
    mvwprintw(popup.m_win, 1, 1, "2048 is a game about sliding tiles on a grid");
    mvwprintw(popup.m_win, 2, 1, "to combine them to make a higher value tile");
    mvwprintw(popup.m_win, 3, 1, "The objective is to create a tile with the value 2048");
    wattron(popup.m_win, A_BOLD);
    mvwprintw(popup.m_win, 5, 1, "How to play:");
    wattroff(popup.m_win, A_BOLD);
    mvwprintw(popup.m_win, 6, 1, "Use arrow keys or WSAD to move the tiles");
    mvwprintw(popup.m_win, 7, 1, "You can exit by pressing the 'c' or 'q' key");
    wattron(popup.m_win, A_BOLD);
    mvwprintw(popup.m_win, 8, 1, "Options: ");
    wattroff(popup.m_win, A_BOLD);
    mvwprintw(popup.m_win, 9, 1, "In the welcome screen, you can set some options,");
    mvwprintw(popup.m_win, 10, 1, "under the Set Options menu.");
    mvwprintw(popup.m_win, 11, 1, "Options can be also set by command line arguments,");
    mvwprintw(popup.m_win, 12, 1, "you can see all of the arguments by passing ");
    mvwprintw(popup.m_win, 13, 1, "the '--help' option.");
    mvwprintw(popup.m_win, 15, 1, "Press any key to exit the help");

    wrefresh(popup.m_win);
    getch();
}

void showAbout(const int row, const int column) {
    Popup popup = Popup(row, column, 8, 56);
    popup.setTitle("2048 Help");
    mvwprintw(popup.m_win, 1, 1, "Clone of the game 2048, played in terminal");
    mvwprintw(popup.m_win, 2, 1, "Author: Alex Cizinsky - https://www.github.com/zalesyc");
    mvwprintw(popup.m_win, 3, 1, "Licence: GPLv3");
    mvwprintw(popup.m_win, 3, 1, "Source Code: https://github.com/zalesyc/terminal-2048");

    mvwprintw(popup.m_win, 6, 1, "Press any key to exit the the about text");

    wrefresh(popup.m_win);
    getch();
}
