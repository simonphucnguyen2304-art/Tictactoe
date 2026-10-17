#include "Global.h"
#include "UI.h"
#include "Setting.h"
#include "Sound.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>


void Setting::createPanel(float w, float h, float x, float y, sf::Texture& texture) {
    panel.setSize(sf::Vector2f(w, h));
    panel.setOrigin(w / 2.0, h / 2.0);
    panel.setPosition(x, y);
    panel.setTexture(&texture);
}

void Setting::createMenu(float startX, float startY, float spacing, vector<string> texts) {
    buttons.resize(4, Button());
    labels.resize(4, Label());
    icons.resize(3, vector(2, Elements()));
    states.resize(2);
    sliders.resize(2, Slider());
    sf::FloatRect bounds;


    // VOLUME SETTING
    for (int i = 0; i < 2; i++) { 
        buttons[i] = Button(buttonWidth, buttonHeight, TEXTURE.MiniButton, TEXTURE.MiniButtonHovered);
        buttons[i].setPosition(startX + 180, startY + i * spacing);

        labels[i].setString(texts[i]);
        bounds = labels[i].getGlobalBounds();
        labels[i].setOrigin(bounds.left + bounds.width / 2.0, bounds.top + bounds.height / 2.0);
        labels[i].setPosition(startX - 240 + bounds.width / 2.0, startY + i * spacing);

        icons[i][1] = Elements(60, 60, TEXTURE.SoundOn, TEXTURE.SoundOnHovered);
        icons[i][1].setPosition(buttons[i].getPosition());

        icons[i][0] = Elements(60, 60, TEXTURE.SoundOff, TEXTURE.SoundOffHovered);
        icons[i][0].setPosition(buttons[i].getPosition());

        states[i] = 1;

        sliders[i].create(startX, startY + i * spacing + 90);
    }

    // THEME SETTING
    labels[2].setString(texts[2]);
    bounds = labels[2].getGlobalBounds();
    labels[2].setOrigin(bounds.left + bounds.width / 2.0, bounds.top + bounds.height / 2.0);
    labels[2].setPosition(startX - 240 + bounds.width / 2.0, startY + 2 * spacing);

    icons[2][0] = Elements(35, 35, TEXTURE.Color0Dot, TEXTURE.Color0DotHovered);
    icons[2][0].setPosition(startX - 30, startY + 2 * spacing);

    icons[2][1] = Elements(35, 35, TEXTURE.Color1Dot, TEXTURE.Color1DotHovered);
    icons[2][1].setPosition(startX + 30, startY + 2 * spacing);


    // BACK BUTTON
    buttons[3] = Button(buttonWidth, buttonHeight, TEXTURE.MiniButton, TEXTURE.MiniButtonHovered);
    buttons[3].setPosition(startX, startY + 3 * spacing - 50);

    labels[3].setString(texts[3]);
    bounds = labels[3].getGlobalBounds();
    labels[3].setOrigin(bounds.left + bounds.width / 2.0, bounds.top + bounds.height / 2.0);
    labels[3].setPosition(buttons[3].getPosition());
}

void Setting::moveCursor(int x) {
    SOUND.sfxMenuMove.play();
    curChoice = (curChoice + x + numOptions) % numOptions;
}

void Setting::switchState() {
    states[curChoice / 2] ^= 1;
    if (states[curChoice / 2]) sliders[curChoice / 2].update(40);
    else sliders[curChoice / 2].update(0);
    if (curChoice == 0) SOUND.setMusicVolume(sliders[0].value);
    else SOUND.setSoundVolume(sliders[1].value);
}

void Setting::selectOption() {
    SOUND.sfxMenuSelect.play();
    if (curChoice == 5) close();
    else switchState();
}

void Setting::EventController() {
    sf::Event event;
    while (WINDOW.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            WINDOW.close();

        if (event.type == sf::Event::KeyPressed) {
            // MOVE
            if (event.key.code == sf::Keyboard::W) moveCursor(-1); // UP
            if (event.key.code == sf::Keyboard::S) moveCursor(1); // DOWN
            if (curChoice == 1 || curChoice == 3 || curChoice == 4) { // REDUCE VOLUME
                if (event.key.code == sf::Keyboard::A) {
                    if (curChoice & 1) {
                        sliders[curChoice / 2].update(sliders[curChoice / 2].value - 10);
                        SOUND.sfxOof.setVolume(sliders[curChoice / 2].value);
                        SOUND.sfxOof.play();

                        if (sliders[curChoice / 2].value == 0) states[curChoice / 2] = 0;
                        else states[curChoice / 2] = 1;

                        if (curChoice == 1) SOUND.setMusicVolume(sliders[0].value);
                        else SOUND.setSoundVolume(sliders[1].value);
                    }
                    else {
                        SOUND.sfxMenuMove.play();
                        COLOR.switchMode();
                        TEXTURE.switchTheme();
                    }
                }

                if (event.key.code == sf::Keyboard::D) { // INCREASE VOLUME
                    if (curChoice & 1) {
                        sliders[curChoice / 2].update(sliders[curChoice / 2].value + 10);
                        SOUND.sfxOof.setVolume(sliders[curChoice / 2].value);
                        SOUND.sfxOof.play();

                        if (sliders[curChoice / 2].value == 0) states[curChoice / 2] = 0;
                        else states[curChoice / 2] = 1;

                        if (curChoice == 1) SOUND.setMusicVolume(sliders[0].value);
                        else SOUND.setSoundVolume(sliders[1].value);
                    }
                    else {
                        SOUND.sfxMenuMove.play();
                        COLOR.switchMode();
                        TEXTURE.switchTheme();
                    }
                }
            }
            else if (event.key.code == sf::Keyboard::Enter) selectOption(); // ENTER = SELECT
        }
    }
}

void Setting::drawMenu() {
    WINDOW.draw(panel);

    for (int i = 0; i < 6; ++i) {
        if (i < 4) {
            labels[i / 2].setFillColor(COLOR.PRIMARY);
            if (curChoice == i) { // Hover effect;
                if (i & 1) sliders[i / 2].setHoveredState();
                else {
                    buttons[i / 2].setHoveredState();
                    icons[i / 2][states[i / 2]].setHoveredState();
                }
            }
            else {
                if (i & 1) sliders[i / 2].setNormalState();
                else {
                    buttons[i / 2].setNormalState();
                    icons[i / 2][states[i / 2]].setNormalState();
                }
            }
        }
        else if (i == 4) {
            labels[i / 2].setFillColor(COLOR.PRIMARY);
            for (int mode = 0; mode < 2; mode++)
                if (curChoice == i && COLOR.curMode == mode) // Hover effect
                    icons[2][mode].setHoveredState();
                else
                    icons[2][mode].setNormalState();
        }
        else {
            if (curChoice == i) {
                buttons[3].setHoveredState();
                labels[3].setHoveredState();
            }
            else {
                buttons[3].setNormalState();
                labels[3].setNormalState();
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        WINDOW.draw(buttons[i]);
        WINDOW.draw(labels[i]);
        if (i < 2) {
            WINDOW.draw(icons[i][states[i]]);
            sliders[i].draw();
        }
        else if (i == 2) {
            WINDOW.draw(icons[i][0]);
            WINDOW.draw(icons[i][1]);
        }
    }
}

void Setting::drawPopUpMenu() {
    sf::RectangleShape overlay;
    overlay.setSize(sf::Vector2f(WINDOW.WIDTH, WINDOW.HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    WINDOW.draw(overlay);
    drawMenu();
}

void Setting::open() {
    curChoice = 0;
    isOpen = true;
}

void Setting::close() {
    isOpen = false;
}