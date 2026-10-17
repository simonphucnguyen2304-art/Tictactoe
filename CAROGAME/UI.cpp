#include "UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <string>
#include <vector>

void TextureManager::switchTheme() {
	curTheme ^= 1;
	setTheme();
}

void TextureManager::setTheme() {
	string id = to_string(curTheme);
	Title.loadFromFile("assets/theme" + id + "/title.png");
	Button.loadFromFile("assets/theme" + id + "/button.png");
	ButtonHovered.loadFromFile("assets/theme" + id + "/button_hovered.png");
	MiniButton.loadFromFile("assets/theme" + id + "/mini_button.png");
	MiniButtonHovered.loadFromFile("assets/theme" + id + "/mini_button_hovered.png");
	SettingsPanel.loadFromFile("assets/theme" + id + "/settings_panel.png");
	OptionsPanel.loadFromFile("assets/theme" + id + "/options_panel.png");
	HelpPanel.loadFromFile("assets/theme" + id + "/help_panel.png");
	AboutPanel.loadFromFile("assets/theme" + id + "/about_panel.png");
	Board.loadFromFile("assets/theme" + id + "/board.png");
	MiniBoard.loadFromFile("assets/theme" + id + "/mini_board.png");
	BigBoard.loadFromFile("assets/theme" + id + "/big_board.png");
	XMark.loadFromFile("assets/theme" + id + "/X.png");
	OMark.loadFromFile("assets/theme" + id + "/O.png");
	XMiniMark.loadFromFile("assets/theme" + id + "/X_mini.png");
	OMiniMark.loadFromFile("assets/theme" + id + "/O_mini.png");
	XBigMark.loadFromFile("assets/theme" + id + "/X_big.png");
	OBigMark.loadFromFile("assets/theme" + id + "/O_big.png");
	XLine1.loadFromFile("assets/theme" + id + "/X_line1.png");
	XLine2.loadFromFile("assets/theme" + id + "/X_line2.png");
	XMiniLine1.loadFromFile("assets/theme" + id + "/X_mini_line1.png");
	XMiniLine2.loadFromFile("assets/theme" + id + "/X_mini_line2.png");
	XStatsBox.loadFromFile("assets/theme" + id + "/X_stats_box.png");
	OStatsBox.loadFromFile("assets/theme" + id + "/O_stats_box.png");
	XWinBox.loadFromFile("assets/theme" + id + "/X_win_box.png");
	OWinBox.loadFromFile("assets/theme" + id + "/O_win_box.png");
	SoundOn.loadFromFile("assets/theme" + id + "/sound_on.png");
	SoundOff.loadFromFile("assets/theme" + id + "/sound_off.png");
	SoundOnHovered.loadFromFile("assets/theme" + id + "/sound_on_hovered.png");
	SoundOffHovered.loadFromFile("assets/theme" + id + "/sound_off_hovered.png");
	TextBox.loadFromFile("assets/theme" + id + "/textbox.png");
	TextBoxHovered.loadFromFile("assets/theme" + id + "/textbox_hovered.png");
	FileBox.loadFromFile("assets/theme" + id + "/file_box.png");
	FileBoxHovered.loadFromFile("assets/theme" + id + "/file_box_hovered.png");
	SavePanel.loadFromFile("assets/theme" + id + "/save_panel.png");
	LoadPanel.loadFromFile("assets/theme" + id + "/load_panel.png");
	NameInputPanel.loadFromFile("assets/theme" + id + "/name_input_panel.png");
	Thumb.loadFromFile("assets/theme" + id + "/thumb.png");
	Rail.loadFromFile("assets/theme" + id + "/rail.png");
	RailHovered.loadFromFile("assets/theme" + id + "/rail_hovered.png");
	Progress.loadFromFile("assets/theme" + id + "/progress.png");
	Color0Dot.loadFromFile("assets/theme" + id + "/color0_dot.png");
	Color0DotHovered.loadFromFile("assets/theme" + id + "/color0_dot_hovered.png");
	Color1Dot.loadFromFile("assets/theme" + id + "/color1_dot.png");
	Color1DotHovered.loadFromFile("assets/theme" + id + "/color1_dot_hovered.png");
	PaperBackground.loadFromFile("assets/theme" + id + "/paper_background.png");
	Paper.loadFromFile("assets/theme" + id + "/paper_no_spring.png");
	MoveKey.loadFromFile("assets/theme" + id + "/move_key.png");
	Enter.loadFromFile("assets/theme" + id + "/enter.png");
	Escape.loadFromFile("assets/theme" + id + "/escape.png");
}


void Color::switchMode() {
	curMode ^= 1;

	if (curMode == 0) {
		PRIMARY = ORANGE;
		SECONDARY = BROWN;
		BACKGROUND = sf::Color(252, 237, 237);
	}
	else {
		PRIMARY = BLUE;
		SECONDARY = NAVY;
		BACKGROUND = sf::Color(225, 244, 252);
	}
}



void Button::setNormalState() {
	setTexture(button);
	setScale(1.0, 1.0);
}

void Button::setHoveredState() {
	setTexture(buttonHovered);
	setScale(1.1, 1.1);
}


void Label::setNormalState() {
	setFillColor(COLOR.PRIMARY);
	setScale(1.0, 1.0);
}

void Label::setHoveredState() {
	setFillColor(COLOR.WHITE);
	setScale(1.1, 1.1);
}


void Elements::setNormalState() {
	setTexture(*element);
	setScale(1.0, 1.0);
}

void Elements::setHoveredState() {
	setTexture(*elementHovered);
	setScale(1.1, 1.1);
}


void TextBox::createBox(float x, float y, string tit, string default_text, int lim) {
	defaultText = default_text;
	limitChar = lim;

	box.setPosition(x, y);
	
	title.setString(tit);
	title.setPosition(box.getPosition().x - box.getSize().x / 2.0, box.getPosition().y - box.getSize().y / 2.0 - 40);

	
	text.setString(defaultText);
	text.setPosition(box.getPosition().x - box.getSize().x / 2.0 + 20, box.getPosition().y - 15);

	countChar.setPosition(box.getPosition().x + box.getSize().x / 2.0 - 80, box.getPosition().y - box.getSize().y / 2.0 - 40);
}

void TextBox::read() {
	sf::Event event;
	while (WINDOW.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			WINDOW.close();

		if (event.type == sf::Event::KeyPressed)
			if (event.key.code == sf::Keyboard::Enter) // ENTER = FINISH TYPING
				isTyping = false; 

		if (event.type == sf::Event::TextEntered) {
			char c = event.text.unicode;
			if (isTyping && c == 8) { // BACKSPACE
				if (!input.empty()) {
					input.pop_back();
					--cntChar;
				}
			}
			else if (isTyping && c >= 32 && c <= 126 && input.size() < limitChar) {
				input += c;
				++cntChar;
			}
		}
	}
}

void TextBox::updateCaret() {
	if (caretTimer.getElapsedTime().asMilliseconds() > 500) {
		caretVisible ^= 1;
		caretTimer.restart();
	}

	sf::FloatRect bounds = text.getGlobalBounds();
	caret.setPosition(box.getPosition().x - box.getSize().x / 2.0 + bounds.width + 25, box.getPosition().y);
}
	
void TextBox::draw() {
	title.setFillColor(COLOR.PRIMARY);
	countChar.setFillColor(COLOR.PRIMARY);
	text.setFillColor(COLOR.SECONDARY);
	caret.setFillColor(COLOR.SECONDARY);

	WINDOW.draw(title);
	WINDOW.draw(box);
	if (!isTyping && input.empty()) text.setString(defaultText);
	else text.setString(input);
	WINDOW.draw(text);

	countChar.setString(to_string(cntChar) + "/" + to_string(limitChar));
	if(isTyping) WINDOW.draw(countChar);

	updateCaret();
	if(isTyping && caretVisible) WINDOW.draw(caret);
}

void TextBox::setNormalState() {
	box.setTexture(&TEXTURE.TextBox);
}

void TextBox::setHoveredState() {
	box.setTexture(&TEXTURE.TextBoxHovered);
}



void Slider::create(float x, float y) {
	rail.setPosition(x, y);
	progress.setPosition(x, y);
	update(40);
}

void Slider::update(int newValue) {
	value = newValue;

	if (value < 0) value = 0;
	if (value > 80) value = 80;

	setNormalState();

	sf::FloatRect bounds = rail.getGlobalBounds();
	float x = bounds.left;
	float y = bounds.top;

	if (value < 20) {
		thumb.setPosition(x + value * 5 + 18, y + 70);
		progress.setTextureRect(sf::IntRect(0, 0, value * 5 + 18, 100));
	}
	else if (value < 40) {
		thumb.setPosition(x + value * 5 + 18 - 5, y + 70);
		progress.setTextureRect(sf::IntRect(0, 0, value * 5 + 18 - 5, 100));
	}
	else if (value < 70) {
		thumb.setPosition(x + value * 5 + 18 - 15, y + 70);
		progress.setTextureRect(sf::IntRect(0, 0, value * 5 + 18 - 15, 100));
	}
	else if (value < 80) {
		thumb.setPosition(x + value * 5 + 18 - 25, y + 70);
		progress.setTextureRect(sf::IntRect(0, 0, value * 5 + 18 - 25, 100));
	}
	else {
		thumb.setPosition(x + value * 5 + 18 - 30, y + 70);
		progress.setTextureRect(sf::IntRect(0, 0, value * 5 + 18 - 30, 100));
	}

	offset = rail.getInverseTransform().transformPoint(thumb.getPosition());
}

void Slider::setNormalState() {
	rail.setNormalState();
	progress.setNormalState();
	thumb.setNormalState();
	thumb.setPosition(rail.getTransform().transformPoint(offset));
}

void Slider::setHoveredState() {
	rail.setHoveredState();
	progress.setHoveredState();
	thumb.setHoveredState();
	thumb.setPosition(rail.getTransform().transformPoint(offset));
}

void Slider::draw() {
	WINDOW.draw(rail);
	WINDOW.draw(progress);
	WINDOW.draw(thumb);
}


void EffectPaper::triggerCut() {
	if (!isDone) {
		isDone = true;

		sf::Vector2u window_size = WINDOW.getSize();
		paper_cap.create(WINDOW.WIDTH, WINDOW.HEIGHT);
		paper_cap.update(WINDOW);
		clock.restart();
	}
}

void EffectPaper::update() {
	float dt = clock.restart().asSeconds();

	if (isDone) {
		total_time += dt;
		zone_cap.top += (dt * speed);
		float sway = cos(total_time * 3.0);
		float dist = 80 * sway;
		zone_cap.left = startX + dist;
	}
}

void EffectPaper::reset() {
	isDone = false;
	zone_cap.top = startY;
	zone_cap.left = startX;
	total_time = 0;
}