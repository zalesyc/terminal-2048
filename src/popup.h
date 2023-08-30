#ifndef POPUP_H
#define POPUP_H

#include <ncurses.h>
#include <string>
#include <vector>

#include "game.h"
#include "tile.h"

class Popup {
  public:
    Popup(App* appconfig, std::vector<std::vector<Tile>>* board);
    ~Popup();

    void setText(std::string text);

  private:
    std::string print(int width);

  private:
    WINDOW* m_win;
    std::string m_text;
    App* m_appconfig;
    std::vector<std::vector<Tile>>* m_board;
    int m_winWidth;
    int m_winHeight;
};

#endif // POPUP_H
