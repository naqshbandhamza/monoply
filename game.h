#pragma once
#include "player.h"
#include "group.h"
#include "property.h"
#include "Character.h"
#include "Textbox.h"
#include "messagebox.h"
#include "button.h"

class game
{
	friend class player;
private:
	int no_groups;
	group** list_of_groups;
public:
	player** list_of_players;
	int no_players;
	int* turns;
	void eliminate_player(int no);
	std::string mychance();
	Character** list_of_characters;
	game(int no_of_players);
	int rolldice();
	bool check_if_string_is_number(string temporary);
	bool check_if_array_filled(int* arr, int size);
	bool check_if_array_has_number(int* arr,int num,int *aarr, int size);
	int find_largest(int arr[], int size);
	int taketurn(int dicenum);
	int auction(sf::RenderWindow& win, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext, sf::Text playermoney,button b3,int &next);
	void takemove(int p_n,int dicenum, sf::RenderWindow & target, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext,sf::Text playermoney);
	void landedonsomeproperty(int p_n,sf::RenderWindow &win ,sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext, sf::Text playermoney);
	void exchange(int p_n,sf::RenderWindow& win, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext, sf::Text playermoney, button b3,button b1,int &next,int &ggx);
	
	~game();
};

