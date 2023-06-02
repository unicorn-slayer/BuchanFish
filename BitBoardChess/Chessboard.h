#pragma once
#include "Sprite.h"
#include <vector>

class Sprite;

class Chessboard : public Sprite
{
public:
	Chessboard();
	Chessboard(Graphics& graphics, float x, float y);
	~Chessboard();
	void draw(Graphics& graphics, int x, int y);

private:
	int _squareWidth = 60;
	int _squareHeight = 60;
	SDL_Rect _whiteSquare;
	SDL_Rect _blackSquare;

};
