#include "Global.h"
#include "Sound.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
using namespace std;

void SoundManager::setMusicVolume(float vol) {
	themeMusic.setVolume(vol);
}

void SoundManager::setSoundVolume(float vol) {
	for (sf::Sound* s : sfxList)
		s->setVolume(vol);
}