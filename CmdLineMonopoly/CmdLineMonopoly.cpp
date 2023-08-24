// CmdLineMonopoly.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
using namespace std;

/*
| Free Parking | Kentucky | Chance | Indiana Avenue | Illinois Avenue | B&O | Atlantic | Ventnor | Water Works | Marvin Gardens |
*/

class Property {
public:
    short price;
    short mortgagePrice;
    string name;
    short housePrice;
    unsigned char ownedBy = 255;
    unsigned char numHouses = 0; // 5 houses equals 1 hotel
};

class Player {
private:
    int cash = 0;
    unsigned char position = 0;
};

class GameBoard {
public:
    GameBoard() : selectedX(0), selectedY(0) {
        
    }
    void draw() {
        printf("\033c");
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (selectedX == i && selectedY == j) {
                    cout << "*";
                }
                else {
                    cout << "o";
                }
            }
            cout << "\n";
        }
    }
    void moveX(unsigned char moveBy) {
        selectedX += moveBy;
    }
private:
    unsigned char selectedX = 0;
    unsigned char selectedY = 0;
};

void draw_options() {

}

int main()
{
    GameBoard board;
    board.draw();
    cout << "What do you want to do? [1]: Move Right\n";
    int action;
    cin >> action;
    if (action == 1) {
        board.moveX(1);
    }
    board.draw();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
