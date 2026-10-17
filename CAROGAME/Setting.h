#ifndef SETTING_H
#define SETTING_H

#include "Global.h"
#include "UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>


struct Setting{
	bool isOpen;
	int curChoice;
	const int numOptions = 6;
	const float buttonWidth = 150;
	const float buttonHeight = 75;

	sf::RectangleShape panel;
	vector <Slider> sliders;
	vector <Label> labels;
	vector <Button> buttons;
	vector<vector<Elements>> icons;
	vector <int> states;

	void createPanel(float, float, float, float, sf::Texture&);
	void createMenu(float, float, float, vector<string>);
	void switchState();
	void selectOption();
	void moveCursor(int);
	void EventController();
	void drawMenu();
	void drawPopUpMenu();
	void open();
	void close();

	Setting() {
		createPanel(1015, 941, WINDOW.WIDTH / 2.0 + 15, WINDOW.HEIGHT / 2.0, TEXTURE.SettingsPanel);
		createMenu(WINDOW.WIDTH / 2.0, WINDOW.HEIGHT / 2.0 - 200, 200, { "MUSIC", "SFX", "THEME", "CLOSE" });
	}
};
 
#endif SETTING_H
