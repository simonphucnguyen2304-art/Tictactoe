#ifndef MENU_H
#define MENU_H
#include "Global.h"
#include "UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>


struct Menu {
    int curChoice;
    int numOptions;
    const float buttonWidth = 380;
    const float buttonHeight = 95;
    vector<Button> buttons;
    vector<Label> labels;
    sf::RectangleShape panel, title;

    // FUNCTION
    bool isActive;
    bool isOpen;
    void createTitle(float, float, float, float, sf::Texture&);
    void createPanel(float, float, float, float, sf::Texture&);
    void createMenu(float, float, float, vector<string>);
    void EventController();
    virtual void selectOption();
    void moveCursor(int);
    void drawMenu();
    void drawPopUpMenu();
    void closeMenu();
    void openMenu();
    void setActive(bool);
};

struct DialogMenu {
    pair<int, int> curChoice;
    int numTextBoxes, numButtons;
    const float buttonWidth = 150;
    const float buttonHeight = 75;
    vector<Button> buttons;
    vector<Label> labels;
    sf::RectangleShape panel, title;
    vector<TextBox> textboxes;
    sf::Text message;
    bool inButtonRow;

    // FUNCTION
    bool isActive;
    bool isOpen;
    void createTitle(float, float, float, float, sf::Texture&);
    void createPanel(float, float, float, float, sf::Texture&);
    void createButtons(float, float, float, vector<string>);
    void createTextBoxes(float, float, float, vector<pair<string, string>>, int);
    void createMessage(float, float, string);
    virtual void EventController();
    virtual void selectOption();
    virtual void moveCursor(int, int);
    virtual void resetData();
    void drawMenu();
    void drawPopUpMenu();
    void closeMenu();
    void openMenu();
    void setActive(bool);
};



#endif