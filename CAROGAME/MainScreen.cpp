#include "Global.h";
#include "UI.h";
#include "Sound.h"
#include "MainScreen.h"
#include "Game.h"
#include "Setting.h"
#include <SFML/Graphics.hpp>    
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;

void Help::createContents() {
    float centerX = WINDOW.WIDTH / 2.0;
    float startY = WINDOW.HEIGHT / 2.0 - 300 + 80;

    headerClassic = Label("CLASSIC MODE", 50, COLOR.SECONDARY);
    headerClassic.setPosition(centerX - 135 - 140, startY);

    contentClassic = Label(
        "Board: 15x15\n"
        "Turn: Players place one mark per turn in an empty cell\n"
        "Win: 5 marks in a row (horizontal, vertical, or diagonal)\n"
        "Draw: The board is full",
        37, COLOR.PRIMARY);

    sf::FloatRect bounds = contentClassic.getLocalBounds();
    contentClassic.setOrigin(bounds.width / 2.0, 0);
    contentClassic.setPosition(centerX - 140, startY + 40);


    headerExtreme = Label("EXTREME MODE", 50, COLOR.SECONDARY);
    headerExtreme.setPosition(centerX - 145 - 140, startY + 260);

    contentExtreme = Label(
        "Board: Big Board (9x9) with 9 Mini Boards\n"
        "Turn: One mark per turn in an unfinished Mini Board\n"
        "Win: 3 Mini Boards in a row on a Big Board\n"
        "Draw: No Big Board win when all Mini Boards are filled",
        37, COLOR.PRIMARY);
    bounds = contentExtreme.getLocalBounds();
    contentExtreme.setOrigin(bounds.width / 2.0, 0);
    contentExtreme.setPosition(centerX - 140, startY + 300);

    headerControl = Label("CONTROL", 50, COLOR.SECONDARY);
    headerControl.setPosition(centerX + 370, startY);


    contentMove = Label("MOVE", 30, COLOR.SECONDARY);
    bounds = contentMove.getLocalBounds();
    contentMove.setPosition(centerX + 330, startY + 60);

    moveKey.setTexture(TEXTURE.MoveKey);
    moveKey.setScale(0.8, 0.8);
    bounds = moveKey.getLocalBounds();
    moveKey.setPosition(centerX + 330, startY + 80);


    contentSelect = Label("SELECT / PLACE MARK", 30, COLOR.SECONDARY);
    bounds = contentSelect.getLocalBounds();
    contentSelect.setPosition(centerX + 330, startY + 275);

    enter.setTexture(TEXTURE.Enter);
    enter.setScale(0.8, 0.8);
    bounds = enter.getLocalBounds();
    enter.setPosition(centerX + 330, startY + 305);


    contentPause = Label("PAUSE", 30, COLOR.SECONDARY);
    bounds = contentPause.getLocalBounds();
    contentPause.setPosition(centerX + 330, startY + 420);

    escape.setTexture(TEXTURE.Escape);
    bounds = escape.getLocalBounds();
    escape.setPosition(centerX + 330, startY + 450);
}

void Help::selectOption() {
    closeMenu();
}

void Help::draw() {
    headerClassic.setFillColor(COLOR.SECONDARY);
    headerExtreme.setFillColor(COLOR.SECONDARY);
    headerControl.setFillColor(COLOR.SECONDARY);

    contentClassic.setFillColor(COLOR.PRIMARY);
    contentExtreme.setFillColor(COLOR.PRIMARY);
    contentMove.setFillColor(COLOR.SECONDARY);
    contentSelect.setFillColor(COLOR.SECONDARY);
    contentPause.setFillColor(COLOR.SECONDARY);

    drawPopUpMenu();
    WINDOW.draw(headerClassic);
    WINDOW.draw(contentClassic);
    WINDOW.draw(headerExtreme);
    WINDOW.draw(contentExtreme);
    WINDOW.draw(headerControl);
    WINDOW.draw(contentMove);
    WINDOW.draw(contentSelect);
    WINDOW.draw(contentPause);
    WINDOW.draw(moveKey);
    WINDOW.draw(enter);
    WINDOW.draw(escape);

}


void About::createContents() {
    // Tính toán toạ độ trung tâm và các cột
    float centerX = WINDOW.WIDTH / 2.0;
    float centerY = WINDOW.HEIGHT / 2.0;

    // Chia làm 2 cột: Cột trái và Cột phải cách tâm 350px
    float leftColX = centerX - 350;
    float rightColX = centerX + 350;

    // Dòng trên bắt đầu cao hơn tâm một chút
    float topRowY = centerY - 250;
    // Dòng dưới (Instructor/Resources) nằm thấp xuống
    float bottomRowY = centerY + 150;


    // --- CỘT TRÁI (LEFT COLUMN) ---

    // 1. GROUP INFO
    headerGroup = Label("GROUP 6 - 25C10", 50, COLOR.SECONDARY);
    headerGroup.setPosition(leftColX - 90, topRowY + 50);

    contentGroup = Label(
        "25127106 - Huynh Phuc Nguyen\n"
        "25127231 - Nguyen Tran Hoang Quan\n"
        "25127396 - Huynh Dinh Khuong\n"
        "25127400 - Vu Dinh Anh Kiet\n"
        "25127464 - Nguyen Kim Phuc",
        40, COLOR.PRIMARY); // Cỡ chữ 32 để vừa khung
    sf::FloatRect bounds = contentGroup.getLocalBounds();
    contentGroup.setOrigin(bounds.width / 2.0, 0); // Origin ở giữa-trên
    contentGroup.setPosition(leftColX + 75, topRowY + 100);

    // 2. INSTRUCTOR
    headerInstructor = Label("INSTRUCTOR", 50, COLOR.SECONDARY);
    headerInstructor.setPosition(leftColX - 90, bottomRowY + -10);

    contentInstructor = Label("Dr. Truong Toan Thinh", 40, COLOR.PRIMARY);
    bounds = contentInstructor.getLocalBounds();
    contentInstructor.setOrigin(bounds.width / 2.0, 0);
    contentInstructor.setPosition(leftColX + 15, bottomRowY + 20);


    // --- CỘT PHẢI (RIGHT COLUMN) ---

    // 3. CONTACT
    headerContact = Label("CONTACT", 50, COLOR.SECONDARY);
    headerContact.setPosition(rightColX - 120, topRowY + 50);

    contentContact = Label(
        "hpnguyen2527@clc.fitus.edu.vn\n"
        "nthquan2520@clc.fitus.edu.vn\n"
        "hdkhuong2539@clc.fitus.edu.vn\n"
        "vdakiet2520@clc.fitus.edu.vn\n"
        "nkphuc2511@clc.fitus.edu.vn",
        40, COLOR.PRIMARY);
    bounds = contentContact.getLocalBounds();
    contentContact.setOrigin(bounds.width / 2.0, 0);
    contentContact.setPosition(rightColX - 30, topRowY + 100);

    // 4. RESOURCES
    headerResources = Label("RESOURCES", 50, COLOR.SECONDARY);
    headerResources.setPosition(rightColX - 150, bottomRowY - 10);

    contentResources = Label(
        "Library:  C++  &  SFML\n"
        "Sound: OpenGameArt",
        40, COLOR.PRIMARY);
    bounds = contentResources.getLocalBounds();
    contentResources.setOrigin(bounds.width / 2.0, 0);
    contentResources.setPosition(rightColX - 40, bottomRowY + 20);
}

void About::selectOption() {
    closeMenu();
}

void About::draw() {
    headerGroup.setFillColor(COLOR.SECONDARY);
    headerInstructor.setFillColor(COLOR.SECONDARY);
    headerResources.setFillColor(COLOR.SECONDARY);
    headerContact.setFillColor(COLOR.SECONDARY);

    contentGroup.setFillColor(COLOR.PRIMARY);
    contentInstructor.setFillColor(COLOR.PRIMARY);
    contentResources.setFillColor(COLOR.PRIMARY);
    contentContact.setFillColor(COLOR.PRIMARY);

    drawPopUpMenu();
    WINDOW.draw(headerGroup);
    WINDOW.draw(contentGroup);
    WINDOW.draw(headerInstructor);
    WINDOW.draw(contentInstructor);
    WINDOW.draw(headerResources);
    WINDOW.draw(contentResources);
    WINDOW.draw(headerContact);
    WINDOW.draw(contentContact);
}


void MainScreen::selectOption(){
    SOUND.sfxMenuSelect.play();
    if (curChoice == 0) GAME_TYPE.chooseGameType(); // NEW GAME
    if (curChoice == 1) GAME_MANAGER.openMenu();// LOAD GAME
    if (curChoice == 2) SETTING.open(); // SETTINGS
     if(curChoice == 3) HELP.openMenu(); // HELP
     if(curChoice == 4) ABOUT.openMenu(); // ABOUT
    if(curChoice == 5) WINDOW.close(); // EXIT
}

void MainScreen::start() {
    SOUND.themeMusic.play();
    openMenu();
    while (isOpen) {
        WINDOW.clear(COLOR.BACKGROUND);
        drawMenu();
        if (SETTING.isOpen) {
            SETTING.EventController();
            SETTING.drawPopUpMenu();
        }
        else if (GAME_MANAGER.isOpen) {
            GAME_MANAGER.EventController();
            GAME_MANAGER.draw();
        }
        else if (HELP.isOpen) {
            HELP.EventController();
            HELP.draw();
        }
        else if (ABOUT.isOpen) {
            ABOUT.EventController();
            ABOUT.draw();
        }
        else EventController();
        WINDOW.display();
    }
}
