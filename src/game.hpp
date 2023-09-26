#ifndef APP_H
#define APP_H

#include "tile.hpp"
#include <vector>

struct App { // defalut options, non const variables may be overwritten
    int playRows = 4;
    int playCollumns = 4;
    int tileHeigth = 3;
    int tileWidth = 7;
    bool useColor = false;

    const int boardStartingRow = 2;             // y, starting row on the screen
    const int boardStartingCollumn = 1;         // x, starting collumn on the screen
    const unsigned char startupProbability = 6; // pobability of new "2" appering on starup
    const unsigned char moveProbability = 12;   // and every move
};
enum welcomeScreenReturn {Play, Exit};

void colors();
void boardInit(std::vector<std::vector<Tile>>* board, App* appConfig);
welcomeScreenReturn welcomeScreen(App* appConfig);

#endif // APP_H
