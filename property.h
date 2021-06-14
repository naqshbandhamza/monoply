#pragma once
#include <string>

class property
{
public:
	int owned_by_g_no;
	std::string name;
	std::string type;
	bool owned,mortgaged,build;
	int owned_by_player;
	int price, pos, rent,no_of_buildings;
	std::string list_of_building[5];
	property(std::string temp,int group_number);
	void setowned(int p);
	bool operator ==(property*ptr);
	~property();
};

