#pragma once
#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class messagebox
{
public:
	messagebox(float x,float y,string mes)
	{
		sf::RenderWindow window(sf::VideoMode(400, 50), "Message Box");
		window.setPosition(sf::Vector2i(x,y));
		sf::Text text;
		sf::Font font;
		font.loadFromFile("arial.ttf");
		text.setFont(font); 
		text.setString(mes);
		text.setCharacterSize(15); // in pixels, not points
		text.setFillColor(sf::Color::White);
		// set the text style
		bool flag = true;
		while (flag)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					flag = false;
			}
			window.clear();
			window.draw(text);
			window.display();
		}
	}
	
};