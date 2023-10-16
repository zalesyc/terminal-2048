#ifndef BOARD_H
#define BOARD_H
#include <ncurses.h>
#include <queue>
#include <vector>

#include "tile.hpp"

class Board : public std::vector<std::vector<Tile>> {
  public:
    Board(const int rows, const int columns);

    enum moveReturn { Ok,
                      NoneMoved,
                      GameLost };

    moveReturn moveLeft();
    moveReturn moveRight();
    moveReturn moveUp();
    moveReturn moveDown();
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
    void singleTileMove(Board::SingleTileMoveData* data, Tile& currentTile, bool* wasMoved);
};

#endif // BOARD_H
