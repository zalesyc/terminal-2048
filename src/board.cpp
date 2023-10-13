#include <algorithm>
#include <memory>
#include <queue>
#include <vector>

#include "board.hpp"
#include "game.hpp"
#include "tile.hpp"

void singleTileMove(bool* moved, bool* firstIter, Tile* mainTile, bool* wasMergedCurr, Tile* secondTile, bool* wasMergedSecond);
void singleTileMove(bool* moved, bool* firstIter, Tile* mainTile, bool* wasMergedCurr); // overload for last rows

Board::Board(const int rows, const int columns)
    : std::vector<std::vector<Tile>>(rows, std::vector<Tile>(columns)),
      m_rows(rows),
      m_columns(columns) {}

#if 1
Board::moveReturn Board::Move(Board::moveDirection direction) {
    switch (direction) {
        case Board::moveDirection::Up: {
        }

        case Board::moveDirection::Down: {
            for (int col = 0; col < this->m_columns; col++) {
                int waitingFor = 0;
                Tile* moveToOnWaiting;
                std::queue<Tile*> freeTiles;
                for (int row = this->m_rows - 1; row >= 0; row--) {
                    if (this->get(row, col).value == 0) {
                        freeTiles.push(&get(row, col));
                        continue;
                    }

                    if (this->get(row, col).value == waitingFor) {
                        waitingFor = 0;
                        moveToOnWaiting->setValue(moveToOnWaiting->value * 2);
                        this->get(row, col).setValue(0);
                        freeTiles.push(&get(row, col));
                        continue;
                    }

                    waitingFor = this->get(row, col).value;

                    if (!freeTiles.empty()) {
                        freeTiles.front()->setValue(this->get(row, col).value);
                        moveToOnWaiting = freeTiles.front();
                        freeTiles.pop();
                        this->get(row, col).setValue(0);
                        continue;
                    }

                    moveToOnWaiting = &this->get(row, col);
                }
            }
            refresh();
        }

        case Board::moveDirection::Left: {
        }

        case Board::moveDirection::Right: {
        }
    }
    return Board::moveReturn::NoneMoved;
}
#else
Board::moveReturn Board::Move(Board::moveDirection direction) { // old algoritmh
    switch (direction) {
        case Board::moveDirection::Up: {
            bool anyMoved = false;
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[m_rows]());
            // TODO: rename coll to col
            for (int coll = 0; coll < m_columns; coll++) {
                std::fill(alreadyMergedTiles.get(), alreadyMergedTiles.get() + m_rows, 0);
                for (int stage = 0; stage < m_rows; stage++) {
                    bool firstIter = true;
                    for (int row = 0; row < m_rows; row++) {
                        if (row < m_rows - 1) {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->get(row, coll),
                                           &alreadyMergedTiles[row],
                                           &this->get(row + 1, coll),
                                           &alreadyMergedTiles[row + 1]);
                        } else {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->get(row, coll),
                                           &alreadyMergedTiles[row]);
                        }
                    }
                }
            }
            return (anyMoved) ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
        }

        case Board::moveDirection::Down: {
            bool anyMoved = false;
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[m_rows]());

            for (int coll = 0; coll < m_columns; coll++) {
                std::fill(alreadyMergedTiles.get(), alreadyMergedTiles.get() + m_rows, 0);
                for (int stage = 0; stage < m_rows; stage++) {
                    bool firstIter = true;
                    for (int row = m_rows - 1; row >= 0; row--) {
                        if (row > 0) {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->get(row, coll),
                                           &alreadyMergedTiles[row],
                                           &this->get(row - 1, coll),
                                           &alreadyMergedTiles[row - 1]);
                        } else {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->get(row, coll),
                                           &alreadyMergedTiles[row]);
                        }
                    }
                }
            }
            return (anyMoved) ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
        }

        case Board::moveDirection::Left: {
            bool anyMoved = false;
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[m_columns]());

            for (int row = 0; row < m_rows; row++) {
                std::fill(alreadyMergedTiles.get(), alreadyMergedTiles.get() + m_columns, 0);
                for (int stage = 0; stage < m_columns; stage++) {
                    bool firstIter = true;
                    for (int coll = 0; coll < m_columns; coll++) {
                        if (coll < m_columns - 1) {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->get(row, coll),
                                           &alreadyMergedTiles[coll],
                                           &this->get(row, coll + 1),
                                           &alreadyMergedTiles[coll + 1]);
                        } else {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->get(row, coll),
                                           &alreadyMergedTiles[coll]);
                        }
                    }
                }
            }
            return (anyMoved) ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
        }

        case Board::moveDirection::Right: {
            bool anyMoved = false;
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[m_columns]());

            for (int row = 0; row < m_rows; row++) {
                std::fill(alreadyMergedTiles.get(), alreadyMergedTiles.get() + m_columns, 0);
                for (int stage = 0; stage < m_columns; stage++) {
                    bool firstIter = true;
                    for (int coll = m_columns - 1; coll >= 0; coll--) {
                        if (coll > 0) {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->get(row, coll),
                                           &alreadyMergedTiles[coll],
                                           &this->get(row, coll - 1),
                                           &alreadyMergedTiles[coll - 1]);
                        } else {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->get(row, coll),
                                           &alreadyMergedTiles[coll]);
                        }
                    }
                }
            }
            return (anyMoved) ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
        }
    }
    return Board::moveReturn::NoneMoved;
}

#endif

Tile& Board::get(const int row, const int column) {
    return this->at(row).at(column);
}

void Board::redrawAll() {
    for (auto& row : *this) {
        for (Tile& tile : row) {
            wbkgd(tile.window, 0); // this is workaround to recolor window, because if i set window to the same color, it doesn't recolor,
            tile.drawLabel();      // so i am setting it to 0 and then to its color again.
        }
    }
}

void Board::populateWithRandomTwos() {
    const int maxTilesToPopulate = m_rows * m_columns - 2; // num of board tiles - 2
    const int minTilesToPopulate = 2;                      // adleast 2 tiles will have 2 in them
    const int tilesToPopulate = randomNumber(minTilesToPopulate, maxTilesToPopulate);
    for (int i = 0; i < tilesToPopulate; i++) {
        this->at(randomNumber(0, m_rows - 1)).at(randomNumber(0, m_columns - 1)).setValue(2);
    }
}

// normal
void singleTileMove(bool* moved, bool* firstIter, Tile* mainTile, bool* wasMergedCurr, Tile* secondTile, bool* wasMergedSecond) {
    if ((mainTile->value == 0 || !(*firstIter))) { // moving
        *firstIter = false;
        mainTile->setValue(secondTile->value);
        if (mainTile->value > 0) {
            *moved = true;
        }
    } else if (mainTile->value == secondTile->value && (!(*wasMergedCurr)) && (!(*wasMergedSecond))) { // merging
        *firstIter = false;
        mainTile->setValue(mainTile->value * 2);
        *wasMergedCurr = true;
        *moved = true;
    }
}

// last row/collumn
void singleTileMove(bool* moved, bool* firstIter, Tile* mainTile, bool* wasMergedCurr) {
    if (mainTile->value == 0 || !(*firstIter)) {
        *firstIter = false;
        mainTile->setValue(0);
    }
}
