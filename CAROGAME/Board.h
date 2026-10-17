#ifndef BOARD_H
#define BOARD_H
#include "Global.h"
#include "UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <stack>


struct EffectX {
    sf::Sprite line1, line2;
    float size;
    const float drawSpeed = 4.0;

    bool isDone;
    float progress;
    void create(float);
    void reset();
    void updateDrawing(float);
    void setPosition(float, float);
    void draw();
};

struct EffectO {
    sf::VertexArray fan;
    const sf::Texture *texture;
    
    sf::Vector2f centerPos, centerTex;
    const float PI = 3.14159265;
    const float drawSpeed = 800;
    float radius;
    float curAngle;
    bool isDone;

    void create(float, sf::Texture&);
    void reset();
    void updateDrawing(float);
    void setPosition(float, float);
    void draw();
};

struct Cell {
    char logicMark;
    sf::Sprite spriteMark;
    sf::Clock popClock;
    const float popSpeed = 5.0;
    const float maxScale = 2.0;
    const float minScale = 1.0;
    float x, y;
    int row, col;
    float scale;
    bool isPopping, isSettling;
    bool visible;

    Cell(float sz){
        visible = true;
        spriteMark.setOrigin(sz / 2.0, sz / 2.0);
        logicMark = '.';
    }

    void resetPop();
    void updatePop(float);
    void draw();
};

struct Board {
    int BOARD_SIZE;
    int numInRow;
    int curX, curY;
    bool isWin, isDraw, isDone, isDrawingX, isDrawingO, effectVisible;
    int score;
    stack<int> scoreHistory;
    vector<vector<Cell>> cells;
    vector<Cell*> effectCells;
    EffectX virtualX;
    EffectO virtualO;


    sf::RectangleShape highlight;
    sf::RectangleShape board;
    sf::Sprite Xmark, Omark;
    sf::Clock frameClock, effectClock;
    vector<pair<int, int>> markPosition;
  
    

    void create(float, float, int, float, float, float, int, sf::Texture&, sf::Texture&, sf::Texture&);
    void resetData();
    void draw(int);
    void moveCursor(int, int);
    void setHighlight();
    void updateHighlight(sf::Color);
    bool isValidCell();
    bool placeMark(char, bool);
    void removeMark();
    bool checkWin(char, bool);
    bool checkDraw(bool);
    void startDrawingMark(char, float, float);
    void drawingMark();
    void startWaveEffect();
    void updateWinEffect();
    void updateDrawEffect();
    int evaluate(int, int);
    void updateScore(char);

    Board() {}
};

#endif