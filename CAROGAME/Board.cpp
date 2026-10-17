#include "Global.h"
#include "UI.h"
#include "Sound.h"
#include "Board.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;


void EffectX::create(float sz) {
    size = sz;

    if (size == 50.0) {
        line1.setTexture(TEXTURE.XLine1);
        line2.setTexture(TEXTURE.XLine2);
    }
    
    if(size == 80.0){
        line1.setTexture(TEXTURE.XMiniLine1);
        line2.setTexture(TEXTURE.XMiniLine2);
    }

    sf::FloatRect bounds = line1.getGlobalBounds();
    line1.setOrigin(bounds.left + bounds.width / 2.0, bounds.top + bounds.height / 2.0);
    line2.setOrigin(bounds.left + bounds.width / 2.0, bounds.top + bounds.height / 2.0);
}

void EffectX::reset() {
    progress = 0;
    line1.setTextureRect(sf::IntRect(0, 0, 0, 0));
    line2.setTextureRect(sf::IntRect(0, 0, 0, 0));
    isDone = false;
}

void EffectX::setPosition(float x, float y) {
    line1.setPosition(x, y);
    line2.setPosition(x, y);
}

void EffectX::updateDrawing(float dt) {
    progress += drawSpeed * dt;

    if (progress <= 1.0) line1.setTextureRect(sf::IntRect(0, 0, size, size * progress));
    else if (progress <= 2.0) line2.setTextureRect(sf::IntRect(0, 0, size, size * (progress - 1)));
    else {
        line1.setTextureRect(sf::IntRect(0, 0, size, size));
        line2.setTextureRect(sf::IntRect(0, 0, size, size));
        isDone = true;
    }
}

void EffectX::draw() {
    WINDOW.draw(line1);
    WINDOW.draw(line2);
}


void EffectO::create(float sz, sf::Texture& Otex) {
    texture = &Otex;
    radius = sz / 2.0;
    fan.setPrimitiveType(sf::TriangleFan);
    centerTex = sf::Vector2f(sz / 2.0, sz / 2.0);
}

void EffectO::reset() {
    curAngle = 0;
    isDone = false;
}

void EffectO::setPosition(float x, float y) {
    centerPos = sf::Vector2f(x, y);
}

void EffectO::updateDrawing(float dt) {
    curAngle += drawSpeed * dt;

    if (curAngle >= 360) {
        curAngle = 360;
        isDone = true;
    }

    fan.clear();
    fan.append(sf::Vertex(centerPos, sf::Color::White, centerTex));

    for (float a = 0; a <= curAngle; a += 5.0) {
        float rad = (a - 90.0) * PI / 180.0;
        float dx = radius * cos(rad);
        float dy = radius * sin(rad);

        sf::Vector2f pos = centerPos + sf::Vector2f(dx, dy);
        sf::Vector2f tex = centerTex + sf::Vector2f(dx, dy);

        fan.append(sf::Vertex(pos, sf::Color::White, tex));
    }
}

void EffectO::draw() {
    if (texture) {
        sf::RenderStates states;
        states.texture = texture;
        WINDOW.draw(fan, states);
    }
}


void Cell::resetPop() {
    isPopping = true;
    scale = 1.0;
}

void Cell::updatePop(float dt) {
    if (!isPopping && !isSettling) return;

    if (isPopping) {
        scale += dt * popSpeed;
        if (scale >= maxScale) {
            scale = maxScale;
            isPopping = false;
            isSettling = true;
        }
    }
    else if (isSettling) {
        scale -= dt * popSpeed;

        if (scale <= minScale) {
            scale = minScale;
            isSettling = false;
        }
    }

    spriteMark.setScale(scale, scale);
}

void Cell::draw() {
    if ((logicMark == 'X' || logicMark == 'O') && visible) WINDOW.draw(spriteMark);
}

void Board::create(float x, float y, int boardSize, float boardPixelSize, float cellSize, float thickness, int num, sf::Texture& boardTexture, sf::Texture& X, sf::Texture& O) {
    BOARD_SIZE = boardSize;
    numInRow = num;
    effectVisible = false;
    score = 0;
    scoreHistory.push(score);

    board.setSize(sf::Vector2f(boardPixelSize, boardPixelSize));
    board.setTexture(&boardTexture);
    board.setOrigin(boardPixelSize / 2.0, boardPixelSize / 2.0);
    board.setPosition(x, y);

    Xmark.setTexture(X);
    Omark.setTexture(O);

    virtualX.create(cellSize);
    virtualO.create(cellSize, O);

    float startX = x - (BOARD_SIZE / 2) * (cellSize + thickness);
    float startY = y - (BOARD_SIZE / 2) * (cellSize + thickness);

    cells.resize(BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; i++) {
        cells[i].resize(BOARD_SIZE, Cell(cellSize));
        for (int j = 0; j < BOARD_SIZE; j++) {
            float x = startX + j * (cellSize + thickness);
            float y = startY + i * (cellSize + thickness);
            cells[i][j].spriteMark.setPosition(x, y);
            cells[i][j].x = x;
            cells[i][j].y = y;
            cells[i][j].row = i;
            cells[i][j].col = j;
        }
    }

    highlight.setSize(sf::Vector2f(cellSize, cellSize));
    highlight.setOrigin(cellSize / 2.0, cellSize / 2.0);
    highlight.setOutlineThickness(thickness+3);
    highlight.setFillColor(sf::Color::Transparent);
}

void Board::resetData(){
    isWin = isDraw = isDone = false;
    effectVisible = false;
    curX = curY = BOARD_SIZE / 2;
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++) {
            cells[i][j].logicMark = '.';
            cells[i][j].spriteMark.setScale(1.0, 1.0);
            cells[i][j].visible = true;
        }
    highlight.setOutlineColor(COLOR.X);
    effectCells.clear();
}

void Board::setHighlight() {    
    highlight.setPosition(cells[curX][curY].spriteMark.getPosition());
    WINDOW.draw(highlight);
}

void Board::updateHighlight(sf::Color color) {
    highlight.setOutlineColor(color);
}

void Board::draw(int highlightVisible) {
    WINDOW.draw(board);
    for(int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            cells[i][j].draw();
    if(highlightVisible) setHighlight();
}

void Board::moveCursor(int dx, int dy) {
    SOUND.sfxBoardMove.play();
    if (0 <= curX + dx && curX + dx < BOARD_SIZE && 0 <= curY + dy && curY + dy < BOARD_SIZE) 
        curX += dx, curY += dy;
}

bool Board::checkWin(char mark, bool getCells){
    bool isWin = false;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // check row
            if (BOARD_SIZE - j >= numInRow) {
                isWin = true;
                for (int k = 0; k < numInRow; k++)
                    if (cells[i][j + k].logicMark != mark) {
                        isWin = false;
                        break;
                    }
                if (isWin && getCells) {
                    for (int k = 0; k < numInRow; k++) 
                        effectCells.push_back(&cells[i][j + k]);
                    break;
                }
            }

            // check column
            if (BOARD_SIZE - i >= numInRow) {
                isWin = true;
                for (int k = 0; k < numInRow; k++)
                    if (cells[i + k][j].logicMark != mark) {
                        isWin = false;
                        break;
                    }
                if (isWin && getCells) {
                    for (int k = 0; k < numInRow; k++)
                        effectCells.push_back(&cells[i + k][j]);
                    break;
                }
            }

            // check right down diagonal 
            if (BOARD_SIZE - i >= numInRow && BOARD_SIZE - j >= numInRow) {
                isWin = true;
                for (int k = 0; k < numInRow; k++)
                    if (cells[i + k][j + k].logicMark != mark) {
                        isWin = false;
                        break;
                    }
                if (isWin && getCells) {
                    for (int k = 0; k < numInRow; k++)
                        effectCells.push_back(&cells[i + k][j + k]);
                    break;
                }
            }

            // check right up diagonal 
            if (i + 1 >= numInRow && BOARD_SIZE - j >= numInRow) {
                isWin = true;
                for (int k = 0; k < numInRow; k++)
                    if (cells[i - k][j + k].logicMark!= mark) {
                        isWin = false;
                        break;
                    }
                if (isWin && getCells){
                    for (int k = 0; k < numInRow; k++)
                        effectCells.push_back(&cells[i - k][j + k]);
                    break;
                }
            }
        }
        if (isWin) break;
    }
    return isWin;
}

bool Board::checkDraw(bool getCells){
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if(cells[i][j].logicMark == '.') 
                return false;

    if (getCells) {
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j <= i; j++)
                effectCells.push_back(&cells[i - j][j]);

        for (int j = 1; j < BOARD_SIZE; j++)
            for (int i = 0; j + i < BOARD_SIZE; i++)
                effectCells.push_back(&cells[BOARD_SIZE - 1 - i][j + i]);
    }

    return true;
}

void Board::startDrawingMark(char mark, float x, float y) {
    if (mark == 'X') {
        isDrawingX = true;
        virtualX.setPosition(x, y);
        virtualX.reset();
    }

    if (mark == 'O') {
        isDrawingO = true;
        virtualO.setPosition(x, y);
        virtualO.reset();
    }
   
    cells[curX][curY].visible = false;
    frameClock.restart();
}

void Board::drawingMark() {
    float dt = frameClock.restart().asSeconds();
    if (isDrawingX) {
        if (!virtualX.isDone) {
            virtualX.updateDrawing(dt);
            virtualX.draw();
        }
        else {
            isDrawingX = false;
            cells[curX][curY].visible = true;
        }
    }
    if (isDrawingO) {
        if (!virtualO.isDone) {
            virtualO.updateDrawing(dt);
            virtualO.draw();
        }
        else {
            isDrawingO = false;
            cells[curX][curY].visible = true;
        }
    }
}

void Board::startWaveEffect() {
    effectVisible = true;
    frameClock.restart();
    effectClock.restart();
    for (Cell* c : effectCells)
        c->resetPop();
}

void Board::updateWinEffect() {
    float dt = frameClock.restart().asSeconds();
    float t = effectClock.getElapsedTime().asSeconds();

    if (t >= 0.9) isDone = true;

    if (t >= 2.0) {
        effectClock.restart();
        t = 0;
        for (Cell* c : effectCells)
            c->resetPop();
    }

    float delay = 0.15;
    for (int i = 0; i < numInRow; i++)
        if(t >= i*delay)
            effectCells[i]->updatePop(dt);
}

void Board::updateDrawEffect() {
    float dt = frameClock.restart().asSeconds();
    float t = effectClock.getElapsedTime().asSeconds();

    if (t >= 0.9) isDone = true;

    if (t >= 4.5) {
        effectClock.restart();
        t = 0;
        for (Cell* c : effectCells)
            c->resetPop();
    }

    float delay = 0.1;
    for (Cell* c : effectCells)
        if (t >= (c->row + c->col) * delay)
            c->updatePop(dt);


}

bool Board::isValidCell(){
    if(cells[curX][curY].logicMark == '.') return true;
    return false;
}

int Board::evaluate(int count, int openEnds) {
    if (openEnds == 0 && count < 5) return 0;

    if (count == 5) return 100000000;

    if (count == 4) return 50000000;

    if (count == 3) {
        if (openEnds == 2) return 50000000;
        if (openEnds == 1) return 50000;  
    }

    if (count == 2) {
        if (openEnds == 2) return 5000;
        if (openEnds == 1) return 100;
    }

    if (count == 1) return 10;

    return 0;
}

void Board::updateScore(char curMark) {
    int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    int dy[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

    int val = 0;
    for (int k = 0; k < 8; k++) {
        int x = curX + dx[k];
        int y = curY + dy[k];
        
        if (0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE && cells[x][y].logicMark != '.') {
            int count = 0;
            int openEnds = 1;
            char mark = cells[x][y].logicMark;

            while (count < 5 && openEnds < 2 && 0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE) {
                if (cells[x][y].logicMark == '.') ++openEnds;
                else if (cells[x][y].logicMark == mark) ++count;
                else break;
                x += dx[k];
                y += dy[k];
            }
            
            if (mark == 'O') val += evaluate(count, openEnds);
            else val -= evaluate(count, openEnds);
        }
    }

    score -= val;

    val = 0;

    if (curMark == 'O') val += evaluate(1, 0);
    else val -= evaluate(1, 0);

    for (int k = 0; k < 8; k++) {
        int x = curX + dx[k];
        int y = curY + dy[k];

        if (0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE && cells[x][y].logicMark != '.') {
            int count = 0;
            int openEnds = 0;
            char mark = cells[x][y].logicMark;

            if (mark != curMark) {
                while (count < 5 && openEnds < 2 && 0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE) {
                    if (cells[x][y].logicMark == '.') ++openEnds;
                    else if (cells[x][y].logicMark == mark) ++count;
                    else break;
                    x += dx[k];
                    y += dy[k];
                }

                if (mark == 'O') val += evaluate(count, openEnds);
                else val -= evaluate(count, openEnds);
                
            }
            else if (k < 4) {
                while (count < 5 && openEnds < 2 && 0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE) {
                    if (cells[x][y].logicMark == '.') ++openEnds;
                    else if (cells[x][y].logicMark == mark) ++count;
                    else break;
                    x += dx[k];
                    y += dy[k];
                }
                
                x = curX;
                y = curY;

                while (count < 5 && openEnds < 2 && 0 <= x && x < BOARD_SIZE && 0 <= y && y < BOARD_SIZE) {
                    if (cells[x][y].logicMark == '.') ++openEnds;
                    else if (cells[x][y].logicMark == mark) ++count;
                    else break;
                    x += dx[k + 4];
                    y += dy[k + 4];
                }

                if (mark == 'O') val += evaluate(count, openEnds);
                else val -= evaluate(count, openEnds);
            }
        }
    }

    score += val;

    scoreHistory.push(score);
}

bool Board::placeMark(char mark, bool write){
    if (!isValidCell()) return false;
    cells[curX][curY].logicMark = mark;
    if (mark == 'X') {
        if (write) {
            SOUND.sfxXPlaceMark.play();
            startDrawingMark(mark, cells[curX][curY].x, cells[curX][curY].y);
        }
        cells[curX][curY].spriteMark.setTexture(*Xmark.getTexture());
    }
    if (mark == 'O') {
        if (write) {
            SOUND.sfxOPlaceMark.play();
            startDrawingMark(mark, cells[curX][curY].x, cells[curX][curY].y);
        }
        cells[curX][curY].spriteMark.setTexture(*Omark.getTexture());
    }

    markPosition.emplace_back(curX, curY);
    updateScore(mark);
    return true;
}

void Board::removeMark() {
    markPosition.pop_back();
    scoreHistory.pop();
    cells[curX][curY].logicMark = '.';
    score = scoreHistory.top();
}
