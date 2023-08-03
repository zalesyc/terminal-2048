#include "tile.h"
#include <array>
#include <random>

void checkAndAdd(Tile *first, Tile *second);
void singleMove(bool *firstIter, Tile *mainTile, bool *wasMergedCurr,
                Tile *secondTile, bool *wasMergedSecond);
void singleMove(bool *firstIter, Tile *mainTile,
                bool *wasMergedCurr); // overload for last rows

template <size_t N, size_t M>
Move::Return Move::Move(std::array<std::array<Tile, M>, N> *matrix,
                        Move::Direction direction) {
    switch (direction) {
    case Move::Direction::Up: {
        const int maxColl = matrix->at(0).size();
        const int maxRow = matrix->size();
        bool alreadyMergedTiles[maxRow][maxColl] = {};

        for (int stage = 0; stage < maxRow; stage++) {
            for (int coll = 0; coll < maxColl; coll++) {
                bool firstIter = true;
                for (int row = 0; row < maxRow; row++) {
                    if (row < maxRow - 1) {
                        singleMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[row][coll],
                                   &matrix->at(row + 1).at(coll),
                                   &alreadyMergedTiles[row + 1][coll]);
                    } else {
                        singleMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[row][coll]);
                    }
                }
            }
        }
        break;
    }

    case Move::Direction::Down: {
        const int maxColl = matrix->at(0).size();
        const int maxRow = matrix->size();
        bool alreadyMergedTiles[maxRow][maxColl] = {};

        for (int i = 0; i < maxRow; i++) {
            for (int coll = 0; coll < maxColl; coll++) {
                bool firstIter = true;
                for (int row = maxRow - 1; row >= 0; row--) {
                    if (row > 0) {
                        singleMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[row][coll],
                                   &matrix->at(row - 1).at(coll),
                                   &alreadyMergedTiles[row - 1][coll]);
                    } else {
                        singleMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[row][coll]);
                    }
                }
            }
        }
        break;
    }

    case Move::Direction::Left: {
        const int maxColl = matrix->at(0).size();
        const int maxRow = matrix->size();
        bool alreadyMergedTiles[maxRow][maxColl] = {};

        for (int stage = 0; stage < maxColl; stage++) {
            for (int row = 0; row < maxRow; row++) {
                bool firstIter = true;
                for (int coll = 0; coll < maxColl; coll++) {
                    if (coll < maxColl - 1) {
                        singleMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[row][coll],
                                   &matrix->at(row).at(coll + 1),
                                   &alreadyMergedTiles[row][coll + 1]);
                    } else {
                        singleMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[row][coll]);
                    }
                }
            }
        }
        break;
    }

    case Move::Direction::Right: {
        const int maxColl = matrix->at(0).size();
        const int maxRow = matrix->size();
        bool alreadyMergedTiles[maxRow][maxColl] = {};

        for (int stage = 0; stage < maxColl; stage++) {
            for (int row = 0; row < maxRow; row++) {
                bool firstIter = true;
                for (int coll = maxColl - 1; coll >= 0; coll--) {
                    if (coll > 0) {
                        singleMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[row][coll],
                                   &matrix->at(row).at(coll - 1),
                                   &alreadyMergedTiles[row][coll - 1]);
                    } else {
                        singleMove(&firstIter, &matrix->at(row).at(coll),
                                   &alreadyMergedTiles[row][coll]);
                    }
                }
            }
        }
        break;
    }
    }
    return Move::Return::Ok;
}

void checkAndAdd(Tile *first, Tile *second) {
    if ((second->value == 0) && (first->value != 0)) {
        second->setValue(first->value);
        first->setValue(0);
    } else if ((first->value == second->value) && first->value != 0) {
        first->setValue(0);
        second->setValue(second->value * 2);
    }
}

// normal
void singleMove(bool *firstIter, Tile *mainTile, bool *wasMergedCurr,
                Tile *secondTile, bool *wasMergedSecond) {
    if ((mainTile->value == 0 || !(*firstIter))) {
        *firstIter = false;
        mainTile->setValue(secondTile->value);
    } else if (mainTile->value == secondTile->value && (!(*wasMergedCurr)) &&
               (!(*wasMergedSecond))) {
        *firstIter = false;
        mainTile->setValue(mainTile->value * 2);
        *wasMergedCurr = true;
    }
}

// last row/collumn
void singleMove(bool *firstIter, Tile *mainTile, bool *wasMergedCurr) {
    if (mainTile->value == 0 || !(*firstIter)) {
        *firstIter = false;
        mainTile->setValue(0);
    }
}

void Move::addRandTwos(Tile *tile, unsigned char probability) {
    // probapility is the chance of showing new 2 on every move
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