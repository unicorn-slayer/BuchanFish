#pragma once
#include <SDL.h>
#include <string>

class Graphics;

class Sprite
{
public:
	Sprite();
	Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);
	~Sprite();
	void draw(Graphics& graphics, int x, int y);

protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
	float _x, _y;
	bool _visible;
};
