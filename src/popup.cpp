#include "popup.h"
#include "game.h"
#include "tile.h"
#include <cmath>
#include <string>
#include <vector>

Popup::Popup(App* appconfig, std::vector<std::vector<Tile>>* board, int height, int width) {
    m_appconfig = appconfig;
    m_board = board;
    m_winWidth = width;
    m_winHeight = height;
    m_win = newwin(height, width, m_appconfig->boardStartingRow + ((m_appconfig->playRows * m_appconfig->tileHeigth) / 2) - (height / 2), m_appconfig->boardStartingCollumn + ((m_appconfig->playCollumns * m_appconfig->tileWidth) / 2) - (width / 2));
    box(m_win, 0, 0);
    wrefresh(m_win);
}

Popup::~Popup() {
    this->setText(std::string(m_text.length(), ' '));
    wborder(m_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(m_win);
    delwin(m_win);
    for (auto& row : *m_board) {
        for (Tile& tile : row) {
            wbkgd(tile.window, 0); // this is workaround to recolor window, because if i set window to the same color, it doesn't recolor,
            tile.drawLabel();      // so i am setting it to 0 and then to its color again.
        }
    }
}

void Popup::setText(std::string text) {
    m_text = text;
    if (m_text.length() < m_winWidth) {
        mvwprintw(m_win, std::ceil((m_winHeight / 2.0) - 1), 1, "%s", this->print(m_winWidth - 2).c_str());
        wrefresh(m_win);
    }
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
