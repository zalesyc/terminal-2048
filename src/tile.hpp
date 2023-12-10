#ifndef TILE_H
#define TILE_H

#include <ncurses.h>
#include <string>

class Tile {
  public:
    WINDOW* window;
    int value;
    int width;
    int heigth;
    void setValue(int newValue);
    void setValue(int newValue, bool redraw);
    void drawLabel();
    void addRandTwos(int probability);

  private:
    std::string print(int width) const;
};

#endif