#ifndef MAINSCREEN_H
#define MAINSCREEN_H
#include "Global.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;

struct Help : DialogMenu {
    Label headerClassic, contentClassic;
    Label headerExtreme, contentExtreme;
    Label headerControl, contentMove, contentSelect, contentPause;

    void selectOption() override;
    void createContents();
    void draw();

    sf::Sprite moveKey, enter, escape;

    Help() {
        createPanel(1400, 940, WINDOW.WIDTH / 2.0 + 15, WINDOW.HEIGHT / 2.0, TEXTURE.HelpPanel);
        createButtons(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0 + 350, 0, { "CLOSE" });
        createContents();
    }
};

struct About : DialogMenu {
    Label headerGroup, contentGroup;
    Label headerInstructor, contentInstructor;
    Label headerResources, contentResources;
    Label headerContact, contentContact;

    void selectOption() override;
    void createContents();
    void draw();

    About() {
        createPanel(1400, 940, WINDOW.WIDTH / 2.0 + 15, WINDOW.HEIGHT / 2.0, TEXTURE.AboutPanel);
        createButtons(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0 + 350, 0, { "CLOSE" });
        createContents();
    }
};


struct MainScreen : Menu{
    void selectOption() override;
    void start();

    MainScreen() {
        createTitle(780, 325, WINDOW.WIDTH / 2.0, 180, TEXTURE.Title);
        createMenu(WINDOW.WIDTH / 2.0, 430, 110, { "NEW GAME", "LOAD GAME", "SETTINGS", "HELP", "ABOUT", "EXIT" });
    }
};

#endif
