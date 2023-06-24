#include <SDL.h>
#include <iostream>
#include "BuchanFish.h"
#include "Graphics.h"
#include "Engine.h"
#include "MagicBitboard.h"


BuchanFish::BuchanFish()
{

	SDL_Init(SDL_INIT_EVERYTHING);

	Bitboard board;

	this->drawBoardAndGetInput(board);
}


BuchanFish::~BuchanFish()
{
}

void BuchanFish::drawBoardAndGetInput(Bitboard& board)
{

	Engine engine;
	
	Graphics graphics;

	Chessboard chessboard(graphics, 0, 0);

	Pieces pieces(graphics);

	engine.initBoard(board);
	engine.setupStartingPosition(board);

	std::string sourceSquare;
	std::string destinationSquare;

	while (true)
	{
		graphics.clear();
		chessboard.draw(graphics, 0, 0);
		pieces.draw(graphics, board);
		graphics.flip();


		SDL_Event event;

		if (SDL_WaitEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return;
			}
		}

		    std::cout << "choose a source square, i.e. E2: ";
		    std::cin >> sourceSquare;
		    std::cout << std::endl;

		    std::cout << "choose a destination square, i.e. E4: ";
		    std::cin >> destinationSquare;
		    std::cout << std::endl;

		    int sourceSquareInt = engine.convert(sourceSquare);
		    int destSquareInt = engine.convert(destinationSquare);

		    if (engine.makeMove(board, sourceSquareInt, destSquareInt) == 1)
		    {
		        graphics.clear();
		        chessboard.draw(graphics, 0, 0);

		        pieces.draw(graphics, board);
		        graphics.flip();
		        engine.computerMakeMove(board, 1);
		    }
	}



}



