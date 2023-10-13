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

Board::moveReturn Board::Move(Board::moveDirection direction) {
    switch (direction) {
        case Board::moveDirection::Up: {
            for (int col = 0; col < this->m_columns; col++) {
                Board::SingleTileMoveData singleTileMoveData;
                for (int row = 0; row < this->m_rows; row++) {
                    this->singleTileMove_newAlg(&singleTileMoveData, this->get(row, col));
                }
            }
            refresh();
            break;
        }

        case Board::moveDirection::Down: {
            for (int col = 0; col < this->m_columns; col++) {
                Board::SingleTileMoveData singleTileMoveData;
                for (int row = this->m_rows - 1; row >= 0; row--) {
                    this->singleTileMove_newAlg(&singleTileMoveData, this->get(row, col));
                }
            }
            refresh();
            break;
        }

        case Board::moveDirection::Left: {
            for (int row = 0; row < this->m_rows; row++) {
                Board::SingleTileMoveData singleTileMoveData;
                for (int col = 0; col < this->m_rows; col++) {
                    this->singleTileMove_newAlg(&singleTileMoveData, this->get(row, col));
                }
            }
            refresh();
            break;
        }

        case Board::moveDirection::Right: {
            for (int row = 0; row < this->m_rows; row++) {
                Board::SingleTileMoveData singleTileMoveData;
                for (int col = this->m_columns - 1; col >= 0; col--) {
                    this->singleTileMove_newAlg(&singleTileMoveData, this->get(row, col));
                }
            }
            refresh();
            break;
        }
    }
    return Board::moveReturn::Ok;
}

void Board::singleTileMove_newAlg(Board::SingleTileMoveData* data, Tile& currentTile) {
    if (currentTile.value == 0) {
        data->freeTiles.push(&currentTile);
        return;
    }

    if (currentTile.value == data->waitingFor) {
        data->waitingFor = 0;
        data->waiter->setValue(data->waiter->value * 2);
        currentTile.setValue(0);
        data->freeTiles.push(&currentTile);
        return;
    }

    data->waitingFor = currentTile.value;
    if (!data->freeTiles.empty()) {
        data->freeTiles.front()->setValue(currentTile.value);
        data->waiter = data->freeTiles.front();
        data->freeTiles.pop();
        currentTile.setValue(0);
        return;
    }
    data->waiter = &currentTile;
}

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
