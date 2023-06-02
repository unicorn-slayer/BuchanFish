#include "Chessboard.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Globals.h"

Chessboard::Chessboard()
{
}

Chessboard::Chessboard(Graphics& graphics, float x, float y) :
	Sprite(graphics, "chessSquares.png", 0, 0, 1000, 500, x, y)
{
	//source rects for squares
	this->_blackSquare = SDL_Rect{ 0, 0, 500, 500 };
	this->_whiteSquare = SDL_Rect{ 500, 0, 500, 500 };
}

Chessboard::~Chessboard()
{
}


void Chessboard::draw(Graphics& graphics, int x, int y)
{

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			bool isLightSquare = (i + j) % 2 == 0;
			if (isLightSquare == true)
			{
				this->_sourceRect = this->_whiteSquare;
			}
			else
			{
				this->_sourceRect = this->_blackSquare;
			}

			float chessboardWidth = this->_squareWidth * 8;
			float chessboardHeight = this->_squareHeight * 8;

			float centreX = globals::SCREEN_WIDTH / 2 - chessboardWidth / 2;
			float centreY = globals::SCREEN_HEIGHT / 2 - chessboardHeight / 2;

			SDL_Rect destinationRectangle = { (i * this->_squareWidth) + centreX, (j * this->_squareHeight) + centreY, this->_squareWidth, this->_squareHeight };
			graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
		}
	}
}
