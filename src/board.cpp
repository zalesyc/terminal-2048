#include "board.h"
#include "tile.h"
#include <algorithm>
#include <memory>
#include <vector>

void singleTileMove(bool* moved, bool* firstIter, Tile* mainTile, bool* wasMergedCurr, Tile* secondTile, bool* wasMergedSecond);
void singleTileMove(bool* moved, bool* firstIter, Tile* mainTile, bool* wasMergedCurr); // overload for last rows

Board::moveReturn Board::Move(Board::moveDirection direction) {
    switch (direction) {
        case Board::moveDirection::Up: {
            const int maxColl = this->at(0).size();
            const int maxRow = this->size();
            bool anyMoved = false;
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[maxRow]());

            for (int coll = 0; coll < maxColl; coll++) {
                std::fill(alreadyMergedTiles.get(), alreadyMergedTiles.get() + maxRow, 0);
                for (int stage = 0; stage < maxRow; stage++) {
                    bool firstIter = true;
                    for (int row = 0; row < maxRow; row++) {
                        if (row < maxRow - 1) {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->at(row).at(coll),
                                           &alreadyMergedTiles[row],
                                           &this->at(row + 1).at(coll),
                                           &alreadyMergedTiles[row + 1]);
                        } else {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->at(row).at(coll),
                                           &alreadyMergedTiles[row]);
                        }
                    }
                }
            }
            return (anyMoved) ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
        }

        case Board::moveDirection::Down: {
            const int maxColl = this->at(0).size();
            const int maxRow = this->size();
            bool anyMoved = false;
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[maxRow]());

            for (int coll = 0; coll < maxColl; coll++) {
                std::fill(alreadyMergedTiles.get(), alreadyMergedTiles.get() + maxRow, 0);
                for (int stage = 0; stage < maxRow; stage++) {
                    bool firstIter = true;
                    for (int row = maxRow - 1; row >= 0; row--) {
                        if (row > 0) {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->at(row).at(coll),
                                           &alreadyMergedTiles[row],
                                           &this->at(row - 1).at(coll),
                                           &alreadyMergedTiles[row - 1]);
                        } else {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->at(row).at(coll),
                                           &alreadyMergedTiles[row]);
                        }
                    }
                }
            }
            return (anyMoved) ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
        }

        case Board::moveDirection::Left: {
            const int maxColl = this->at(0).size();
            const int maxRow = this->size();
            bool anyMoved = false;
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[maxColl]());

            for (int row = 0; row < maxRow; row++) {
                std::fill(alreadyMergedTiles.get(), alreadyMergedTiles.get() + maxColl, 0);
                for (int stage = 0; stage < maxColl; stage++) {
                    bool firstIter = true;
                    for (int coll = 0; coll < maxColl; coll++) {
                        if (coll < maxColl - 1) {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->at(row).at(coll),
                                           &alreadyMergedTiles[coll],
                                           &this->at(row).at(coll + 1),
                                           &alreadyMergedTiles[coll + 1]);
                        } else {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->at(row).at(coll),
                                           &alreadyMergedTiles[coll]);
                        }
                    }
                }
            }
            return (anyMoved) ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
        }

        case Board::moveDirection::Right: {
            const int maxColl = this->at(0).size();
            const int maxRow = this->size();
            bool anyMoved = false;
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[maxColl]());

            for (int row = 0; row < maxRow; row++) {
                std::fill(alreadyMergedTiles.get(), alreadyMergedTiles.get() + maxColl, 0);
                for (int stage = 0; stage < maxColl; stage++) {
                    bool firstIter = true;
                    for (int coll = maxColl - 1; coll >= 0; coll--) {
                        if (coll > 0) {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->at(row).at(coll),
                                           &alreadyMergedTiles[coll],
                                           &this->at(row).at(coll - 1),
                                           &alreadyMergedTiles[coll - 1]);
                        } else {
                            singleTileMove(&anyMoved,
                                           &firstIter, &this->at(row).at(coll),
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