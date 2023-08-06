#include "tile.h"
#include "move.h"
#include <vector>
#include <random>
#include <memory>

void singleTileMove(bool* firstIter, Tile* mainTile, bool* wasMergedCurr, Tile* secondTile, bool* wasMergedSecond);
void singleTileMove(bool* firstIter, Tile* mainTile, bool* wasMergedCurr); // overload for last rows

Move::Return Move::Move(std::vector<std::vector<Tile>>* matrix, Move::Direction direction) {
    switch (direction) {
    case Move::Direction::Up: {
        const int maxColl = matrix->at(0).size();
        const int maxRow = matrix->size();

        for (int coll = 0; coll < maxColl; coll++) {
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[maxRow]());
            for (int stage = 0; stage < maxRow; stage++) {
                bool firstIter = true;
                for (int row = 0; row < maxRow; row++) {
                    if (row < maxRow - 1) {
                        singleTileMove(&firstIter, &matrix->at(row).at(coll),
                                       &alreadyMergedTiles[row],
                                       &matrix->at(row + 1).at(coll),
                                       &alreadyMergedTiles[row + 1]);
                    } else {
                        singleTileMove(&firstIter, &matrix->at(row).at(coll),
                                       &alreadyMergedTiles[row]);
                    }
                }
            }
        }
        break;
    }

    case Move::Direction::Down: {
        const int maxColl = matrix->at(0).size();
        const int maxRow = matrix->size();
                
        for (int coll = 0; coll < maxColl; coll++) {
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[maxRow]());
            for (int stage = 0; stage < maxRow; stage++) {
                bool firstIter = true;
                for (int row = maxRow - 1; row >= 0; row--) {
                    if (row > 0) {
                        singleTileMove(&firstIter, &matrix->at(row).at(coll),
                                       &alreadyMergedTiles[row],
                                       &matrix->at(row  - 1).at(coll),
                                       &alreadyMergedTiles[row - 1]);
                    } else {
                        singleTileMove(&firstIter, &matrix->at(row).at(coll),
                                       &alreadyMergedTiles[row]);
                    }
                }
            }
        }
        break;
    }

    case Move::Direction::Left: {
        const int maxColl = matrix->at(0).size();
        const int maxRow = matrix->size();
                
        for (int row = 0; row < maxRow; row++) {
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[maxColl]());
            for (int stage = 0; stage < maxColl; stage++) {
                bool firstIter = true;
                for (int coll = 0; coll < maxColl; coll++) {
                    if (coll < maxColl - 1) {
                        singleTileMove(&firstIter, &matrix->at(row).at(coll),
                                       &alreadyMergedTiles[coll],
                                       &matrix->at(row).at(coll + 1),
                                       &alreadyMergedTiles[coll + 1]);
                    } else {
                        singleTileMove(&firstIter, &matrix->at(row).at(coll),
                                       &alreadyMergedTiles[coll]);
                    }
                }
            }
        }
        break;
    }

    case Move::Direction::Right: {
        const int maxColl = matrix->at(0).size();
        const int maxRow = matrix->size();

        for (int row = 0; row < maxRow; row++) {
            std::unique_ptr<bool[]> alreadyMergedTiles(new bool[maxColl]());
            for (int stage = 0; stage < maxColl; stage++) {
                bool firstIter = true;
                for (int coll = maxColl - 1; coll >= 0; coll--) {
                    if (coll > 0) {
                        singleTileMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[coll],
                                   &matrix->at(row).at(coll - 1),
                                   &alreadyMergedTiles[coll - 1]);
                    } else {
                        singleTileMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[coll]);
                    }
                }
            }
        }
        break;
    }
    }
    return Move::Return::Ok;
}


// normal
void singleTileMove(bool* firstIter, Tile* mainTile, bool* wasMergedCurr,
                Tile* secondTile, bool* wasMergedSecond) {
    if ((mainTile->value == 0 || !(*firstIter))) {
        *firstIter = false;
        mainTile->setValue(secondTile->value);
    } else if (mainTile->value == secondTile->value && (!(*wasMergedCurr)) && (!(*wasMergedSecond))) {
        *firstIter = false;
        mainTile->setValue(mainTile->value * 2);
        *wasMergedCurr = true;
    }
}

// last row/collumn
void singleTileMove(bool* firstIter, Tile* mainTile, bool* wasMergedCurr) {
    if (mainTile->value == 0 || !(*firstIter)) {
        *firstIter = false;
        mainTile->setValue(0);
    }
}

void Move::addRandTwos(Tile* tile, unsigned char probability) {
    // probability is the chance of showing new 2 on every move
    if (tile->value == 0) {
        std::random_device r;
        std::default_random_engine eng(r());
        std::uniform_int_distribution<int> uniform_dist(
            1, probability);
        if (uniform_dist(eng) == 1) {
            tile->setValue(2, true);
        }
    }
}