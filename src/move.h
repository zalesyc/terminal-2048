#ifndef MOVE_H
#define MOVE_H
#include "tile.h"
#include <vector>

namespace Move {
enum Direction { Left,
                 Right,
                 Up,
                 Down };
enum Return { Ok = 0,
              GameLost = 1 };
void addRandTwos(Tile* tile, unsigned char probability);

Return Move(std::vector<std::vector<Tile>>* matrix, Direction direction);

} // namespace Move
#endif // MOVE_H