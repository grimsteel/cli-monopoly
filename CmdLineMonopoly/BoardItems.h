#pragma once

#include <string>
#include <curses.h>

using namespace std;

// Each character is about 2x as tall as it is wide, so the height to width ratio is more like 16:11.
constexpr int V_PROPERTY_HEIGHT = 8;
constexpr int V_PROPERTY_WIDTH = 11;

constexpr int H_PROPERTY_WIDTH = 16;
constexpr int H_PROPERTY_HEIGHT = 5;

constexpr int PROPERTIES_PER_SIDE = 9;

constexpr char CHANCE_DISPLAY_V[] = "   Chance";
constexpr char COMMUNITY_CHEST_DISPLAY_V[] = " Community    Chest";
constexpr char CHANCE_DISPLAY_H[] = "     Chance";
constexpr char COMMUNITY_CHEST_DISPLAY_H[] = "   Community         Chest";
constexpr char INCOME_TAX_DISPLAY_V[] = "   Income      Tax";
constexpr char LUXURY_TAX_DISPLAY_H[] = "   Luxury Tax";

enum BoardItemLocation {
    Bottom,
    Top,
    Left,
    Right
};

class BoardItem {
public:
    BoardItem(int index, string name, BoardItemLocation location);
    ~BoardItem();
    void initWindow();
    string name;
    /// @brief Draw this entire item, using a specific display name
    /// @param displayName The specific display name (formatted) to use
    /// @warning If this is not implemented by a child class, it defaults to calling `drawInitial` without parameters
    virtual void drawInitial(string displayName) { drawInitial(); }
    /// @brief Same as drawInitial(string displayName) but try to infer a display name
    /// Pure virtual
    virtual void drawInitial() = 0;
    void redraw();
protected:
    WINDOW* win = nullptr;
    BoardItemLocation location;
    int index;
};

/// A single Property card on the map
class Property : public BoardItem {
public:
    Property(int index, string name, short price, unsigned char colorGroup, BoardItemLocation location);
    short price;
    short mortgagePrice;
    short housePrice;
    unsigned char ownedBy = 255; // 0-254 are player IDs, 255 is -1
    unsigned char numHouses = 0; // 5 houses equals 1 hotel
    void drawInitial(string displayName) override;
    void drawInitial() override { drawInitial(name); }
private:
    unsigned char colorGroup;
};

/// Chance or Community Chest
class RandomDraw : public BoardItem {
public:
    enum RandomDrawType {
        Chance,
        CommunityChest
    };
    RandomDraw(int index, RandomDrawType type, BoardItemLocation location);
    void drawInitial() override;
private:
    RandomDrawType type;
};

/// Luxury/Income tax
class TaxItem : public BoardItem {
public:
    enum TaxType {
        Income,
        Luxury
    };
    TaxItem(int index, TaxType type, BoardItemLocation location);
    void drawInitial() override;
private:
    TaxType type;
};
