#ifndef PLAYER_H
#define PLAYER_H

struct Player{
    string name;
    char mark;
    int score, winStreak, moveCount;
    sf::Text nameText;
    pair<Label, sf::Text> scoreText, winStreakText, moveCountText;
    sf::RectangleShape statsBox, winBox;
    sf::Color color;
    sf::Vector2f offsetName;
    pair<sf::Vector2f, sf::Vector2f> offsetScore, offsetWinStreak, offsetMoveCount;
    Player() {}

    Player(char m, sf::Color c, sf::Texture& sb, sf::Texture& wb) {
        mark = m;

        statsBox.setSize(sf::Vector2f(530, 300));
        statsBox.setOrigin(265, 150);
        statsBox.setTexture(&sb);
        
        winBox.setSize(sf::Vector2f(530, 300));
        winBox.setOrigin(265, 150);
        winBox.setTexture(&wb);
        winBox.setScale(1.1, 1.1);

        color = c;

        nameText = Label("", 25, color);
        scoreText.first = Label("scores", 20, color);
        winStreakText.first = Label("win streak", 20, color);
        moveCountText.first = Label("move count", 20, color);
    }
};

#endif