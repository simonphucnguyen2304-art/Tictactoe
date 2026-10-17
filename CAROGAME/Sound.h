#ifndef SOUND_H
#define SOUND_H
#include "Global.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;

struct SoundManager {
    sf::Music themeMusic;

    sf::SoundBuffer bufferMenuMove;
    sf::SoundBuffer bufferMenuSelect;
    sf::SoundBuffer bufferBoardMove;
    sf::SoundBuffer bufferWin;
    sf::SoundBuffer bufferDraw;
    sf::SoundBuffer bufferXPlaceMark;
    sf::SoundBuffer bufferOPlaceMark;
    sf::SoundBuffer bufferOof;
    sf::SoundBuffer bufferTear;

    sf::Sound sfxMenuMove;
    sf::Sound sfxMenuSelect;
    sf::Sound sfxBoardMove;
    sf::Sound sfxWin;
    sf::Sound sfxDraw;
    sf::Sound sfxXPlaceMark;
    sf::Sound sfxOPlaceMark;
    sf::Sound sfxOof;
    sf::Sound sfxTear;

    vector<sf::Sound*> sfxList;

    void setMusicVolume(float vol);
    void setSoundVolume(float vol);

    SoundManager() : 
        sfxList{ &sfxMenuMove, &sfxMenuSelect, &sfxBoardMove, &sfxXPlaceMark, &sfxOPlaceMark, &sfxWin, &sfxDraw, &sfxTear }
    {
        themeMusic.openFromFile("audio/theme_music.wav");
        themeMusic.setLoop(true);

        bufferMenuMove.loadFromFile("audio/menu_move.wav");
        sfxMenuMove.setBuffer(bufferMenuMove);

        bufferMenuSelect.loadFromFile("audio/menu_select.wav");
        sfxMenuSelect.setBuffer(bufferMenuSelect);

        bufferBoardMove.loadFromFile("audio/board_move.wav");
        sfxBoardMove.setBuffer(bufferBoardMove);

        bufferWin.loadFromFile("audio/game_win.wav");
        sfxWin.setBuffer(bufferWin);

        bufferDraw.loadFromFile("audio/game_draw.wav");
        sfxDraw.setBuffer(bufferDraw);

        bufferOPlaceMark.loadFromFile("audio/O_place_mark.wav");
        sfxOPlaceMark.setBuffer(bufferOPlaceMark);

        bufferXPlaceMark.loadFromFile("audio/X_place_mark.wav");
        sfxXPlaceMark.setBuffer(bufferXPlaceMark);

        bufferOof.loadFromFile("audio/Oof.wav");
        sfxOof.setBuffer(bufferOof);

        bufferTear.loadFromFile("audio/tear.wav");
        sfxTear.setBuffer(bufferTear);

        setMusicVolume(50.0);
        setSoundVolume(50.0);
    }


};

#endif
