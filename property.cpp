#include "property.h"
#include <iostream>

using namespace std;
//we will make functions afterwards which fill no_of_buildings and list_of_buildings when upgrading
property::property(string temp,int group_number)
{
	owned_by_g_no = group_number;
	build = false;
	this->no_of_buildings = 4;
	name = temp;
	owned = false;
	mortgaged = false;
	string temporary = "";
	for (int index = 0; temp[index] != ')'; index++)
		temporary += temp[index];
	pos = stoi(temporary);
	cout << "pos:" << pos << endl;
	list_of_building[0] = "house";
	list_of_building[1] = "house";
	list_of_building[2] = "shop";
	list_of_building[3] = "hotel";
}

void property::setowned(int p)
{
	this->owned_by_player = p;
}

bool property::operator ==(property* ptr)
{
	if (name == ptr->name)
		return true;
	return false;
}

property::~property()
{
	cout << "property destructor called" << endl;
}
