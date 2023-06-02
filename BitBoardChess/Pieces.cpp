#include "Pieces.h"
#include <cstdint>
#include <vector>
#include <bit>
#include <bitset>
#include "Globals.h"
#include "Graphics.h"
#include "Sprite.h"

Pieces::Pieces(Graphics& graphics) :
	_width(globals::TILE_WIDTH),
	_height(globals::TILE_HEIGHT),
	Sprite(graphics, "chessPieces.png", 0, 0, 1000, 500, 0, 0)
{
	_sourceRect = { 0, 0, 0, 0 };

	// pawn, knight, bishop, rook, queen, king
	_clipWidths = {185, 265, 285, 235, 310, 265};
	_clipX = {1690, 1000, 660, 1352, 312, 0};
	_clipHeight = 270;
}

void Pieces::draw(Graphics& graphics, Bitboard& board)
{
	//white pieces
	for (int i = 1; i < 7; i++)
	{
		std::string piece = std::bitset< 64 >(board.pieces[0][i]).to_string();
		std::reverse(piece.begin(), piece.end());

		for (int j = 0; j < 64; j++)
		{
			if (piece[j] == '1')
			{
				int rank = j / 8;
				int file = j % 8;

				int x = file * _width; // (0)
				int y = (7 - rank) * _height; // (360)

				_sourceRect = { _clipX[i-1], 0, _clipWidths[i-1], _clipHeight };
				SDL_Rect destinationRect = { x + _centreX, y + _centreY, _width, _height };
				graphics.blitSurface(this->_spriteSheet, &_sourceRect, &destinationRect);

			}

		}

	}

	//black pieces
	for (int i = 1; i < 7; i++)
	{
		std::string piece = std::bitset< 64 >(board.pieces[1][i]).to_string();
		std::reverse(piece.begin(), piece.end());

		for (int j = 0; j < 64; j++)
		{
			if (piece[j] == '1')
			{
				int rank = j / 8;
				int file = j % 8;

				int x = file * _width; // (0)
				int y = (7 - rank) * _height; // (360)

				_sourceRect = { _clipX[i - 1], 330, _clipWidths[i - 1], _clipHeight };
				SDL_Rect destinationRect = { x + _centreX, y + _centreY, _width, _height };
				graphics.blitSurface(this->_spriteSheet, &_sourceRect, &destinationRect);

			}

		}

	}


}


