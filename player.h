#pragma once
#include "group.h"
#include "property.h"
#include <SFML/Graphics.hpp>


class button;

class game;

class player
{
	//friend class game;
private:
	
	char *position;
	int no_of_properties,no_of_groups;
	group** list_of_groups;
	sf::Color color;
public:
	std::string player_status;
	bool jail_status;
	bool free_jail_pass;
	void read_data_from_file(game* ptr, sf::RectangleShape list[]);
	void sell_property(property*ptr,game*gptr,sf::RectangleShape list[]);
	void sell_to_payoff(game*ptr,sf::RenderWindow& win, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext, sf::Text playermoney, button b3, button b1);
	void own_group(int g_n,group*ptr);
	void disown_any_group_property();
	void clear_group(group*g);
	property** list_of_properties;
	bool check_if_can_pay_off(int money);
	int compute_landtax();
	int compute_propertytax();
	int player_num, player_money;
	int prev_pos,pos;
	std::string getnameofpropertyatpos(int pos);
	bool pay_bail();
	property* getpropertyatpos(int pos);
	int getindexofpropertyatpos(int pos);
	int getpriceofpropertyatpos(int pos);
	int get_n_properties();
	player(int player_number);
	bool buy_property(property* ptr,int next = 0);
	sf::Color& getcolor();
	bool check_if_this_player_owns_property(property* ptr);
	bool pay_landtax();
	bool pay_propertytax();
	bool pay_hotelrent();
	bool pay_rent(property* ptr);
	~player();
};

