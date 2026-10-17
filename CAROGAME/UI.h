#ifndef UI_H
#define UI_H
#include "Global.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
using namespace std; 

struct Window : public sf::RenderWindow {
	const float WIDTH = 1920;
	const float HEIGHT = 1080;
	Window() :
		sf::RenderWindow(sf::VideoMode(1920, 1080), "Caro Game - Group 6", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)
	{}
};

struct FontManager{
	sf::Font Main;

	FontManager() {
		Main.loadFromFile("font/Borders Divide, But Hearts Shall Conquer.ttf");
	}
};

struct TextureManager {
	sf::Texture Title;
	sf::Texture Button, ButtonHovered, MiniButton, MiniButtonHovered;
	sf::Texture SettingsPanel, HelpPanel, AboutPanel, OptionsPanel, SavePanel, LoadPanel, NameInputPanel;
	sf::Texture Board, MiniBoard, BigBoard;
	sf::Texture XMark, OMark, XMiniMark, OMiniMark, XBigMark, OBigMark;
	sf::Texture XLine1, XLine2, XMiniLine1, XMiniLine2;
	sf::Texture XStatsBox, OStatsBox;
	sf::Texture XWinBox, OWinBox;
	sf::Texture SoundOn, SoundOnHovered;
	sf::Texture SoundOff, SoundOffHovered;
	sf::Texture TextBox, TextBoxHovered;
	sf::Texture FileBox, FileBoxHovered;
	sf::Texture Thumb;
	sf::Texture Rail, RailHovered;
	sf::Texture Progress;
	sf::Texture Color0Dot, Color0DotHovered, Color1Dot, Color1DotHovered;
	sf::Texture Paper, PaperBackground;
	sf::Texture MoveKey, Enter, Escape;

	int curTheme;

	void switchTheme();
	void setTheme();

	TextureManager() {
		curTheme = 0;
		setTheme();
	}
};

struct Color {
	sf::Color PRIMARY;
	sf::Color SECONDARY;
	sf::Color BACKGROUND;
	sf::Color WHITE;
	sf::Color ORANGE, BLUE;
	sf::Color BROWN, NAVY;
	sf::Color X, O;
	int curMode;

	void switchMode();

	Color() :
		curMode(0),
		ORANGE(239, 128, 108),
		BLUE(106, 178, 239),
		BROWN(139, 70, 58),
		NAVY(54, 103, 144),
		PRIMARY(239, 128, 108),
		SECONDARY(139, 70, 58),
		//BACKGROUND(255, 246, 227),
		BACKGROUND(252, 237, 237),
		WHITE(255, 255, 255),
		X(172, 43, 33),
		O(33, 89, 172)
	{}
};

struct Button : public sf::RectangleShape {
	float width;
	float height;
	sf::Texture *button, *buttonHovered;

	Button(float w, float h, sf::Texture &btn, sf::Texture &btnHovered) :
		width(w),
		height(h),
		button(&btn),
		buttonHovered(&btnHovered)
	{	
		setSize(sf::Vector2f(w, h));
		setOrigin(w / 2.0, h / 2.0);
	}

	Button() {}
		
	void setNormalState();
	void setHoveredState();
	void setPressedState();
};

struct Label : public sf::Text {
	Label() {
		setFont(FONT.Main);
		setString("");
		setFillColor(COLOR.PRIMARY);
		setCharacterSize(40);
		setStyle(sf::Text::Bold);
	}

	Label(string str, double sz, sf::Color c) {
		setFont(FONT.Main);
		setString(str);
		setFillColor(c);
		setCharacterSize(sz);
		setStyle(sf::Text::Bold);
		sf::FloatRect bounds = sf::Text::getGlobalBounds();
		setOrigin(bounds.left, bounds.top + bounds.height / 2.0);
	}

	void setNormalState();
	void setHoveredState();
};

struct Elements : public sf::Sprite{
	sf::Texture *element, *elementHovered;
	float width, height;

	void setNormalState();
	void setHoveredState();

	Elements(float w, float h, sf::Texture& elem, sf::Texture& elemHovered) :
		width(w),
		height(h),
		element(&elem),
		elementHovered(&elemHovered)
	{
		setTexture(*element);
		setOrigin(w / 2.0, h / 2.0);
	}

	Elements() {}
};

struct TextBox{
	const float width = 700;
	const float height = 70;
	bool isTyping;
	string input;
	string defaultText;
	int cntChar;
	int limitChar;
	sf::Text text, countChar, title;
	sf::RectangleShape box;
	sf::RectangleShape caret;
	sf::Clock caretTimer;
	bool caretVisible;

	void createBox(float, float, string, string, int);
	void updateCaret();
	void setNormalState();
	void setHoveredState();
	void draw();
	void read();

	TextBox() {
		isTyping = false;
		cntChar = 0;

		box.setSize(sf::Vector2f(width, height));
		box.setOrigin(width / 2.0, height / 2.0);
		box.setFillColor(COLOR.WHITE);
		box.setTexture(&TEXTURE.TextBox);

		title.setFont(FONT.Main);
		title.setCharacterSize(30);
		title.setFillColor(COLOR.PRIMARY);
		title.setStyle(sf::Text::Bold);

		text.setFont(FONT.Main);
		text.setCharacterSize(30);
		text.setFillColor(COLOR.BROWN);

		countChar.setFont(FONT.Main);
		countChar.setCharacterSize(30);
		countChar.setFillColor(COLOR.PRIMARY);
		countChar.setStyle(sf::Text::Bold);

		caret.setSize({ 2, 40 });
		caret.setFillColor(COLOR.BROWN);
		caret.setOrigin(caret.getSize().x / 2.0, caret.getSize().y / 2.0);
		caretVisible = true;
		caretTimer.restart();
	}
};

struct Slider {
	Elements rail;
	Elements progress;
	Elements thumb;
	
	int value;
	sf::Vector2f offset;

	void create(float, float);
	void update(int);
	void setNormalState();
	void setHoveredState();
	void draw();

	Slider() :
		value(40)
	{
		rail = Elements(400, 100, TEXTURE.Rail, TEXTURE.RailHovered);
		progress = Elements(400, 100, TEXTURE.Progress, TEXTURE.Progress);
		thumb = Elements(35, 35, TEXTURE.Thumb, TEXTURE.Thumb);
	}
};

struct EffectPaper {
	sf::FloatRect zone_cap{ 90.f, 100.f, 1700.f, 1080.f };
	sf::RectangleShape cut;
	float startX = 90;
	float startY = 70;

	float total_time = 0;
	bool isDone = false;
	const float speed = 800;

	sf::Clock clock;
	sf::Texture paper_cap;

	void triggerCut();
	void update();
	void reset();
};


#endif
