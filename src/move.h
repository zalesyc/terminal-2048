#ifndef MOVE_H
#define MOVE_H
#include "tile.h"
#include <array>

namespace Move {
enum Direction { Left,
                 Right,
                 Up,
                 Down };
enum Return { Ok = 0,
              GameLost = 1 };
void addRandTwos(Tile* tile, unsigned char probability);

template <size_t N, size_t M>
Return Move(const std::array<std::array<Tile, M>, N>* matrix, Direction direction);

} // namespace Move

#include "move.cpp" // i am including a cpp file, because when using function using temlplates the definition cannot be in a different file
// as per
// https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file

#endif // MOVE_H