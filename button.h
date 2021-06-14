#pragma once

#include<iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
using namespace std;

enum button_states { BTN_IDLE = 0, BTN_HOVER, BTN_PRESSED,BIN_RELEASED };

class button
{
private:
	short unsigned buttonstate;

	sf::RectangleShape shape;
	sf::CircleShape shape2;
	sf::Font* font;
	sf::Text text;
	sf::Text byline;
	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;

	int ex;
public:
	button(float x,float y,float width,float height,
		sf::Font* font,sf::Font font2,string text,
		sf::Color idleColor, sf::Color hoverColor, sf::Color ActiveColor,int choice = 0);
	~button();
	const bool isPressed() const;
	void settext(std::string t);
	void update(const sf::Vector2f mousePos,sf::RenderWindow& win);
	void render(sf::RenderTarget* target);

};

