#pragma once
#include "Sprite.h"
#include <vector>



// Constants for piece colors
enum PieceColor {
    WHITE,
    BLACK
};

// Bitboard representation of the chess board
struct Bitboard {
    uint64_t pieces[2][7]; // Two-dimensional array for piece types and colors
};

enum PieceType {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

class Pieces : public Sprite
{
public:

	Pieces(Graphics& graphics);

	void draw(Graphics& graphics, Bitboard& board);

private:

    int _width;
    int _height;
    int _centreX = 235;
    int _centreY = 85;
    SDL_Rect _sourceRect;
    std::vector<int> _clipWidths;
    std::vector<int> _clipX;
    int _clipHeight;

};