#pragma once
#include "Chessboard.h"

struct Bitboard;

class Graphics;

class BuchanFish
{
public:
	BuchanFish();
	BuchanFish(Bitboard& board);
	~BuchanFish();

private:
	void drawBoardAndGetInput(Bitboard& board);
	void draw(Graphics& graphics);
	
};