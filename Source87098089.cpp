#include <iostream>
#include <crtdbg.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int main()
{
	sf::Text* monoplytext;
	monoplytext = new sf::Text();
	delete monoplytext;
	_CrtDumpMemoryLeaks();
}