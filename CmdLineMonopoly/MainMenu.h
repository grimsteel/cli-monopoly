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
    bool setYesNoPrompt(string prompt);
private:
    WINDOW* win;
};