#include "Character.h"
#include <iostream>
#include "game.h"
#include <fstream>
using namespace std;

float Character::speed = 1.0f;


int find_index(string walkpattern[],string temp)
{
	for (int i = 0; i < 4; i++)
	{
		if (temp == walkpattern[i])
			return i;
	}
	return 0;
}

Character::Character(int no,const sf::Vector2f &pos,int cno, int dimenX, int dimenY, int noframes, int total_cycles,string walkpattern[],string filename)
{
	this->c_no = no;
	string up = "Up", down = "Down", right = "Right", left = "Left";
	
		animations = new Animation*[int(AnimationIndex::Count)];
		//animations[int(AnimationIndex::walkingDown)] = new Animation(0,0, dimenX / noframes,dimenY / total_cycles,noframes);
		animations[int(AnimationIndex::walkingDown)] = new Animation(0, (dimenY / total_cycles) * find_index(walkpattern, up), (dimenX / noframes), dimenY / total_cycles, noframes,filename);
		animations[int(AnimationIndex::walkingRight)] = new Animation(0, (dimenY / total_cycles)*find_index(walkpattern,right), dimenX / noframes, dimenY / total_cycles,noframes,filename);
		//animations[int(AnimationIndex::walkingUp)] = new Animation(0, (dimenY / total_cycles )*2, dimenX / noframes, dimenY / total_cycles,noframes);
		animations[int(AnimationIndex::walkingUp)] = new Animation(0, (dimenY / total_cycles) * find_index(walkpattern, down), dimenX / noframes, dimenY / total_cycles, noframes,filename);
		animations[int(AnimationIndex::walkingLeft)] = new Animation(0, (dimenY / total_cycles) *find_index(walkpattern,left), dimenX / noframes, dimenY / total_cycles,noframes,filename);
		texture.loadFromFile(filename);
	sprite.setTexture(texture);
	sprite.setTextureRect({ 0,0,dimenX / noframes,dimenY / total_cycles });
	this->pos = pos;
	sprite.setPosition(this->pos);
}

void Character::Draw(sf::RenderTarget &target)
{
	target.draw(sprite);
}

void Character::setDirection(const sf::Vector2f & dir,int choice)
{
	vel = dir * speed;
	if (dir.x > 0.0f && choice==0)
	{
		curAnimation = AnimationIndex::walkingRight;
	}
	else if (dir.x < 0.0f && choice==1)
	{
		curAnimation = AnimationIndex::walkingLeft;
	}
	else if (dir.y < 0.0f && choice==3)
	{
		curAnimation = AnimationIndex::walkingDown;
		//curAnimation = AnimationIndex::walkingUp;
	}
	else if (dir.y > 0.0f && choice==2)
	{
		//curAnimation = AnimationIndex::walkingDown;
		curAnimation = AnimationIndex::walkingUp;
	}
}

void Character::Update(float dt)
{
	pos += vel * dt;
	animations[int(curAnimation)]->Update(dt);
	animations[int(curAnimation)]->ApplyToSprite(sprite);
	sprite.setPosition(pos);
}

sf::Vector2f Character:: getcharacterposition()
{
	return sprite.getPosition();
}


void Character::read_data_from_file(game* ptr)
{
	ifstream read;
	string filename = "character" + to_string(this->c_no) + ".txt";
	read.open(filename); char temp; string textra = "",pos_X="",pos_Y="";
	int count = 0, inner_count = 0;
	while (!read.eof())
	{
		read >> noskipws >> temp;
		if (temp != '\n' && !read.eof())
			textra = textra + temp;
		else
		{
			if (count == 0)
			{
				count++;
				textra = "";
			}
			else if (count == 1)
			{
				for (int i = 0; i < textra.length(); i++)
				{
					if (textra[i] != ',' && (i+1)!=textra.length())
					{
						if (inner_count == 0)
							pos_X = pos_X + textra[i];
						else if (inner_count == 1)
							pos_Y = pos_Y + textra[i];
					}
					else
					{
						if (inner_count == 0)
							this->pos.x = stoi(pos_X);
						else if ((i + 1) == textra.length())
						{
							pos_Y = pos_Y + textra[i];
							this->pos.y = stoi(pos_Y);
						}
						inner_count++;
					}
				}
			}
		}
	}
	read.close();
	sf::Vector2f dir = { 0.0f,0.0f }, vel = { 0.0f,0.0f };
	sprite.setPosition(this->pos);
	if (ptr->list_of_players[this->c_no - 1]->pos >= 1 && ptr->list_of_players[this->c_no - 1]->pos < 12)
	{
		dir.y = 0.0f;
		if (dir.x < 0.0f)
			dir.x *= -1;
		dir.x += 1.0f;
		this->setDirection(dir, 0);
		this->Update(1.0f / 20.0f);
	}
	else if (ptr->list_of_players[c_no - 1]->pos >= 22 && ptr->list_of_players[c_no - 1]->pos < 32)
	{
		dir.y = 0.0f;
		if (dir.x > 0.0f)
			dir.x *= -1;
		dir.x -= 1.0f;
		this->setDirection(dir, 1);
		this->Update(1.0f / 20.f);
	}
	else if (ptr->list_of_players[c_no - 1]->pos >= 12 && ptr->list_of_players[c_no - 1]->pos < 22)
	{
		dir.x = 0.0f;
		if (dir.y < 0.0f)
			dir.y *= -1;
		dir.y += 1.0f;
		this->setDirection(dir, 2);
		this->Update(1.0f / 20.f);
	}
	else if (ptr->list_of_players[c_no - 1]->pos >= 32 && ptr->list_of_players[c_no - 1]->pos < 42)
	{
		dir.x = 0.0f;
		if (dir.y > 0.0f)
			dir.y *= -1;
		dir.y -= 1.0f;
		this->setDirection(dir, 3);
		this->Update(1.0f / 20.f);
	}

}

Character::~Character()
{
	cout << "character destructor called" << endl;

	ofstream write;
	std::string filename = "character"+to_string(this->c_no)+".txt";
	write.open(filename);
	write << c_no;
	write <<'\n'<<this->pos.x << "," << this->pos.y;
	write.close();
	delete animations[int(AnimationIndex::walkingUp)];
	delete animations[int(AnimationIndex::walkingDown)];
	delete animations[int(AnimationIndex::walkingLeft)];
	delete animations[int(AnimationIndex::walkingRight)];
	delete[] animations;
}


