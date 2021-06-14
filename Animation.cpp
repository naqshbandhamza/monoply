#include "Animation.h"
#include <iostream>
using namespace std;

//int Animation::nFrames = 8;
float Animation::holdTime = 0.1f;


Animation::Animation(int x,int y,int width,int height,int noframes,string filename)
{
	nframes = noframes;
	texture.loadFromFile(filename);
	for (int i = 0; i < nframes; i++)
	{
		frames[i] = { x + i * width,y,width,height };
	}
}

void Animation::ApplyToSprite(sf::Sprite& s)
{
	s.setTexture(texture);
	s.setTextureRect(frames[iFrame]);
}

void Animation::Advance()
{
	if (++iFrame >= nframes)
	{
		iFrame = 0;
	}
}

void Animation::Update(float dt)
{
	time += dt;
	while (time >= holdTime)
	{
		time -= holdTime;
		Advance(); 
	}
}

Animation::~Animation()
{
	cout << "animation destructor called"<<endl;
}
