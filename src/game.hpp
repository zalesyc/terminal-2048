#ifndef APP_H
#define APP_H

#include "board.hpp"

struct App { // defalut options, non const variables may be overwritten
    int playRows = 4;
    int playCollumns = 4;
    int tileHeigth = 3;
    int tileWidth = 7;
    bool useColor = false;

    const int boardStartingRow = 2;     // y, starting row on the screen
    const int boardStartingCollumn = 1; // x, starting collumn on the screen
    const char moveProbability = 12;    // and every move
};

enum welcomeScreenReturn { Play,
                           Exit };

void colors();
void boardInit(Board* board, App* appConfig);
welcomeScreenReturn welcomeScreen(App* appConfig);
int randomNumber(const int min, const int max);
bool gameLost(Board* board);

#endif // APP_H
