#ifndef APP_H
#define APP_H

struct App {
    int playRows;
    int playCollumns;
    int tileHeigth;
    int tileWidth;
    bool useColor;

    const int boardStartingRow = 2;             // y, starting row on the screen
    const int boardStartingCollumn = 1;         // x, starting collumn on the screen
    const unsigned char startupProbability = 6; // pobability of new "2" appering on starup
    const unsigned char moveProbability = 12;   // and every move
};

#endif // APP_H
