#include <queue>
#include <vector>

#include "board.hpp"
#include "game.hpp"
#include "tile.hpp"

Board::Board(const int rows, const int columns)
    : std::vector<std::vector<Tile>>(rows, std::vector<Tile>(columns)),
      m_rows(rows),
      m_columns(columns) {}

Board::moveReturn Board::moveLeft() {
    bool wasMoved = false;
    for (int row = 0; row < this->m_rows; row++) {
        Board::SingleTileMoveData singleTileMoveData;
        for (int col = 0; col < this->m_rows; col++) {
            Board::singleTileMove(&singleTileMoveData, this->get(row, col), &wasMoved);
        }
    }
    refresh();
    return wasMoved ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
}

Board::moveReturn Board::moveRight() {
    bool wasMoved = false;
    for (int row = 0; row < this->m_rows; row++) {
        Board::SingleTileMoveData singleTileMoveData;
        for (int col = this->m_columns - 1; col >= 0; col--) {
            Board::singleTileMove(&singleTileMoveData, this->get(row, col), &wasMoved);
        }
    }
    refresh();
    return wasMoved ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
}

Board::moveReturn Board::moveUp() {
    bool wasMoved = false;
    for (int col = 0; col < this->m_columns; col++) {
        Board::SingleTileMoveData singleTileMoveData;
        for (int row = 0; row < this->m_rows; row++) {
            Board::singleTileMove(&singleTileMoveData, this->get(row, col), &wasMoved);
        }
    }
    refresh();
    return wasMoved ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
}

Board::moveReturn Board::moveDown() {
    bool wasMoved = false;
    for (int col = 0; col < this->m_columns; col++) {
        Board::SingleTileMoveData singleTileMoveData;
        for (int row = this->m_rows - 1; row >= 0; row--) {
            Board::singleTileMove(&singleTileMoveData, this->get(row, col), &wasMoved);
        }
    }
    refresh();
    return wasMoved ? Board::moveReturn::Ok : Board::moveReturn::NoneMoved;
}

void Board::singleTileMove(Board::SingleTileMoveData* data, Tile& currentTile, bool* wasMoved) {
    if (currentTile.value == 0) {
        data->freeTiles.push(&currentTile);
        return;
    }

    if (currentTile.value == data->waitingFor) {
        data->waitingFor = 0;
        data->waiter->setValue(data->waiter->value * 2);
        currentTile.setValue(0);
        data->freeTiles.push(&currentTile);
        *wasMoved = true;
        return;
    }

    data->waitingFor = currentTile.value;
    if (!data->freeTiles.empty()) {
        data->freeTiles.front()->setValue(currentTile.value);
        data->waiter = data->freeTiles.front();
        data->freeTiles.pop();
        data->freeTiles.push(&currentTile);
        currentTile.setValue(0);
        *wasMoved = true;
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
