#include <ncurses.h>
#include <vector>
#include <array>
#include <string>
#include <iostream>

#include "popup.hpp"
#include "game.hpp"
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
int selectMenu(const int row, const int column, const std::vector<std::string> options, const int highlightedOption) {
    int choice = highlightedOption;

    while (1) {
        // Print the menu
        
        for (int i = 0; i < options.size(); i++) {
            if (i == choice) {
                attron(A_REVERSE);  // Highlight the current choice
            }
            mvprintw(i+row , column, options.at(i).c_str());
            attroff(A_REVERSE);  // Turn off highlighting
            
        }
        refresh();

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
            case 10:  // Enter key
                return choice;
        }
    }
}

void welcomeScreen(App appConfig) {
    clear();
    const std::array<std::string, 6> asciiText = {
    R"( ___   ___  _  _   ___  )", 
    R"(|__ \ / _ \| || | / _ \ )",
    R"(   ) | | | | || || (_) |)",
    R"(  / /| | | |__   _> _ < )",
    R"( / /_| |_| |  | || (_) |)",
    R"(|____|\___/   |_| \___/ )"                   
    };

    for (int i = 0; i < asciiText.size(); i++) {
        mvprintw(i, 0, asciiText.at(i).c_str());
    }
    refresh();

    const int selectedOption = selectMenu(8, 1, {"Set options", "Help", "", "StartPlaying"}, 3);

    switch (selectedOption)
    {
    case 0:
        // edit options
        break;
    
    case 1:
        //show help
        break;
    }


    refresh();
    clear();
    }