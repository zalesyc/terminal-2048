#ifndef BOARD_H
#define BOARD_H
#include <ncurses.h>
#include <queue>
#include <vector>

#include "tile.hpp"

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

    struct SingleTileMoveData {
        int waitingFor = 0;
        Tile* waiter;
        std::queue<Tile*> freeTiles;
    };

  private:
    void singleTileMove(SingleTileMoveData* data, Tile& currentTile);
};

#endif // BOARD_H
