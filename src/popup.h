#ifndef POPUP_H
#define POPUP_H

#include <ncurses.h>
#include <string>

class Popup 
{
public:
    Popup(int heigth, int width);
    ~Popup();
    
    void setText(std::string);
    std::string getText();
private:
    WINDOW* m_win;
    std::string m_text;
}

#endif // POPUP_H
