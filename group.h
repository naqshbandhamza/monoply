#pragma once
#include "property.h"
#include <string>

class group
{
public:
	bool owned;
	int owned_by;
	char* group_name;
	int no_properties;
	property** list_of_properties;
	group(std::string temp);
	bool check_if_group_owned(int p_n);
	bool endswith(std::string temp);
	std::string return_string_index_to_index(int start, int end, std::string temp);
	~group();
};

