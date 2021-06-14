#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Animation.h"

class game;

class Character
{
private:
	sf::Vector2f pos;
	sf::Vector2f vel = { 0.0f,0.0f };
	sf::Texture texture;
	sf::Sprite sprite;
	static float speed;
	int c_no;
	enum class AnimationIndex
	{
		walkingUp,
		walkingRight,
		walkingDown,
		walkingLeft,
		Count
	};
	Animation** animations;
	AnimationIndex curAnimation = AnimationIndex::walkingUp;
public:
	Character(int no,const sf::Vector2f &pos,int cno, int dimenX, int dimenY,int noframes,int total_cycles,std::string walkpattern[],std::string filename);
	void Draw(sf::RenderTarget& target);
	void read_data_from_file(game* ptr);
	void setDirection(const sf::Vector2f& dir,int choice);
	void Update(float dt);
	sf::Vector2f  getcharacterposition();
	~Character();
};

