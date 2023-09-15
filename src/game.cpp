#include <array>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <utility>
#include <vector>

#include "game.hpp"
#include "popup.hpp"
#include "random"
#include "tile.hpp"

void colors() {
    init_pair(2, -1, COLOR_WHITE);
    init_pair(4, -1, COLOR_YELLOW);
    init_pair(8, -1, COLOR_GREEN);
    init_pair(16, -1, COLOR_CYAN);
    init_pair(32, -1, COLOR_BLUE);
    init_pair(64, -1, COLOR_MAGENTA);
    init_pair(128, -1, COLOR_RED);
}

void boardInit(std::vector<std::vector<Tile>>* board, App* appConfig) {
    for (int row = 0; row < appConfig->playRows; row++) {
        for (int coll = 0; coll < appConfig->playCollumns; coll++) {

            board->at(row).at(coll).window = newwin(appConfig->tileHeigth, appConfig->tileWidth, (row * appConfig->tileHeigth) + appConfig->boardStartingRow, (coll * appConfig->tileWidth) + appConfig->boardStartingCollumn);
            board->at(row).at(coll).width = appConfig->tileWidth;
            board->at(row).at(coll).heigth = appConfig->tileHeigth;
            board->at(row).at(coll).setValue(0, true);

            board->at(row).at(coll).addRandTwos(appConfig->startupProbability);

            if (appConfig->useColor) {
                wbkgd(board->at(row).at(coll).window,
                      COLOR_PAIR((board->at(row).at(coll).value <= 128) ? board->at(row).at(coll).value : 128));
            } else {
                box(board->at(row).at(coll).window, 0, 0);
            }

            wrefresh(board->at(row).at(coll).window);
        }
    }
    refresh();
}
int selectMenu(WINDOW* win, int row, const int column, const std::vector<std::string> options, const int highlightedOption) {
    int choice = highlightedOption;

    while (1) {
        // Print the menu

        for (int i = 0; i < options.size(); i++) {
            if (i == choice) {
                wattron(win, A_REVERSE); // Highlight the current choice
            }
            mvwprintw(win, i + row, column, options.at(i).c_str());
            wattroff(win, A_REVERSE); // Turn off highlighting
        }
        wrefresh(win);

        // Get user input
        int c = getch();

        switch (c) {
            case KEY_UP:
                choice = (choice - 1 + options.size()) % options.size();
                if (options.at(choice).empty()) {
                    choice = (choice - 1 + options.size()) % options.size();
                }
                break;
            case KEY_DOWN:
                choice = (choice + 1) % options.size();
                if (options.at(choice).empty()) {
                    choice = (choice + 1) % options.size();
                }
                break;
            case 10: // Enter key
                return choice;
        }
    }
}

int selectMenu(WINDOW* win, int row, const int column, const std::vector<std::pair<std::string, int*>> options, const int highlightedOption) {
    int choice = highlightedOption;

    while (1) {
        // Print the menu

        for (int i = 0; i < options.size(); i++) {
            if (i == choice) {
                wattron(win, A_REVERSE); // Highlight the current choice
            }
            mvwprintw(win, i + row, column, "%s: %d", options.at(i).first.c_str(), *options.at(i).second);
            wattroff(win, A_REVERSE); // Turn off highlighting
        }
        wrefresh(win);

        // Get user input
        int c = getch();

        switch (c) {
            case KEY_UP:
                choice = (choice - 1 + options.size()) % options.size();
                if (options.at(choice).first.empty()) {
                    choice = (choice - 1 + options.size()) % options.size();
                }
                break;
            case KEY_DOWN:
                choice = (choice + 1) % options.size();
                if (options.at(choice).first.empty()) {
                    choice = (choice + 1) % options.size();
                }
                break;
            case 10: // Enter key
                return choice;
        }
    }
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
    getch();
    curs_set(false);
}

void editOptions(App* appConfig, const int row, const int column) {
    Popup popup = Popup(row, column, 8, 22);
    popup.setTitle("Set Options");

    const int optionToEdit = selectMenu(popup.m_win, 1, 1, {{"Board Rows", &appConfig->playRows}, {"Board Columns", &appConfig->playCollumns}, {"Tile Height", &appConfig->tileHeigth}, {"Tile Width", &appConfig->tileWidth}}, 0);
    switch (optionToEdit) {
        case 0:
            editField("Board Rows", &appConfig->playRows, {3, 100});
        case 1:
            editField("Board Columns", &appConfig->playCollumns, {3, 100});
        case 2:
            editField("Tile Height", &appConfig->tileHeigth, {3, 10});
        case 3:
            editField("Tile Width", &appConfig->tileWidth, {5, 15});
    }
    wrefresh(popup.m_win);

    getch();
}

void welcomeScreen(App* appConfig) {
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
        mvprintw(i, 0, asciiText.at(i).c_str());
    }
    refresh();

    const int selectedOption = selectMenu(stdscr, 8, 1, {"Set options", "Help", "", "Start Playing", "Exit"}, 3);

    switch (selectedOption) {
        case 0:
            editOptions(appConfig, 7, 1);
            break;

        case 1:
            // show help
            break;
        case 4:
            // return exit
            break;
    }

    refresh();
    clear();
}