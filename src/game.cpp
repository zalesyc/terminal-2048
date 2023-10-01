#include <array>
#include <cctype>
#include <iostream>
#include <ncurses.h>
#include <ncurses/curses.h>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "board.hpp"
#include "game.hpp"
#include "popup.hpp"
#include "tile.hpp"

void editField(std::string name, int* variable, std::pair<int, int> range);
void editOptions(App* appConfig, const int row, const int column);
void showHelp(const int row, const int column);

void colors() {
    init_pair(2, -1, COLOR_WHITE);
    init_pair(4, -1, COLOR_YELLOW);
    init_pair(8, -1, COLOR_GREEN);
    init_pair(16, -1, COLOR_CYAN);
    init_pair(32, -1, COLOR_BLUE);
    init_pair(64, -1, COLOR_MAGENTA);
    init_pair(128, -1, COLOR_RED);
}

void boardInit(Board* board, App* appConfig) {
    for (int row = 0; row < appConfig->playRows; row++) {
        for (int coll = 0; coll < appConfig->playCollumns; coll++) {

            board->at(row).at(coll).window = newwin(appConfig->tileHeigth, appConfig->tileWidth, (row * appConfig->tileHeigth) + appConfig->boardStartingRow, (coll * appConfig->tileWidth) + appConfig->boardStartingCollumn);
            board->at(row).at(coll).width = appConfig->tileWidth;
            board->at(row).at(coll).heigth = appConfig->tileHeigth;
            board->at(row).at(coll).setValue(0, true);

            if (appConfig->useColor) {
                wbkgd(board->at(row).at(coll).window,
                      COLOR_PAIR((board->at(row).at(coll).value <= 128) ? board->at(row).at(coll).value : 128));
            } else {
                box(board->at(row).at(coll).window, 0, 0);
            }

            wrefresh(board->at(row).at(coll).window);
        }
    }
    board->populateWithRandomTwos();
    refresh();
}

welcomeScreenReturn welcomeScreen(App* appConfig) {
    // TODO: show options on the right
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
    refresh();
    while (true) {
        const int selectedOption = SelectMenu::menu(stdscr, 8, 1, {"Set options", "Help", "", "Start Playing", "Exit"}, 3);

        switch (selectedOption) {
            case 0:
                editOptions(appConfig, 7, 1);
                break;

            case 1:
                showHelp(7, 1);
                break;

            case 3:
                clear();
                refresh();
                return welcomeScreenReturn::Play;

            case 4:
                return welcomeScreenReturn::Exit;
        }
    }
    return welcomeScreenReturn::Play;
}

void editField(std::string name, int* variable, std::pair<int, int> range) {
    Popup field = Popup(16, 1, 5, 22);
    field.setTitle("Set " + name);
    mvwprintw(field.m_win, 1, 1, "currently: %d", *variable);
    mvwprintw(field.m_win, 3, 1, "Press 'c' to cancel");
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
    return;
}

void editOptions(App* appConfig, const int row, const int column) {
    const SelectMenu::Option BoardRows = {"Board Rows", true, &appConfig->playRows};
    const SelectMenu::Option BoardColumns = {"Board Columns", true, &appConfig->playCollumns};
    const SelectMenu::Option TileHeight = {"Tile Height", true, &appConfig->tileHeigth};
    const SelectMenu::Option TileWidth = {"Tile Width", true, &appConfig->tileWidth};
    const SelectMenu::Option EmptyLine = {"", false, nullptr};
    const SelectMenu::Option Exit = {"End Editing", false, nullptr};

    Popup popup = Popup(row, column, 8, 22);
    popup.setTitle("Set Options");

    while (true) {
        const int optionToEdit = SelectMenu::menu(popup.m_win, 1, 1, {BoardRows, BoardColumns, TileHeight, TileWidth, EmptyLine, Exit}, 0);
        switch (optionToEdit) {
            case 0:
                editField("Board Rows", &appConfig->playRows, {3, 100});
                break;
            case 1:
                editField("Board Columns", &appConfig->playCollumns, {3, 100});
                break;
            case 2:
                editField("Tile Height", &appConfig->tileHeigth, {3, 10});
                break;
            case 3:
                editField("Tile Width", &appConfig->tileWidth, {5, 15});
                break;
            case 5:
                return;
        }

        wrefresh(popup.m_win);
    }
}

void showHelp(const int row, const int column) {
    Popup popup = Popup(row, column, 8, 40);
    popup.setTitle("Help");
    mvwprintw(popup.m_win, 1, 1, "This is my Help");
    mvwprintw(popup.m_win, 2, 1, "Use Arrows To move");
    mvwprintw(popup.m_win, 6, 1, "press any key to exit");
    wrefresh(popup.m_win);

    getch();
}

// min, max both inclusive
int randomNumber(const int min, const int max) {
    std::random_device r;
    std::default_random_engine eng(r());
    std::uniform_int_distribution<int> uniform_dist(min, max);
    return uniform_dist(eng);
}
