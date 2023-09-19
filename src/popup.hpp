#ifndef POPUP_H
#define POPUP_H

#include <ncurses.h>
#include <string>
#include <vector>

#include "game.hpp"
#include "tile.hpp"

class Popup {
  public:
    Popup(const int row, const int column, const int height, const int width);
    Popup(App* appconfig);
    ~Popup();

    void setText(std::string text);
    void setTitle(std::string text);

  private:
    std::string print(int width);
    void drawWindow(const int topLeftX, const int topLeftY, const int bottomRightX, const int bottomRightY);

  public:
    WINDOW* m_win;

  private:
    std::string m_text;
    std::vector<std::vector<Tile>>* m_board;
    int m_winWidth;
    int m_winHeight;
};

struct SelectMenuOption {
    const std::string name;
    const bool hasValue;
    const int* value;
};

int selectMenu(WINDOW* win, int row, const int column, const std::vector<SelectMenuOption> options, const int highlightedOption);
int selectMenu(WINDOW* win, int row, const int column, const std::vector<std::string> options, const int highlightedOption);

#endif // POPUP_H
