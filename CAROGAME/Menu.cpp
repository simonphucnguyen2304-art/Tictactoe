#include "Global.h"
#include "UI.h"
#include "Sound.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <iostream> 
#include <conio.h>
#include <string>
#include <vector>
using namespace std;

void Menu::createTitle(float w, float h, float x, float y, sf::Texture& titleTexture) {
    title.setSize(sf::Vector2f(w, h));
    title.setTexture(&titleTexture);
    title.setOrigin(w / 2.0, h / 2.0);
    title.setPosition(x, y);
}

void Menu::createPanel(float w, float h, float x, float y, sf::Texture& texture) {
    panel.setSize(sf::Vector2f(w, h));
    panel.setOrigin(w / 2.0, h / 2.0);
    panel.setPosition(x, y);
    panel.setTexture(&texture);
}


void Menu::createMenu(float startX, float startY, float spacing, vector<string> texts) {
    curChoice = 0;
    numOptions = texts.size();

    buttons.resize(numOptions, Button(buttonWidth, buttonHeight, TEXTURE.Button, TEXTURE.ButtonHovered));
    labels.resize(numOptions, Label());

    for (int i = 0; i < numOptions; ++i) {
        buttons[i].setPosition(startX, startY + i * spacing);

        labels[i].setString(texts[i]);
        sf::FloatRect bounds = labels[i].getGlobalBounds();
        labels[i].setOrigin(bounds.left + bounds.width / 2.0, bounds.top + bounds.height / 2.0);
        labels[i].setPosition(buttons[i].getPosition());
    }
}

void Menu::moveCursor(int x) {
    SOUND.sfxMenuMove.play();
    curChoice = (curChoice + x + numOptions) % numOptions;
}

void Menu::selectOption() {}

void Menu::EventController() {
    sf::Event event;
    while (WINDOW.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            WINDOW.close();

        if (event.type == sf::Event::KeyPressed) {
            // DO
            if (event.key.code == sf::Keyboard::Enter) selectOption(); // ENTER = SELECT
            // MOVE
            if (event.key.code == sf::Keyboard::W) moveCursor(-1); // UP
            if (event.key.code == sf::Keyboard::S) moveCursor(1); // DOWN
        }
    }
}

void Menu::drawMenu() {
    WINDOW.draw(title);
    WINDOW.draw(panel);

    for (int i = 0; i < numOptions; ++i) {
        if (curChoice == i) { // Hover effect;
            buttons[i].setHoveredState();
            labels[i].setHoveredState();
        }
        else {
            buttons[i].setNormalState();
            labels[i].setNormalState();
        }

        WINDOW.draw(buttons[i]);
        WINDOW.draw(labels[i]);
    }
}

void Menu::drawPopUpMenu() {
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(WINDOW.WIDTH, WINDOW.HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    WINDOW.draw(overlay);
    drawMenu();
}

void Menu::setActive(bool state) {
    isActive = state;
}

void Menu::closeMenu() {
    isOpen = isActive = false;
}

void Menu::openMenu() {
    curChoice = 0;
    isOpen = isActive = true;
}


void DialogMenu::createTitle(float w, float h, float x, float y, sf::Texture& titleTexture) {
    title.setSize(sf::Vector2f(w, h));
    title.setTexture(&titleTexture);
    title.setOrigin(w / 2.0, h / 2.0);
    title.setPosition(x, y);
}

void DialogMenu::createPanel(float w, float h, float x, float y, sf::Texture& texture) {
    panel.setSize(sf::Vector2f(w, h));
    panel.setOrigin(w / 2.0, h / 2.0);
    panel.setPosition(x, y);
    panel.setTexture(&texture);
}

void DialogMenu::createButtons(float startX, float startY, float spacing, vector<string> texts) {
    numButtons = texts.size();

    buttons.resize(numButtons, Button(buttonWidth, buttonHeight, TEXTURE.MiniButton, TEXTURE.MiniButtonHovered));
    labels.resize(numButtons, Label());

    for (int i = 0; i < numButtons; ++i) {
        buttons[i].setPosition(startX + i * spacing, startY);

        labels[i].setString(texts[i]);
        sf::FloatRect bounds = labels[i].getGlobalBounds();
        labels[i].setOrigin(bounds.left + bounds.width / 2.0, bounds.top + bounds.height / 2.0);
        labels[i].setPosition(buttons[i].getPosition());
    }
}

void DialogMenu::createTextBoxes(float startX, float startY, float spacing, vector<pair<string, string>> texts, int lim) {
    numTextBoxes = texts.size();

    textboxes.resize(numTextBoxes);
    for (int i = 0; i < numTextBoxes; i++)
        textboxes[i].createBox(startX, startY + i * spacing, texts[i].first, texts[i].second, lim);
}

void DialogMenu::createMessage(float x, float y, string text) {
    message.setString(text);
    message.setFont(FONT.Main);
    message.setFillColor(COLOR.PRIMARY);
    message.setCharacterSize(40);
    message.setStyle(sf::Text::Bold);
    sf::FloatRect bounds = message.getGlobalBounds();
    message.setOrigin(bounds.left + bounds.width / 2.0, bounds.top + bounds.height / 2.0);
    message.setPosition(x, y);
}

void DialogMenu::moveCursor(int x, int y) {
    SOUND.sfxMenuMove.play();
    curChoice.first = (curChoice.first + x + numTextBoxes+1) % (numTextBoxes+1);
    curChoice.second = (curChoice.second + y + numButtons) % numButtons;

    if (curChoice.first == numTextBoxes) inButtonRow = true;
    else inButtonRow = false;
}

void DialogMenu::selectOption() {}

void DialogMenu::EventController() {
    sf::Event event;
    while (WINDOW.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            WINDOW.close();

        if (event.type == sf::Event::KeyPressed) {
            // DO
            if (event.key.code == sf::Keyboard::Enter) selectOption(); // ENTER = SELECT
            // MOVE
            if (event.key.code == sf::Keyboard::W) moveCursor(-1, 0); // UP
            if (event.key.code == sf::Keyboard::S) moveCursor(1, 0); // DOWN
            if (event.key.code == sf::Keyboard::A) moveCursor(0, -1); // LEFT
            if (event.key.code == sf::Keyboard::D) moveCursor(0, 1); // RIGHT
        }
    }
}

void DialogMenu::drawMenu() {
    WINDOW.draw(title);
    WINDOW.draw(panel);
    message.setFillColor(COLOR.PRIMARY);
    WINDOW.draw(message);
    for (int i = 0; i < numTextBoxes; ++i) {
        if (curChoice.first == i) { // Hover effect;
            textboxes[i].setHoveredState();
        }
        else {
            textboxes[i].setNormalState();
        }
        textboxes[i].draw();
    }

    for (int i = 0; i < numButtons; ++i) {
        if (inButtonRow && curChoice.second == i) { // Hover effect;
            buttons[i].setHoveredState();
            labels[i].setHoveredState();
        }
        else {
            buttons[i].setNormalState();
            labels[i].setNormalState();
        }
        WINDOW.draw(buttons[i]);
        WINDOW.draw(labels[i]);
    }
}

void DialogMenu::drawPopUpMenu() {
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(WINDOW.WIDTH, WINDOW.HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    WINDOW.draw(overlay);
    drawMenu();
}

void DialogMenu::setActive(bool state) {
    isActive = state;
}

void DialogMenu::closeMenu() {
    isOpen = isActive = false;

}

void DialogMenu::resetData() {
    curChoice = { 0, 0 };
    if (numTextBoxes == 0) inButtonRow = true;
    else inButtonRow = false;
    for (int i = 0; i < numTextBoxes; i++) {
        textboxes[i].input = "";
        textboxes[i].cntChar = 0;
    }
}

void DialogMenu::openMenu() {
    resetData();
    isOpen = isActive = true;
}
