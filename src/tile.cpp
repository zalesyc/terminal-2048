#include <cmath>
#include <ncurses.h>
#include <string>

#include "game.hpp"
#include "tile.hpp"

void Tile::setValue(int newValue) {
    if (value != newValue) {
        value = newValue;
        drawLabel();
    }
}

void Tile::setValue(int newValue, bool redraw) {
    if (redraw) {
        value = newValue;
        drawLabel();
    }
}

void Tile::addRandTwos(const int probability) {
    // probability is the chance of showing new 2 on every move
    if (value == 0) {
        if (randomNumber(1, probability) == 1) {
            this->setValue(2, true);
        }
    }
}

std::string Tile::print(int width) const {
    std::string strValue = std::to_string(value);
    int len = strValue.length();
    if (width < len) {
        return strValue;
    }

    int diff = width - len;
    int pad1 = diff / 2;
    int pad2 = diff - pad1;
    return std::string(pad1, ' ') + strValue + std::string(pad2, ' ');
}

void Tile::drawLabel() {
    mvwprintw(window, std::ceil((this->heigth / 2.0) - 1), 1, "%s", print(this->width - 2).c_str());
    if (has_colors()) {
        wbkgd(window, COLOR_PAIR((value <= 128) ? value : 128));
    }
    wrefresh(window);
}
