#ifndef BOARD_H
#define BOARD_H

#include "tile.h"
#include <iostream>
#include <ncurses.h>
#include <vector>

class Board : public std::vector<std::vector<Tile>> {
  public:
    Board(int rows, int columns) : std::vector<std::vector<Tile>>(rows, std::vector<Tile>(columns)) {}

    enum moveDirection { Left,
                         Right,
                         Up,
                         Down };
    enum moveReturn { Ok,
                      NoneMoved,
                      GameLost };

    moveReturn Move(moveDirection direction);
};

#endif // BOARD_H
