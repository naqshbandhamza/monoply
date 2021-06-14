#include "group.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string group::return_string_index_to_index(int start,int end,string temp)
{
	string Temp = "";
	for (int i = start; i < end + 1; i++)
	{
		Temp += temp[i];
	}
	return Temp;
}

group::group(string temp)
{
	group_name = new char[temp.length()+1];
	for (int i = 0; i < temp.length(); i++)
	{
		group_name[i] = temp[i];
	}
	this->owned = false;
	group_name[temp.length()] = '\0';
}

bool group::check_if_group_owned(int p_n)
{
	bool check = true;
	for (int i = 0; i < this->no_properties; i++)
	{
		if (list_of_properties[i]->owned_by_player != p_n)
		{
			check = false; break;
		}
	}
	return check;
}

bool group::endswith(std::string temp)
{
	int index = strlen(group_name) - 1, index2 = temp.length() - 1;;
	while (group_name[index] != ',')
	{
		if (temp[index2] != group_name[index])
			return false;
		index--;
		index2--;
		if (index2 == -1)
			break;
	}
	if (group_name[index] == ',')
		return true;
	else
		return false;
}

group::~group()
{
	delete[]group_name;
	cout << "group destructor called" << endl;
	for (int i = 0; i < no_properties; i++)
		delete list_of_properties[i];
	delete[]list_of_properties;
}