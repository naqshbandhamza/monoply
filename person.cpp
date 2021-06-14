#include "person.h"
#include <iostream>

using namespace std;

person::person(char *fn,char *ln)
{
	fname = fn;
	lname = ln;
}

person::~person()
{
	delete[] fname;
	delete[] lname;
}
