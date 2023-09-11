#include <curses.h>
#include <string>

using namespace std;

class MainMenu
{
public:
    MainMenu();
    ~MainMenu();
    void setPlayerName(string name);
    void setLocation(string location);
private:
    WINDOW* win;
};