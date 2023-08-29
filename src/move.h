#ifndef MOVE_H
#define MOVE_H
#include "tile.h"
#include <vector>

namespace Move {
enum Direction { Left,
                 Right,
                 Up,
                 Down };
enum Return { Ok,
              NoneMoved,
              GameLost };

Return Move(std::vector<std::vector<Tile>>* matrix, Direction direction);

} // namespace Move
#endif // MOVE_H