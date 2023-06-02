#include <SDL.h>
#include <iostream>
#include "BuchanFish.h"
#include "Graphics.h"


BuchanFish::BuchanFish()
{
}

BuchanFish::BuchanFish(Bitboard& board)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->drawBoardAndGetInput(board);
}

BuchanFish::~BuchanFish()
{
}

void BuchanFish::drawBoardAndGetInput(Bitboard& board)
{
	Graphics graphics;

	//draws chessboard to screen
	this->_chessboard = Chessboard(graphics, 0, 0);


		SDL_Event event;

		if (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return;
			}
		}

}

void BuchanFish::draw(Graphics& graphics)
{
	graphics.clear();
	this->_chessboard.draw(graphics, 0, 0);
	graphics.flip();

}

