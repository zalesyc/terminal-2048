#include "popup.hpp"
#include "game.hpp"
#include <cmath>
#include <iostream>
#include <ncurses/curses.h>
#include <sstream>
#include <string>
#include <vector>

Popup::Popup(App* appconfig) {
    // calculating the size and position of the popup
    const int rowMargin = appconfig->tileHeigth * ((appconfig->playRows > 6) ? 2 : 0.5);
    const int colMargin = appconfig->tileWidth * ((appconfig->playCollumns > 6) ? 2 : 0.5);
    const int column = appconfig->boardStartingCollumn + colMargin;
    const int row = appconfig->boardStartingRow + rowMargin;
    const int width = (appconfig->playCollumns * appconfig->tileWidth) - colMargin;
    const int height = (appconfig->playRows * appconfig->tileHeigth) - rowMargin;
    this->drawWindow(row, column, height, width);
}

Popup::Popup(const int row, const int column, const int height, const int width) {
    this->drawWindow(row, column, height, width);
}

Popup::~Popup() {
    this->setText(std::string(m_text.length(), ' '));
    werase(m_win);
    wrefresh(m_win);
    delwin(m_win);
}

void Popup::setText(std::string text) {
    m_text = text;
    mvwprintw(m_win, std::ceil((m_winHeight / 2.0) - 1), 1, "%s", this->print(m_winWidth - 2).c_str());
    wrefresh(m_win);
}

void Popup::setTitle(std::string text) {
    mvwprintw(this->m_win, 0, 0, "%s", text.c_str());
    wrefresh(this->m_win);
}

std::string Popup::print(int width) {
    if (width < m_text.length()) {
        return m_text;
    }

    int diff = width - m_text.length();
    int pad1 = diff / 2;
    int pad2 = diff - pad1;
    return std::string(pad1, ' ') + m_text + std::string(pad2, ' ');
}

void Popup::drawWindow(const int row, const int column, const int height, const int width) {
    m_winWidth = width;
    m_winHeight = height;

    m_win = newwin(m_winHeight, m_winWidth, row, column);
    box(m_win, 0, 0);
    wrefresh(m_win);
    refresh();
}

int SelectMenu::verticalMenu(WINDOW* win, int row, const int column, const std::vector<SelectMenu::Option> options, const int highlightedOption) {
    int choice = highlightedOption;

    while (1) {
        // Print the menu

        for (int i = 0; i < options.size(); i++) {
            if (i == choice) {
                wattron(win, A_REVERSE); // Highlight the current choice
            }
            if (options.at(i).hasValue) {
                mvwprintw(win, i + row, column, "%s: %d", options.at(i).name.c_str(), *options.at(i).value);
            } else {
                mvwprintw(win, i + row, column, "%s", options.at(i).name.c_str());
            }
            wattroff(win, A_REVERSE); // Turn off highlighting
        }
        wrefresh(win);

        // Get user input
        int c = getch();

        switch (c) {
            case KEY_UP:
                choice = (choice - 1 + options.size()) % options.size();
                if (options.at(choice).name.empty()) {
                    choice = (choice - 1 + options.size()) % options.size();
                }
                break;
            case KEY_DOWN:
                choice = (choice + 1) % options.size();
                if (options.at(choice).name.empty()) {
                    choice = (choice + 1) % options.size();
                }
                break;
            case 10: // Enter key
                return choice;
        }
    }
}

int SelectMenu::verticalMenu(WINDOW* win, int row, const int column, const std::vector<std::string> options, const int highlightedOption) {
    int choice = highlightedOption;

    while (1) {
        // Print the menu

        for (int i = 0; i < options.size(); i++) {
            if (i == choice) {
                wattron(win, A_REVERSE); // Highlight the current choice
            }
            mvwprintw(win, i + row, column, "%s", options.at(i).c_str());
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

int SelectMenu::horizontalMenu(WINDOW* win, int row, const int column, const std::vector<std::string> options, const int highlightedOption) {
    int choice = highlightedOption;

    while (1) {
        // Print the menu
        for (int i = 0; i < options.size(); i++) {
            if (i == choice) {
                wattron(win, A_REVERSE); // Highlight the current choice
            }
            mvwprintw(win, row, (i * (options.at(i).length() + 4)) + column, "%s", options.at(i).c_str());
            wattroff(win, A_REVERSE); // Turn off highlighting
        }
        wrefresh(win);

        // Get user input
        int c = getch();

        switch (c) {
            case KEY_LEFT:
                choice = (choice - 1 + options.size()) % options.size();
                if (options.at(choice).empty()) {
                    choice = (choice - 1 + options.size()) % options.size();
                }
                break;
            case KEY_RIGHT:
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