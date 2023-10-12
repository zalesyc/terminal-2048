#ifndef BOARD_H
#define BOARD_H

#include "tile.hpp"
#include <ncurses.h>
#include <vector>

class Board : public std::vector<std::vector<Tile>> {
  public:
    Board(const int rows, const int columns);

    enum moveDirection { Left,
                         Right,
                         Up,
                         Down };
    enum moveReturn { Ok,
                      NoneMoved,
                      GameLost };

    moveReturn Move(moveDirection direction);
    Tile& get(const int row, const int column);
    void redrawAll();
    void populateWithRandomTwos();

  private:
    const int m_rows;
    const int m_columns;
};

#endif // BOARD_H
