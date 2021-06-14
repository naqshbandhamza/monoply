#pragma once
#include <sfml/Graphics.hpp>

class Animation
{
private:
	static constexpr int nFrames = 10;
	static float holdTime;
	sf::Texture texture;
	sf::IntRect frames[nFrames];
	int iFrame = 0;
	float time = 0.0f;
	void Advance();
	int nframes;
public:
	Animation(int x, int y, int width, int height,int noframes,std::string filename);
	void ApplyToSprite(sf::Sprite& s);
	void Update(float dt);
	~Animation();
};

