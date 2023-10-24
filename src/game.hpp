#ifndef APP_H
#define APP_H

#include "board.hpp"

struct App { // defalut options, non const variables may be overwritten
    int playRows = 4;
    int playColumns = 4;
    int tileHeigth = 3;
    int tileWidth = 7;
    bool useColor = false;

    const int boardStartingRow = 2;    // y, starting row on the screen
    const int boardStartingColumn = 1; // x, starting column on the screen
    const char moveProbability = 12;   // and every move
};

enum welcomeScreenReturn { Play,
                           Exit };

void colors();
void boardInit(Board* board);
welcomeScreenReturn welcomeScreen();
int randomNumber(int min, int max);
bool gameLost(Board* board);

extern App appConfig;

#endif // APP_H
