#include <ncurses.h>

#include "game.h"
#include "random"
#include "tile.h"

void colors() {
    init_pair(2, -1, COLOR_WHITE);
    init_pair(4, -1, COLOR_YELLOW);
    init_pair(8, -1, COLOR_GREEN);
    init_pair(16, -1, COLOR_CYAN);
    init_pair(32, -1, COLOR_BLUE);
    init_pair(64, -1, COLOR_MAGENTA);
    init_pair(128, -1, COLOR_RED);
}

void addRandTwos(Tile* tile, unsigned char probability) {
    // probability is the chance of showing new 2 on every move
    if (tile->value == 0) {
        std::random_device r;
        std::default_random_engine eng(r());
        std::uniform_int_distribution<int> uniform_dist(1, probability);
        if (uniform_dist(eng) == 1) {
            tile->setValue(2, true);
        }
    }
}