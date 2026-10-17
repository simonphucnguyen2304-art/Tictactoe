#ifndef GAME_H
#define GAME_H
#include "Global.h"
#include "UI.h"
#include "Board.h"
#include "Player.h"
#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;

struct GameController;

struct GameOption : Menu {
    GameController* controller = nullptr;
    void selectOption() override;   

    GameOption() {
        createPanel(1015, 941, WINDOW.WIDTH / 2.0 + 15, WINDOW.HEIGHT / 2.0, TEXTURE.OptionsPanel);
        createMenu(WINDOW.WIDTH / 2.0, 380, 110, { "SAVE GAME", "SETTINGS", "HELP", "QUIT GAME", "CLOSE" });
    }
};

struct GameOver : Menu {
    GameController* controller = nullptr;
    void selectOption() override;

    GameOver() {
        createMenu(1440, 770, 110, { "NEW GAME", "SAVE GAME", "QUIT GAME" });
    }
};

struct GameMode : Menu {
    GameController* controller = nullptr;
    void selectOption() override;
    void chooseGameMode();

    GameMode() {
        createTitle(780, 325, WINDOW.WIDTH / 2.0, 180, TEXTURE.Title);
        createMenu(WINDOW.WIDTH / 2.0, 450, 110, { "PVP", "PVE" });
    }
};

struct GameSaver : DialogMenu {
    GameController* controller = nullptr;
    bool wantQuit;
    void selectOption() override;
    void getFileName();
    string getCurrentDateTime();
    GameSaver() {
        createPanel(900, 450, WINDOW.WIDTH / 2.0 + 15, WINDOW.HEIGHT / 2.0, TEXTURE.SavePanel);
        createTextBoxes(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0, 0, { make_pair("FILE NAME", "")}, 20);
        createButtons(WINDOW.WIDTH / 2.0 - 200, WINDOW.HEIGHT / 2.0 + 100, 400, { "SAVE", "CANCEL"});
    }
};

struct GameDialog : DialogMenu {
    GameController* controller = nullptr;
    void selectOption() override;

    GameDialog() {
        createPanel(900, 450, WINDOW.WIDTH / 2.0 + 15, WINDOW.HEIGHT / 2.0, TEXTURE.SavePanel);
        createMessage(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0, "Save your current game before quitting?");
        createButtons(WINDOW.WIDTH / 2.0 - 200, WINDOW.HEIGHT / 2.0 + 100, 200, { "YES", "NO", "CANCEL" });
    }
};

struct PlayerNameInput : DialogMenu {
    GameController* controller = nullptr;
    void selectOption() override;
    void collectPlayerName();

    PlayerNameInput() {
        createTitle(780, 325, WINDOW.WIDTH / 2.0, 180, TEXTURE.Title);
        createPanel(900, 600, WINDOW.WIDTH / 2.0 + 15, WINDOW.HEIGHT / 2.0 + 150, TEXTURE.NameInputPanel);
        createTextBoxes(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0 + 90, 160, { make_pair("PLAYER 1'S NAME", "Player 1"), make_pair("PLAYER 2'S NAME", "Player 2") }, 15);
        createButtons(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0 + 350, 0, { "PLAY" });
        createMessage(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0 - 40, "ENTER PLAYERS' NAME!");
    }

};

struct GameController {
    GameOption GAME_OPTIONS;
    GameOver GAME_OVER;
    GameSaver GAME_SAVER;
    GameDialog GAME_DIALOG;
    PlayerNameInput PLAYER_NAME;
    Player PLAYER[2];
    string filename;
    bool inGame;
    bool inSession;
    bool gameOver;
    bool pausing;
    int curPlayer;
    int gameMode;
    int gameResult;
    bool tearEffectRunning;
    sf::RectangleShape paper_background;
    sf::RectangleShape paper;

    EffectPaper virtualPaper;
    sf::RectangleShape cap_part;
    
    void pause();
    void resume();
    void quit();
    void stop();
    void openDialog();
    void drawStats(float, float, float);
    void createStats(float, float, float);
    void setGameMode(int);
    virtual void start();
    virtual void runGame();
    virtual void saveGame(bool);
    virtual void loadGame(string);
    virtual void EventController();
    virtual void switchPlayer();
    virtual void resetData();
    virtual void apply();
    virtual void reset();


    GameController() {
        paper_background.setSize(sf::Vector2f(WINDOW.WIDTH, WINDOW.HEIGHT));
        paper_background.setOrigin(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0);
        paper_background.setTexture(&TEXTURE.PaperBackground);
        paper_background.setPosition(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0);

        paper.setSize(sf::Vector2f(WINDOW.WIDTH, WINDOW.HEIGHT));
        paper.setOrigin(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0);
        paper.setTexture(&TEXTURE.Paper);
        paper.setPosition(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0);
    }
};

struct Move {
    int x, y, score;

    bool operator < (const Move& other) {
        return score > other.score;
    }
};

struct Bot {
    GameController* controller = nullptr;
    Board* board = nullptr;
    const int maxDepth = 5;
    const int inf = 0x3f3f3f3f;
    const int maxRange = 2;
    int curX, curY;
    bool vis[15][15];

    void makeMove();
    void getCandidateMoves(vector<Move>*);
    int evaluateBoard(int);
    int minimax(int, int, int, int);

    Bot() {}
};

struct ClassicGame : GameController{
    Bot BOT;
    GameMode GAME_MODE;
    Board BOARD;

    void start() override;
    void switchPlayer() override;
    void drawBoard();
    void resetData() override;
    void runGame() override;
    void saveGame(bool) override;
    void loadGame(string) override;
    void EventController() override;
    void apply() override;

    ClassicGame() {
        BOT.controller = this;
        BOT.board = &BOARD;
        GAME_OPTIONS.controller = this;
        GAME_OVER.controller = this;
        GAME_MODE.controller = this;
        GAME_SAVER.controller = this;
        GAME_DIALOG.controller = this;
        PLAYER_NAME.controller = this;
        BOARD.create(600, 600, 15, 898, 50, 3, 5, TEXTURE.Board, TEXTURE.XMark, TEXTURE.OMark);
    }
};

struct ExtremeGame : GameController{
    const int BOARD_SIZE = 9;
    int curX, curY;
    Board BOARD[3][3], BIGBOARD;
    
    void createBoards();
    void drawBoards();
    void moveCursor(int, int);
    void start() override;
    void resetData() override;
    void runGame() override;
    void saveGame(bool) override;
    void loadGame(string) override;
    void switchPlayer() override;
    void EventController() override;
    void apply() override;

    ExtremeGame() {
        GAME_OPTIONS.controller = this;
        GAME_OVER.controller = this;
        GAME_SAVER.controller = this;
        GAME_DIALOG.controller = this;
        PLAYER_NAME.controller = this;
        gameMode = 2;
        createBoards();
    }
};

struct GameType : Menu {
    void selectOption() override;
    void chooseGameType();

    GameType() {
        createTitle(780, 325, WINDOW.WIDTH / 2.0, 180, TEXTURE.Title);
        createMenu(WINDOW.WIDTH / 2.0, 450, 110, { "CLASSIC", "EXTREME" });
    }
};

struct File {
    Button box;
    Label nameText, modeText, modifiedTimeText;
    int mode;


    File(string name, int m) {
        box = Button(970, 120, TEXTURE.FileBox, TEXTURE.FileBoxHovered);
        nameText = Label(name, 40, COLOR.PRIMARY);
        mode = m;
        if (mode == 0) modeText = Label("PvP", 40, COLOR.PRIMARY);
        else if (mode == 1) modeText = Label("PvE", 40, COLOR.PRIMARY);
        else modeText = Label("Extreme", 40, COLOR.PRIMARY);
        
    }
};

struct GameManager : DialogMenu {
    vector<File> files;
    int numFiles;
    int L, R;

    void selectOption() override;
    void moveCursor(int, int) override;
    void resetData() override;
    void getFiles();
    void addFile(string, int);
    void deleteFile();
    void updateModifiedDate(string);
    void drawFileInfo(float, float, float);
    void draw();

    GameManager() {
        createPanel(1400, 940, WINDOW.WIDTH / 2.0 + 15, WINDOW.HEIGHT / 2.0, TEXTURE.LoadPanel);
        createButtons(WINDOW.WIDTH / 2.0 - 300, WINDOW.HEIGHT / 2.0 + 300, 300, { "LOAD", "DELETE", "CLOSE" });
        getFiles();
    }
};

#endif
