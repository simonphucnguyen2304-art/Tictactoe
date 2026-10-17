#define _CRT_SECURE_NO_WARNINGS 
#include "Global.h"
#include "UI.h"
#include "Sound.h"
#include "Mainscreen.h"
#include "Game.h"
#include "Setting.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace std;
namespace fs = filesystem;

void GameType::selectOption() {
    SOUND.sfxMenuSelect.play();
    closeMenu();
    if (curChoice == 0) CLASSIC_GAME.start();
    else EXTREME_GAME.start();
}

void GameType::chooseGameType() {
    openMenu();
    while (isOpen) {
        WINDOW.clear(COLOR.BACKGROUND);
        EventController();
        drawMenu();
        WINDOW.display();
    }
}


void GameManager::selectOption() {
    SOUND.sfxMenuSelect.play();
    if (curChoice.second == 0 && numFiles > 0) { // LOAD
        closeMenu();
        if (files[curChoice.first].mode != 2) CLASSIC_GAME.loadGame(files[curChoice.first].nameText.getString().toAnsiString());
        else EXTREME_GAME.loadGame(files[curChoice.first].nameText.getString().toAnsiString());
    }
    if (curChoice.second == 1) // DELETE
        deleteFile();
    if (curChoice.second == 2) // CLOSE
        closeMenu();
}

void GameManager::moveCursor(int x, int y) {
    SOUND.sfxMenuMove.play();
    curChoice.first = max(0, min(curChoice.first + x, numFiles-1));
    curChoice.second = (curChoice.second + y + numButtons) % numButtons;

    if (curChoice.first > R) {
        ++R;
        ++L;
    }

    if (curChoice.first < L) {
        --L;
        --R;
    }
}

void GameManager::resetData() {
    curChoice = { 0, 0 };
    inButtonRow = true;
    L = 0;
    R = 2;
    for (int i = 0; i < numTextBoxes; i++) {
        textboxes[i].input = "";
        textboxes[i].cntChar = 0;
    }
}

void GameManager::getFiles() {
    for (const auto& entry : fs::directory_iterator("saves")) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            string filename = entry.path().filename().string();
            ifstream load("saves/" + filename);
            int mode;
            load >> mode;
            load.close();
            addFile(filename, mode);
        }
    }
}

void GameManager::addFile(string filename, int mode) {
    files.push_back(File(filename, mode));
    ++numFiles;
    updateModifiedDate(filename);
}

void GameManager::deleteFile() {
    if (numFiles == 0) return;

    fs::remove("saves/" + files[curChoice.first].nameText.getString().toAnsiString());
    files.erase(files.begin() + curChoice.first);
    --numFiles;

    if (curChoice.first >= numFiles && numFiles > 0)
        curChoice.first = numFiles - 1;
    

    if (L + 2 >= numFiles) {
        L = max(0, numFiles - 3);
        R = L + 2;
    }

}

void GameManager::updateModifiedDate(string filename) {
    for(int i=0; i<numFiles; i++)
        if (files[i].nameText.getString().toAnsiString() == filename) {
            auto ftime = fs::last_write_time("saves/" + filename);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now()
                + std::chrono::system_clock::now()
            );

            std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
            std::tm tm{};
            localtime_s(&tm, &cftime);

            std::ostringstream oss;
            oss << std::put_time(&tm, "%d/%m/%Y");
            files[i].modifiedTimeText = Label(oss.str(), 40, COLOR.PRIMARY);
            break;
        }
}

void GameManager::drawFileInfo(float startX, float startY, float spacing) {
    for (int i = 0; i < min(numFiles, 3); i++) {
        if (curChoice.first == L + i) {
            files[L + i].box.setHoveredState();
            files[L + i].nameText.setHoveredState();
            files[L + i].modeText.setHoveredState();
            files[L + i].modifiedTimeText.setHoveredState();
        }
        else {
            files[L + i].box.setNormalState();
            files[L + i].nameText.setNormalState();
            files[L + i].modeText.setNormalState();
            files[L + i].modifiedTimeText.setNormalState();
        }

        files[L + i].box.setPosition(startX, startY + i * spacing);
        files[L + i].nameText.setPosition(startX - 450, startY + i * spacing);
        files[L + i].modeText.setPosition(startX + 100, startY + i * spacing);
        files[L + i].modifiedTimeText.setPosition(startX + 300, startY + i * spacing);

        WINDOW.draw(files[L + i].box);
        WINDOW.draw(files[L + i].nameText);
        WINDOW.draw(files[L + i].modeText);
        WINDOW.draw(files[L + i].modifiedTimeText);
    }
}

void GameManager::draw() {
    drawPopUpMenu();
    drawFileInfo(WINDOW.WIDTH / 2.0, 400, 150);
}



void GameOption::selectOption() {
    SOUND.sfxMenuSelect.play();
    if (curChoice == 0) // SAVE GAME
        if (controller->filename.empty()) {
            controller->GAME_SAVER.wantQuit = false;
            controller->GAME_SAVER.openMenu();
        } 
        else controller->saveGame(0);

    if (curChoice == 1) // SETTINGS
        SETTING.open();

    if (curChoice == 2) // HELP
        HELP.openMenu();

    if (curChoice == 3) // QUIT GAME
        controller -> openDialog();
 
    if (curChoice == 4) { // CLOSE
        closeMenu();
        controller -> resume();
    }
}


void GameOver::selectOption() {
    SOUND.sfxMenuSelect.play();
    if (curChoice == 0) { // NEW GAME
        SOUND.sfxTear.play();
        (*controller).apply();
        controller->virtualPaper.triggerCut();
        (*controller).reset();
        controller->tearEffectRunning = true;

        closeMenu();
        controller->resetData();
        controller->runGame();
    }

    if (curChoice == 1) { // SAVE GAME
        if (controller->filename.empty()) {
            controller->GAME_SAVER.wantQuit = false;
            controller->GAME_SAVER.openMenu();
        }
        else controller->saveGame(0);
    }

    if (curChoice == 2) // QUIT GAME
        controller -> openDialog();
    
}


void GameMode::selectOption() {
    SOUND.sfxMenuSelect.play();
    controller->gameMode = curChoice;
    closeMenu();
}

void GameMode::chooseGameMode() {
    openMenu();
    while (isOpen) {
        WINDOW.clear(COLOR.BACKGROUND);
        EventController();
        drawMenu();
        WINDOW.display();   
    }
}


void PlayerNameInput::selectOption() {
    SOUND.sfxMenuSelect.play();
    if (curChoice.first < numTextBoxes) {
        setActive(false);
        textboxes[curChoice.first].isTyping = true;
    }
    else {
        for (int i = 0; i < numTextBoxes; i++) {
            if (textboxes[i].input.empty()) {
                controller->PLAYER[i].name = textboxes[i].defaultText;
                controller->PLAYER[i].nameText.setString(textboxes[i].defaultText);
            }
            else {
                controller->PLAYER[i].name = textboxes[i].input;
                controller->PLAYER[i].nameText.setString(textboxes[i].input);
            }
        }
        closeMenu();
    }
}

void PlayerNameInput::collectPlayerName() {
    openMenu();
    while (isOpen) {
        WINDOW.clear(COLOR.BACKGROUND);

        if (curChoice.first < numTextBoxes) {
            if (textboxes[curChoice.first].isTyping) textboxes[curChoice.first].read();
            else setActive(true);
        }

        if(isActive) EventController();

        drawMenu();
        WINDOW.display();
    }
}


void GameSaver::selectOption() {
    SOUND.sfxMenuSelect.play();
    if (curChoice.first < numTextBoxes) {
        setActive(false);
        textboxes[curChoice.first].isTyping = true;
    }
    else {
        if (curChoice.second == 0) {
            if (textboxes[0].input.empty()) controller->filename = getCurrentDateTime();
            else controller->filename = textboxes[0].input + ".txt";
            controller -> saveGame(1);
            if (wantQuit) controller -> quit();
        }
        closeMenu();  
    }
}

string GameSaver::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(std::localtime(&in_time_t), "%d%m%Y-%H%M%S.txt");
    return ss.str();
}

void GameSaver::getFileName() {
    if (curChoice.first < numTextBoxes) {
        if (textboxes[curChoice.first].isTyping) textboxes[curChoice.first].read();
        else setActive(true);
    }
    if (isActive) EventController();
}


void GameDialog::selectOption() {
    SOUND.sfxMenuSelect.play();
    if (curChoice.second == 0)  // YES = SAVE AND QUIT
        if (controller->filename.empty()) {
            controller->GAME_SAVER.wantQuit = true;
            controller->GAME_SAVER.openMenu();
        }
        else {
            controller->saveGame(0);
            controller->quit();
        }
    
    if (curChoice.second == 1) // NO = QUIT WITHOUT SAVE
        controller -> quit();

    if (curChoice.second == 2) // CANCEL = NO QUIT
        closeMenu();
}



void GameController::pause() {
    pausing = true;
}

void GameController::resume() {
    pausing = false;
}

void GameController::quit() {
    inSession = false;
    inGame = false;
    GAME_OPTIONS.closeMenu();
    GAME_OVER.closeMenu();
    GAME_DIALOG.closeMenu();
    GAME_SAVER.closeMenu();
}

void GameController::stop() {
    gameOver = pausing = true;
    pause();
}

void GameController::openDialog() {
    GAME_DIALOG.openMenu();
}

void GameController::setGameMode(int gm){
    gameMode = gm;
}

void GameController::createStats(float startX, float startY, float spacing) {
    for (int i = 0; i < 2; i++) {
        if (i == 0) PLAYER[i] = Player('X', COLOR.X, TEXTURE.XStatsBox, TEXTURE.XWinBox);
        else PLAYER[i] = Player('O', COLOR.O, TEXTURE.OStatsBox, TEXTURE.OWinBox);

        PLAYER[i].statsBox.setPosition(startX, startY + i * spacing + 55);
        PLAYER[i].winBox.setPosition(startX, startY + i * spacing + 55);

        PLAYER[i].nameText.setPosition(startX - 40, startY + i * spacing + 25);
        PLAYER[i].offsetName = PLAYER[i].statsBox.getInverseTransform().transformPoint(PLAYER[i].nameText.getPosition());

        PLAYER[i].scoreText.first.setPosition(startX - 40, startY + i * spacing + 70);
        PLAYER[i].offsetScore.first = PLAYER[i].statsBox.getInverseTransform().transformPoint(PLAYER[i].scoreText.first.getPosition());

        PLAYER[i].winStreakText.first.setPosition(startX - 40, startY + i * spacing + 100);
        PLAYER[i].offsetWinStreak.first = PLAYER[i].statsBox.getInverseTransform().transformPoint(PLAYER[i].winStreakText.first.getPosition());

        PLAYER[i].moveCountText.first.setPosition(startX - 40, startY + i * spacing + 130);
        PLAYER[i].offsetMoveCount.first = PLAYER[i].statsBox.getInverseTransform().transformPoint(PLAYER[i].moveCountText.first.getPosition());
    }
}

void GameController::drawStats(float startX, float startY, float spacing) {
    for (int i = 0; i < 2; i++) {
        PLAYER[i].scoreText.second = Label(to_string(PLAYER[i].score), 20, PLAYER[i].color);
        PLAYER[i].scoreText.second.setPosition(startX + 120, startY + i * spacing + 70);
        PLAYER[i].offsetScore.second = PLAYER[i].statsBox.getInverseTransform().transformPoint(PLAYER[i].scoreText.second.getPosition());

        PLAYER[i].winStreakText.second = Label(to_string(PLAYER[i].winStreak), 20, PLAYER[i].color);
        PLAYER[i].winStreakText.second.setPosition(startX + 120, startY + i * spacing + 100);
        PLAYER[i].offsetWinStreak.second = PLAYER[i].statsBox.getInverseTransform().transformPoint(PLAYER[i].winStreakText.second.getPosition());

        PLAYER[i].moveCountText.second = Label(to_string(PLAYER[i].moveCount), 20, PLAYER[i].color);
        PLAYER[i].moveCountText.second.setPosition(startX + 120, startY + i * spacing + 130);
        PLAYER[i].offsetMoveCount.second = PLAYER[i].statsBox.getInverseTransform().transformPoint(PLAYER[i].moveCountText.second.getPosition());

        if (gameOver && gameResult == 1 && curPlayer == i) {
            PLAYER[i].nameText.setScale(1.1, 1.1);
            PLAYER[i].nameText.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetName));

            PLAYER[i].scoreText.first.setScale(1.1, 1.1);
            PLAYER[i].scoreText.first.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetScore.first));

            PLAYER[i].scoreText.second.setScale(1.1, 1.1);
            PLAYER[i].scoreText.second.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetScore.second));

            PLAYER[i].winStreakText.first.setScale(1.1, 1.1);
            PLAYER[i].winStreakText.first.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetWinStreak.first));

            PLAYER[i].winStreakText.second.setScale(1.1, 1.1);
            PLAYER[i].winStreakText.second.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetWinStreak.second));

            PLAYER[i].moveCountText.first.setScale(1.1, 1.1);
            PLAYER[i].moveCountText.first.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetMoveCount.first));

            PLAYER[i].moveCountText.second.setScale(1.1, 1.1);
            PLAYER[i].moveCountText.second.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetMoveCount.second));
        }
        else {
            PLAYER[i].nameText.setScale(1.0, 1.0);
            PLAYER[i].nameText.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetName));

            PLAYER[i].scoreText.first.setScale(1.0, 1.0);
            PLAYER[i].scoreText.first.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetScore.first));

            PLAYER[i].scoreText.second.setScale(1.0, 1.0);
            PLAYER[i].scoreText.second.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetScore.second));

            PLAYER[i].winStreakText.first.setScale(1.0, 1.0);
            PLAYER[i].winStreakText.first.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetWinStreak.first));

            PLAYER[i].winStreakText.second.setScale(1.0, 1.0);
            PLAYER[i].winStreakText.second.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetWinStreak.second));

            PLAYER[i].moveCountText.first.setScale(1.0, 1.0);
            PLAYER[i].moveCountText.first.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetMoveCount.first));

            PLAYER[i].moveCountText.second.setScale(1.0, 1.0);
            PLAYER[i].moveCountText.second.setPosition(PLAYER[i].statsBox.getTransform().transformPoint(PLAYER[i].offsetMoveCount.second));
        }

        if (gameOver && gameResult == 1 && curPlayer == i) WINDOW.draw(PLAYER[i].winBox);
        else WINDOW.draw(PLAYER[i].statsBox);
        WINDOW.draw(PLAYER[i].nameText);
        WINDOW.draw(PLAYER[i].scoreText.first);
        WINDOW.draw(PLAYER[i].scoreText.second);
        WINDOW.draw(PLAYER[i].winStreakText.first);
        WINDOW.draw(PLAYER[i].winStreakText.second);
        WINDOW.draw(PLAYER[i].moveCountText.first);
        WINDOW.draw(PLAYER[i].moveCountText.second);
    }
}

void GameController::start() {}

void GameController::resetData() {}

void GameController::runGame() {}

void GameController::saveGame(bool newFile) {};

void GameController::loadGame(string filename) {};

void GameController::switchPlayer() {}

void GameController::EventController() {}

void GameController::apply() {}

void GameController::reset() {
    WINDOW.clear(COLOR.BACKGROUND);
    WINDOW.draw(paper_background);
}



void Bot::getCandidateMoves(vector<Move>* moves) {
    for (pair<int, int> pos : board->markPosition) {
        int x = pos.first;
        int y = pos.second;
        for (int dx = -maxRange; dx <= maxRange; dx++)
            for (int dy = -maxRange; dy <= maxRange; dy++)
                if (0 <= x + dx && x + dx < board->BOARD_SIZE && 0 <= y + dy && y + dy < board->BOARD_SIZE && board->cells[x + dx][y + dy].logicMark == '.' && !vis[x + dx][y + dy]) {
                    vis[x + dx][y + dy] = true;
                    moves->emplace_back(x + dx, y + dy, 0);
                }
    }

    if (moves->empty())
        for (int i = 0; i < board->BOARD_SIZE; i++)
            for (int j = 0; j < board->BOARD_SIZE; j++)
                if (board->cells[i][j].logicMark == '.')
                    moves->emplace_back(i, j, 0);

    
    for (Move m : *moves) vis[m.x][m.y] = false;

}

void Bot::makeMove() {
    minimax(maxDepth, -inf, inf, 0);
    board->curX = curX;
    board->curY = curY;
    board->placeMark('O', 1);
}

int Bot::minimax(int depth, int alpha, int beta, int curTurn) {
    if (board->checkWin('X', 0)) return -100000000;
    if (board->checkWin('O', 0)) return 100000000;
    if (board->checkDraw(0)) return 0;
    if (depth == 0) return board->score;

    int score;

    vector<Move> moves;
    getCandidateMoves(&moves);

    // Evaluate moves;
    for (Move m : moves) {
        int oldX = board->curX;
        int oldY = board->curY;
        board->curX = m.x;
        board->curY = m.y;
        board->placeMark('O', 0);

        m.score = board->score;

        board->removeMark();
        board->curX = oldX;
        board->curY = oldY;
    }

    sort(moves.begin(), moves.end());
    
    while (moves.size() > 12) moves.pop_back();

    if (curTurn == 0) { // BOT
        score = -inf;

        for (Move m : moves) {
            int oldX = board->curX;
            int oldY = board->curY;
            board->curX = m.x;
            board->curY = m.y;
            board->placeMark('O', 0);

            int newScore = minimax(depth - 1, alpha, beta, curTurn ^ 1);

            if (newScore > score) {
                score = newScore;
                if (depth == maxDepth) {
                    curX = board->curX;
                    curY = board->curY;
                }
            }

            board->removeMark();
            board->curX = oldX;
            board->curY = oldY;

            alpha = max(alpha, score);
            if (alpha >= beta) break;
        }
    }
    else { // HUMAN;
        score = inf;

        reverse(moves.begin(), moves.end());
        for (Move m : moves) {
            int oldX = board->curX;
            int oldY = board->curY;
            board->curX = m.x;
            board->curY = m.y;
            board->placeMark('X', 0);

            int newScore = minimax(depth - 1, alpha, beta, curTurn ^ 1);

            if (newScore < score) {
                score = newScore;
                if (depth == maxDepth) {
                    curX = board->curX;
                    curY = board->curY;
                }
            }

            board->removeMark();
            board->curX = oldX;
            board->curY = oldY;
            beta = min(beta, score);
            if (beta <= alpha) break;
        }
    }

    return score;
}



void ClassicGame::start() {
    GAME_MODE.chooseGameMode();
    createStats(1440, 230, 260);
    PLAYER_NAME.collectPlayerName();
    resetData();
    runGame();
}

void ClassicGame::resetData() {
    curPlayer = 0;
    gameResult = -1;
    PLAYER[0].moveCount = PLAYER[1].moveCount = 0;
    if (!inSession) { // If start a new game in a session then do not reset players' scores
        PLAYER[0].score = PLAYER[1].score = 0;
        PLAYER[0].winStreak = PLAYER[1].winStreak = 0;
        filename = "";
    }
    BOARD.resetData();
}

void ClassicGame::drawBoard() {
    if (BOARD.isDrawingX || BOARD.isDrawingO) BOARD.drawingMark();
    else if (BOARD.effectVisible) {
        if (BOARD.isWin) BOARD.updateWinEffect();
        if (BOARD.isDraw) BOARD.updateDrawEffect();
    }
    else if (BOARD.isWin || BOARD.isDraw) BOARD.startWaveEffect();

    BOARD.draw(1);
}

void ClassicGame::runGame() {
    inSession = true;
    inGame = true;
    gameOver = false;
    pausing = false;
    while (inGame) {
        WINDOW.clear(COLOR.BACKGROUND);
        WINDOW.draw(paper_background);
        

        if (!pausing) {
            if (gameMode == 1 && curPlayer == 1) {
                if (!BOARD.isDrawingX) {
                    BOT.makeMove();
                    if (BOARD.checkWin('O', 1)) {
                        SOUND.sfxWin.play();
                        BOARD.isWin = true;


                        ++PLAYER[curPlayer].score;
                        ++PLAYER[curPlayer].winStreak;
                        PLAYER[curPlayer ^ 1].winStreak = 0;

                        stop();
                        GAME_OVER.openMenu();
                    }
                    else if (BOARD.checkDraw(1)) {
                        SOUND.sfxDraw.play();
                        BOARD.isDraw = true;

                        PLAYER[0].winStreak = PLAYER[1].winStreak = 0;

                        stop();
                        GAME_OVER.openMenu();
                    }
                    else switchPlayer();
                }
            }
            else EventController();
        }

        if (gameResult == -1 && BOARD.isDone)
            if (BOARD.isWin) gameResult = 1;
            else gameResult = 0;

        drawBoard();
        drawStats(1440, 230, 260);
        
        if (tearEffectRunning && virtualPaper.isDone) {
            virtualPaper.update();

            if (virtualPaper.isDone) {

                cap_part.setSize(sf::Vector2f(virtualPaper.zone_cap.width, virtualPaper.zone_cap.height));
                cap_part.setOrigin(virtualPaper.zone_cap.width / 2.0, virtualPaper.zone_cap.height / 2.0);
                cap_part.setPosition(
                    virtualPaper.zone_cap.left + virtualPaper.zone_cap.width / 2,
                    virtualPaper.zone_cap.top + virtualPaper.zone_cap.height / 2);
                cap_part.setTexture(&virtualPaper.paper_cap);
                cap_part.setTextureRect(sf::IntRect(
                    (int)virtualPaper.startX,
                    (int)virtualPaper.startY,
                    (int)virtualPaper.zone_cap.width,
                    (int)virtualPaper.zone_cap.height
                ));

                float rotation = sin(virtualPaper.total_time * 3) * 15.0;
                cap_part.setRotation(rotation);
                WINDOW.draw(cap_part);

            }

            if (virtualPaper.zone_cap.top > WINDOW.getSize().y + 100) {
                virtualPaper.reset();
                tearEffectRunning = false;
            }
        }

        if (GAME_OPTIONS.isOpen) {
            if (GAME_SAVER.isOpen) {
                GAME_SAVER.getFileName();
                GAME_SAVER.drawPopUpMenu();
            }
            else if (SETTING.isOpen) {
                SETTING.EventController();
                SETTING.drawPopUpMenu();
            }
            else if (GAME_DIALOG.isOpen) {
                GAME_DIALOG.EventController();
                GAME_DIALOG.drawPopUpMenu();
            }
            else if (HELP.isOpen) {
                HELP.EventController();
                HELP.draw();
            }
            else {
                GAME_OPTIONS.EventController();
                GAME_OPTIONS.drawPopUpMenu();
            }
        }
        
        if (BOARD.isDone) {
            GAME_OVER.drawMenu();
            if (GAME_SAVER.isOpen) {
                GAME_SAVER.getFileName();
                GAME_SAVER.drawPopUpMenu();
            }
            else if (GAME_DIALOG.isOpen) {
                GAME_DIALOG.EventController();
                GAME_DIALOG.drawPopUpMenu();
            }
            else GAME_OVER.EventController();
        }
        WINDOW.display();
    }
}

void ClassicGame::saveGame(bool newFile) {
    if (!filesystem::exists("saves")) {
        filesystem::create_directory("saves");
    }

    ofstream save("saves/" + filename);
    if (!save) return;

    save << gameMode << endl;
    save << gameOver << endl;

    for (int i = 0; i < 2; i++) {
        save << PLAYER[i].name << endl;
        save << PLAYER[i].score << ' ' << PLAYER[i].winStreak << ' ' << PLAYER[i].moveCount << endl;
    }
    
    save << curPlayer << endl;
    save << BOARD.curX << ' ' << BOARD.curY << endl;

    for (int i = 0; i < BOARD.BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD.BOARD_SIZE; j++) {
            save << BOARD.cells[i][j].logicMark << ' ';
        }
        save << endl;
    }

    save.close();

    if (newFile) GAME_MANAGER.addFile(filename, gameMode);
    else GAME_MANAGER.updateModifiedDate(filename);
}

void ClassicGame::loadGame(string fname) {
    ifstream load("saves/" + fname);
    if (!load) return;

    createStats(1440, 230, 260);
    resetData();

    load >> gameMode;
    load >> gameOver;
    load.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < 2; i++) {
        getline(load, PLAYER[i].name);
        load >> PLAYER[i].score >> PLAYER[i].winStreak;
        if(!gameOver) load >> PLAYER[i].moveCount;
        load.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        PLAYER[i].nameText.setString(PLAYER[i].name);
    }

    if (!gameOver) {
        load >> curPlayer;
        if (curPlayer == 0) BOARD.updateHighlight(COLOR.X);
        else BOARD.updateHighlight(COLOR.O);

        load >> BOARD.curX >> BOARD.curY;

        for (int i = 0; i < BOARD.BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD.BOARD_SIZE; j++) {
                load >> BOARD.cells[i][j].logicMark;
                if (BOARD.cells[i][j].logicMark == 'X') BOARD.cells[i][j].spriteMark.setTexture(*BOARD.Xmark.getTexture());
                if (BOARD.cells[i][j].logicMark == 'O') BOARD.cells[i][j].spriteMark.setTexture(*BOARD.Omark.getTexture());
            }
        }
    }


    load.close();

    filename = fname;
    runGame();
}

void ClassicGame::switchPlayer() {
    curPlayer ^= 1;
    if (curPlayer == 0) BOARD.updateHighlight(COLOR.X);
    else BOARD.updateHighlight(COLOR.O);
}

void ClassicGame::EventController() {
    sf::Event event;
    while (WINDOW.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            WINDOW.close();

        if (event.type == sf::Event::KeyPressed) {      
                if (event.key.code == sf::Keyboard::Escape) {// ESC = GAME GAME_OPTIONS
                    pause();
                    GAME_OPTIONS.openMenu(); 
                }
                if (event.key.code == sf::Keyboard::Enter) { // ENTER = PLACE MARK
                    if (BOARD.placeMark(PLAYER[curPlayer].mark, 1)) { // PLACE MARK SUCCESSFULLY
                        ++PLAYER[curPlayer].moveCount;
                        

                        if (BOARD.checkWin(PLAYER[curPlayer].mark, 1)) {
                            SOUND.sfxWin.play();
                            BOARD.isWin = true;


                            ++PLAYER[curPlayer].score;
                            ++PLAYER[curPlayer].winStreak;
                            PLAYER[curPlayer ^ 1].winStreak = 0;

                            stop();
                            GAME_OVER.openMenu();
                        }
                        else if (BOARD.checkDraw(1)) {
                            SOUND.sfxDraw.play();
                            BOARD.isDraw = true;

                            PLAYER[0].winStreak = PLAYER[1].winStreak = 0;

                            stop();
                            GAME_OVER.openMenu();
                        }
                        else switchPlayer();
                    }
                }

                // MOVE
                if (event.key.code == sf::Keyboard::W) { // UP
                    if (BOARD.isDrawingX || BOARD.isDrawingO) {
                        BOARD.isDrawingX = BOARD.isDrawingO = false;
                        BOARD.cells[BOARD.curX][BOARD.curY].visible = true;
                    }
                    BOARD.moveCursor(-1, 0);
                }
                if (event.key.code == sf::Keyboard::S) { // DOWN
                    if (BOARD.isDrawingX || BOARD.isDrawingO) {
                        BOARD.isDrawingX = BOARD.isDrawingO = false;
                        BOARD.cells[BOARD.curX][BOARD.curY].visible = true;
                    }
                    BOARD.moveCursor(1, 0); 
                }
                if (event.key.code == sf::Keyboard::A) { // LEFT
                    if (BOARD.isDrawingX || BOARD.isDrawingO) {
                        BOARD.isDrawingX = BOARD.isDrawingO = false;
                        BOARD.cells[BOARD.curX][BOARD.curY].visible = true;
                    }
                    BOARD.moveCursor(0, -1); 
                }
                if (event.key.code == sf::Keyboard::D) { // RIGHT
                    if (BOARD.isDrawingX || BOARD.isDrawingO) {
                        BOARD.isDrawingX = BOARD.isDrawingO = false;
                        BOARD.cells[BOARD.curX][BOARD.curY].visible = true;
                    }
                    BOARD.moveCursor(0, 1); 
                }
            }
    } 
}

void ClassicGame::apply() {
    WINDOW.clear(COLOR.BACKGROUND);
    WINDOW.draw(paper);
    BOARD.draw(0);
    drawStats(1440, 230, 260);
}


void ExtremeGame::createBoards() {
    BIGBOARD.create(600, 600, 3, 762, 246, 6, 3, TEXTURE.BigBoard, TEXTURE.XBigMark, TEXTURE.OBigMark);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            BOARD[i][j].create(BIGBOARD.cells[i][j].spriteMark.getPosition().x, BIGBOARD.cells[i][j].spriteMark.getPosition().y, 3, 252, 80, 3, 3, TEXTURE.MiniBoard, TEXTURE.XMiniMark, TEXTURE.OMiniMark);
}

void ExtremeGame::drawBoards() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            if (!BOARD[i][j].isDone) {
                if (BOARD[i][j].isDrawingX || BOARD[i][j].isDrawingO) BOARD[i][j].drawingMark();
                else if (BOARD[i][j].effectVisible) {
                    if (BOARD[i][j].isWin) BOARD[i][j].updateWinEffect();
                    if (BOARD[i][j].isDraw) BOARD[i][j].updateDrawEffect();
                }
                else if (BOARD[i][j].isWin || BOARD[i][j].isDraw) BOARD[i][j].startWaveEffect();

                BIGBOARD.cells[i][j].visible = false;
                BOARD[i][j].draw(0);
            }
            else if (!BIGBOARD.cells[i][j].visible) {
                BIGBOARD.cells[i][j].visible = true;
                if(BIGBOARD.isWin || BIGBOARD.isDraw) BIGBOARD.startWaveEffect();
            }
        }

    if(BIGBOARD.effectVisible)
        if (BIGBOARD.isWin) BIGBOARD.updateWinEffect();
        else BIGBOARD.updateDrawEffect();

    BIGBOARD.draw(0);

    if (!BOARD[curX / 3][curY / 3].isDone) BOARD[curX / 3][curY / 3].setHighlight();
    else BIGBOARD.setHighlight();

}
 
void ExtremeGame::moveCursor(int dx, int dy) {
    SOUND.sfxBoardMove.play();

    if (BOARD[curX / 3][curY / 3].isDone) {
        dx *= abs(2 - 2 * (curX % 3) + dx);
        dy *= abs(2 - 2 * (curY % 3) + dy);
    }

    if (0 <= curX + dx && curX + dx < BOARD_SIZE && 0 <= curY + dy && curY + dy < BOARD_SIZE) {
        curX += dx;
        curY += dy;
        BIGBOARD.curX = curX / 3;
        BIGBOARD.curY = curY / 3;
        BOARD[curX / 3][curY / 3].curX = curX % 3;
        BOARD[curX / 3][curY / 3].curY = curY % 3;
    }
}

void ExtremeGame::start() {
    createStats(1440, 230, 260);
    PLAYER_NAME.collectPlayerName();
    resetData();
    runGame();
}

void ExtremeGame::resetData() {
    curPlayer = 0;
    gameResult = -1;

    curX = BOARD_SIZE / 2;
    curY = BOARD_SIZE / 2;

    PLAYER[0].moveCount = PLAYER[1].moveCount = 0;
    if (!inSession) { 
        PLAYER[0].score = PLAYER[1].score = 0;
        PLAYER[0].winStreak = PLAYER[1].winStreak = 0;
        filename = "";
    }

    BIGBOARD.resetData();
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            BOARD[i][j].resetData();
}

void ExtremeGame::runGame() {
    inSession = true;
    inGame = true;
    gameOver = false;
    pausing = false;
    while (inGame) {
        WINDOW.clear(COLOR.BACKGROUND);
        WINDOW.draw(paper_background);
        if (!pausing) EventController();

        if (gameResult == -1 && BIGBOARD.isDone)
            if (BIGBOARD.isWin) gameResult = 1;
            else gameResult = 0;

        drawBoards();
        drawStats(1440, 230, 260);

        if (tearEffectRunning && virtualPaper.isDone) {
            virtualPaper.update();

            if (virtualPaper.isDone) {

                cap_part.setSize(sf::Vector2f(virtualPaper.zone_cap.width, virtualPaper.zone_cap.height));
                cap_part.setOrigin(virtualPaper.zone_cap.width / 2.0, virtualPaper.zone_cap.height / 2.0);
                cap_part.setPosition(
                    virtualPaper.zone_cap.left + virtualPaper.zone_cap.width / 2,
                    virtualPaper.zone_cap.top + virtualPaper.zone_cap.height / 2);
                cap_part.setTexture(&virtualPaper.paper_cap);
                cap_part.setTextureRect(sf::IntRect(
                    (int)virtualPaper.startX,
                    (int)virtualPaper.startY,
                    (int)virtualPaper.zone_cap.width,
                    (int)virtualPaper.zone_cap.height
                ));

                float rotation = sin(virtualPaper.total_time * 3) * 15.0;
                cap_part.setRotation(rotation);
                WINDOW.draw(cap_part);

            }

            if (virtualPaper.zone_cap.top > WINDOW.getSize().y + 100) {
                virtualPaper.reset();
                tearEffectRunning = false;
            }
        }

        if (GAME_OPTIONS.isOpen) {
            if (GAME_SAVER.isOpen) {
                GAME_SAVER.getFileName();
                GAME_SAVER.drawPopUpMenu();
            }
            else if (SETTING.isOpen) {
                SETTING.EventController();
                SETTING.drawPopUpMenu();
            }
            else if (GAME_DIALOG.isOpen) {
                GAME_DIALOG.EventController();
                GAME_DIALOG.drawPopUpMenu();
            }
            else if (HELP.isOpen) {
                HELP.EventController();
                HELP.draw();
            }
            else {
                GAME_OPTIONS.EventController();
                GAME_OPTIONS.drawPopUpMenu();
            }
        }

        if (BIGBOARD.isDone) {
            GAME_OVER.drawMenu();
            if (GAME_SAVER.isOpen) {
                GAME_SAVER.getFileName();
                GAME_SAVER.drawPopUpMenu();
            }
            else if (GAME_DIALOG.isOpen) {
                GAME_DIALOG.EventController();
                GAME_DIALOG.drawPopUpMenu();
            }
            else GAME_OVER.EventController();
        }
        WINDOW.display();
    }
}

void ExtremeGame::saveGame(bool newFile) {
    if (!filesystem::exists("saves")) {
        filesystem::create_directory("saves");
    }

    ofstream save("saves/" + filename);
    if (!save) return;

    save << gameMode << endl;
    save << gameOver << endl;

    for (int i = 0; i < 2; i++) {
        save << PLAYER[i].name << endl;
        save << PLAYER[i].score << ' ' << PLAYER[i].winStreak << ' ' << PLAYER[i].moveCount << endl;
    }

    save << curPlayer << endl;
    save << BIGBOARD.curX << ' ' << BIGBOARD.curY << endl;

    for (int i = 0; i < BIGBOARD.BOARD_SIZE; i++) {
        for (int j = 0; j < BIGBOARD.BOARD_SIZE; j++) {
            save << BIGBOARD.cells[i][j].logicMark << ' ';
        }
        save << endl;
    }

    for (int i = 0; i < BIGBOARD.BOARD_SIZE; i++) {
        for (int j = 0; j < BIGBOARD.BOARD_SIZE; j++) {
            save << BOARD[i][j].isWin << ' ' << BOARD[i][j].isDraw << ' ' << BOARD[i][j].isDone << endl;
            save << BOARD[i][j].curX << ' ' << BOARD[i][j].curY << endl;
            for (int x = 0; x < BOARD[i][j].BOARD_SIZE; x++) {
                for (int y = 0; y < BOARD[i][j].BOARD_SIZE; y++) {
                    save << BOARD[i][j].cells[x][y].logicMark << ' ';
                }
                save << endl;
            }
        }
    }

    save.close();

    if (newFile) GAME_MANAGER.addFile(filename, gameMode);
    else GAME_MANAGER.updateModifiedDate(filename);
}

void ExtremeGame::loadGame(string fname) {
    ifstream load("saves/" + fname);
    if (!load) return;

    createStats(1440, 230, 260);
    resetData();

    load >> gameMode;
    load >> gameOver;
    load.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < 2; i++) {
        getline(load, PLAYER[i].name);
        load >> PLAYER[i].score >> PLAYER[i].winStreak;
        if (!gameOver) load >> PLAYER[i].moveCount;
        load.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        PLAYER[i].nameText.setString(PLAYER[i].name);
    }

    if (!gameOver) {
        load >> curPlayer;

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (curPlayer == 0) BOARD[i][j].updateHighlight(COLOR.X);
                else BOARD[i][j].updateHighlight(COLOR.O);

        if (curPlayer == 0) BIGBOARD.updateHighlight(COLOR.X);
        else BIGBOARD.updateHighlight(COLOR.O);
        
        load >> BIGBOARD.curX >> BIGBOARD.curY;

        for (int i = 0; i < BIGBOARD.BOARD_SIZE; i++) {
            for (int j = 0; j < BIGBOARD.BOARD_SIZE; j++) {
                load >> BIGBOARD.cells[i][j].logicMark;
                if (BIGBOARD.cells[i][j].logicMark == 'X') BIGBOARD.cells[i][j].spriteMark.setTexture(*BIGBOARD.Xmark.getTexture());
                if (BIGBOARD.cells[i][j].logicMark == 'O') BIGBOARD.cells[i][j].spriteMark.setTexture(*BIGBOARD.Omark.getTexture());
            }
        }


        for (int i = 0; i < BIGBOARD.BOARD_SIZE; i++) {
            for (int j = 0; j < BIGBOARD.BOARD_SIZE; j++) {
                load >> BOARD[i][j].isWin >> BOARD[i][j].isDraw >> BOARD[i][j].isDone;
                load >> BOARD[i][j].curX >> BOARD[i][j].curY;
                for (int x = 0; x < BOARD[i][j].BOARD_SIZE; x++) {
                    for (int y = 0; y < BOARD[i][j].BOARD_SIZE; y++) {
                        load >> BOARD[i][j].cells[x][y].logicMark;
                        if (BOARD[i][j].cells[x][y].logicMark == 'X') BOARD[i][j].cells[x][y].spriteMark.setTexture(*BOARD[i][j].Xmark.getTexture());
                        if (BOARD[i][j].cells[x][y].logicMark == 'O') BOARD[i][j].cells[x][y].spriteMark.setTexture(*BOARD[i][j].Omark.getTexture());
                    }
                }
            }
        }
    }


    load.close();

    filename = fname;
    runGame();
}

void ExtremeGame::switchPlayer() {
    curPlayer ^= 1;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if (curPlayer == 0) BOARD[i][j].updateHighlight(COLOR.X);
            else BOARD[i][j].updateHighlight(COLOR.O);

    if (curPlayer == 0) BIGBOARD.updateHighlight(COLOR.X);
    else BIGBOARD.updateHighlight(COLOR.O);
}

void ExtremeGame::EventController() {
    sf::Event event;
    while (WINDOW.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            WINDOW.close();

        if (event.type == sf::Event::KeyPressed) {
            // DO
            if (event.key.code == sf::Keyboard::Escape) {// ESC = GAME GAME_OPTIONS
                pause();
                GAME_OPTIONS.openMenu();
            }

            Board& curSubBoard = BOARD[curX / 3][curY / 3];

            if (event.key.code == sf::Keyboard::Enter) { // ENTER = PLACE MARK
                if (curSubBoard.isDone) continue;

                if (curSubBoard.placeMark(PLAYER[curPlayer].mark, 1)) { // PLACE MARK SUCCESSFULLY
                    ++PLAYER[curPlayer].moveCount;
                    if (curSubBoard.checkWin(PLAYER[curPlayer].mark, 1)) {
                        SOUND.sfxWin.play();
                        curSubBoard.isWin = true;
                        

                        BIGBOARD.placeMark(PLAYER[curPlayer].mark, 0);
                        if (BIGBOARD.checkWin(PLAYER[curPlayer].mark, 1)){
                            SOUND.sfxWin.play();
                            BIGBOARD.isWin = true;

                            ++PLAYER[curPlayer].score;
                            ++PLAYER[curPlayer].winStreak;
                            PLAYER[curPlayer ^ 1].winStreak = 0;
                            
                            stop();
                            GAME_OVER.openMenu();
                        }
                        else if (BIGBOARD.checkDraw(1)) {
                            SOUND.sfxDraw.play();
                            BIGBOARD.isDraw = true;

                            PLAYER[curPlayer].winStreak = 0;
                            PLAYER[curPlayer ^ 1].winStreak = 0;
                            
                            stop();
                            GAME_OVER.openMenu();
                        }
                        else switchPlayer();
                    }
                    else if (curSubBoard.checkDraw(1)) {
                        SOUND.sfxDraw.play();
                        BIGBOARD.placeMark('-', 0);
                        curSubBoard.isDraw = true;

                        if (BIGBOARD.checkDraw(1)) {
                            SOUND.sfxDraw.play();

                            PLAYER[curPlayer].winStreak = 0;
                            PLAYER[curPlayer ^ 1].winStreak = 0;
                            
                            stop();
                            GAME_OVER.openMenu();
                        }
                        else switchPlayer();
                    }
                    else switchPlayer();
                }
            }
            // MOVE
            if (event.key.code == sf::Keyboard::W) { // UP
                if (curSubBoard.isDrawingX || curSubBoard.isDrawingO) {
                    curSubBoard.isDrawingX = curSubBoard.isDrawingO = false;
                    curSubBoard.cells[curX % 3][curY % 3].visible = true;
                }
                moveCursor(-1, 0);
            }
            if (event.key.code == sf::Keyboard::S) { // DOWN
                if (curSubBoard.isDrawingX || curSubBoard.isDrawingO) {
                    curSubBoard.isDrawingX = curSubBoard.isDrawingO = false;
                    curSubBoard.cells[curX % 3][curY % 3].visible = true;
                }
               moveCursor(1, 0);
            }
            if (event.key.code == sf::Keyboard::A) { // LEFT
                if (curSubBoard.isDrawingX || curSubBoard.isDrawingO) {
                    curSubBoard.isDrawingX = curSubBoard.isDrawingO = false;
                    curSubBoard.cells[curX % 3][curY % 3].visible = true;
                }
                moveCursor(0, -1);
            }
            if (event.key.code == sf::Keyboard::D) { // RIGHT
                if (curSubBoard.isDrawingX || curSubBoard.isDrawingO) {
                    curSubBoard.isDrawingX = curSubBoard.isDrawingO = false;
                    curSubBoard.cells[curX % 3][curY % 3].visible = true;
                }
                moveCursor(0, 1);
            }
        }
    }
}

void ExtremeGame::apply() {
    WINDOW.clear(COLOR.BACKGROUND);
    WINDOW.draw(paper);
    drawBoards();
    drawStats(1440, 230, 260);
}



