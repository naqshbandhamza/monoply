#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sstream>
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

using namespace std;

class Textbox
{
public:
	bool isSelected = false;
	Textbox()
	{

	}
	Textbox(int size,sf::Color color,bool sel)
	{
		textbox.setCharacterSize(size);
		textbox.setFillColor(color);
		isSelected = sel;
		if (sel)
		{
			textbox.setString("_");
		}
		else
			textbox.setString("");
	}
	void setFont(sf::Font& font)
	{
		textbox.setFont(font);
	}
	void setPosition(sf::Vector2f pos)
	{
		textbox.setPosition(pos);
	}
	void setLimit(bool ToF)
	{
		hasLimit = ToF;
	}
	void setLimit(bool ToF, int lim)
	{
		hasLimit = ToF;
		limit = lim;
	}
	void setSelected(bool sel)
	{
		isSelected = sel;
		if (!sel)
		{
			string t = text;
			string newT = "";
			for (int i = 0; i < t.length()-1; i++)
			{
				newT+= t[i];
			}
			textbox.setString(newT);
		}
	
	}
	string getText()
	{
		return text;
	}
	void drawTo(sf::RenderWindow & window)
	{
		window.draw(textbox);
	}
	void typedOn(sf::Event input,string&user)
	{
		if (isSelected)
		{
			int charTyped = input.text.unicode;
			if (charTyped < 128)
			{
				if (charTyped == DELETE_KEY)
				{
					if(user[user.length() - 1] != ']')
					    deleteLastChar();
				}
				else
				{
					if (user[user.length() - 1] != ']')
					{
						user = inputLogic(charTyped);

					}
				}
			}
		}
	}
	void typedOn1(sf::Event input,string& temp)
	{
		if (isSelected)
		{
			cout << "inwhat" << endl;
			int charTyped = input.text.unicode;
			if (charTyped < 128)
			{
				if (charTyped == DELETE_KEY)
				{
					if (temp[temp.length() - 1] != ']')
						deleteLastChar();
				}
				else
				{
					if (temp[temp.length() - 1] != ']')
					{
						temp = inputLogic1(charTyped);

					}
				}
			}
		}
		else
			cout << isSelected << endl;
	}
	void setstring()
	{
		text = "";
		text1 = "";
		textbox.setString("_");
	}
	void deleteLastChar()
	{
		string t = text;
		string newT = "";
		if (t.length() != 1 && t.length()!=0)
		{
			for (int i = 0; i < t.length() - 1; i++)
			{
				newT += t[i];
			}

			text="";
			text = newT;
			textbox.setString(newT);
		}
		else
		{
			cout << "good" << endl;
				text = ""; text1 = ""; textbox.setString("_");
		}
	}
private:
	sf::Text textbox;
	string text="",text1="";
	bool hasLimit = false;
	int limit;
	string inputLogic(int charTyped)
	{
		string temp = "";
		if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
		{
			text += static_cast<char>(charTyped);
		}
		else if (charTyped == ENTER_KEY)
		{
			textbox.setString(text);
			temp = text + "]";
			text = "";
			textbox.setString("_");
			return temp;
		}
		textbox.setString(text);
		return text;
	}
	string inputLogic1(int charTyped)
	{
		string temp = "";
		if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
		{
				text += static_cast<char>(charTyped);
		}
		else if (charTyped == ENTER_KEY)
		{
			textbox.setString(text);
				temp = text + "]";
				text = "";
				textbox.setString("_");
				return temp;
		}
		textbox.setString(text);
			return text;
	}
};

