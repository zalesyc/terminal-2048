#ifndef TILE_H
#define TILE_H

#include <ncurses.h>
#include <string>

class Tile {
  public:
    WINDOW *window;
    int width;
    int heigth;

    int value;
    void setValue(int newValue);
    void setValue(int newValue, bool redraw);

  private:
    std::string print(int width);
    void drawLabel();
};

#endif