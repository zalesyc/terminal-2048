#ifndef POPUP_H
#define POPUP_H

#include <ncurses/curses.h>
#include <string>
#include <vector>

class Popup {
  public:
    Popup(int row, int column, int height, int width);
    ~Popup();

    void setText(const std::string& text);
    void setTitle(const std::string& text) const;
    void delWindow() const;

  private:
    std::string print(int width);
    void drawWindow(int row, int column, int height, int width);

  public:
    WINDOW* m_win;
    int m_winWidth;
    int m_winHeight;

  private:
    std::string m_text;
};

namespace SelectMenu {
struct Option {
    const std::string name;
    const bool hasValue;
    const int* value;
};

int verticalMenu(WINDOW* win, int row, int column, const std::vector<Option>& options, int highlightedOption);
int verticalMenu(WINDOW* win, int row, int column, const std::vector<std::string>& options, int highlightedOption);
int horizontalMenu(WINDOW* win, int row, int column, const std::vector<std::string>& options, int highlightedOption);
} // namespace SelectMenu

#endif // POPUP_H
