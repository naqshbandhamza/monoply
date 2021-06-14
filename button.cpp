#include "button.h"

#include <cmath>
#include <iostream>

using namespace std;


button::button(float x, float y, float width, float height,
	sf::Font* font, sf::Font font2, string text,
	sf::Color idleColor, sf::Color hoverColor, sf::Color ActiveColor, int choice)
{
	ex = 1;
	this->buttonstate = BTN_IDLE;
	if (choice != 3)
	{
		this->shape.setSize(sf::Vector2f(width, height));
		this->shape.setPosition(sf::Vector2f(x, y));
	}
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(30);
	if (choice == 0)
		this->text.setPosition(sf::Vector2f(x + 120, y+10));
	else if (choice == 1)
	{
		this->text.setCharacterSize(20);
		this->text.setPosition(sf::Vector2f(x, y));
	}
	else if (choice == 2)
		this->text.setPosition(sf::Vector2f(x + 40, y + 80));
	else if (choice == 19910)
	{
		ex = 19910;
		this->text.setCharacterSize(17);
		shape2.setRadius(width);
		//shape2.setPointCount(100);
		shape2.setPosition(sf::Vector2f(x, y));
		//shape2.setOutlineColor(sf::Color::White);
		//shape2.setOutlineThickness(1);
		this->text.setPosition(sf::Vector2f(x+5,y+6));
	}
	this->hoverColor = hoverColor;
	this->idleColor = idleColor;
	this->activeColor = ActiveColor;

	this->shape.setFillColor(this->idleColor);
}


button::~button()
{

}

const bool button::isPressed() const
{
	if (this->buttonstate == BTN_PRESSED)
		return true;
	return false;

}

void button::settext(std::string t)
{
	this->text.setString(t);
}

void button::update(const sf::Vector2f mousePos,sf::RenderWindow& win)
{
	//sf::Mouse::isButtonPressed(sf::Mouse::Left)
	this->buttonstate = BTN_IDLE;
	if (ex!=19910)
	{
		if (this->shape.getGlobalBounds().contains(mousePos))
		{
			this->buttonstate = BTN_HOVER;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					this->buttonstate = BTN_PRESSED;
				}
		}
	}
	else
	{
		if (this->shape2.getGlobalBounds().contains(mousePos))
		{
			this->buttonstate = BTN_HOVER;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->buttonstate = BTN_PRESSED;
			}
		}
	}

	switch (this->buttonstate)
	{
	case BTN_IDLE:
		if(ex!=19910)
		    this->shape.setFillColor(this->idleColor);
		else
			this->shape2.setFillColor(this->idleColor);
		break;
	case BTN_HOVER:
		if(ex!=19910)
		    this->shape.setFillColor(this->hoverColor);
		else
			this->shape2.setFillColor(this->hoverColor);
		break;
	case BTN_PRESSED:
		if(ex!=19910)
		    this->shape.setFillColor(this->activeColor);
		else
			this->shape2.setFillColor(this->activeColor);
		break;
	default:
		if(ex!=19910)
		    this->shape.setFillColor(sf::Color::Red);
		else
			this->shape2.setFillColor(sf::Color::Red);
		break;
	}
}

void button::render(sf::RenderTarget* target)
{
	if (ex != 19910)
		target->draw(this->shape);
	else
	{
		target->draw(this->shape2);
	}
	target->draw(this->text);
}




