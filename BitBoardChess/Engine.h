#pragma once
#include "Pieces.h"
#include "vector"
#include "MagicBitboard.h"

struct Move {

	int sourceSquare;
	int destSquare;
};


class Engine
{
public:

	Engine();

	void initBoard(Bitboard& board);

	void setPiece(Bitboard& board, int square, int pieceType, int pieceColor);

	void unsetPiece(Bitboard& board, int square, int pieceType, int pieceColor);

	void setupStartingPosition(Bitboard& board);

	void printBoard(const Bitboard& board);

	static bool isMoveLegal(const Bitboard& board, int sourceSquare, int destinationSquare);

	std::vector<Move> generateMoves(const Bitboard& board, int player);

	int convert(std::string number);

	int makeMove(Bitboard& board, int sourceSquare, int destinationSquare);

	int evaluatePosition(Bitboard& board);

	int minimax(Bitboard& board, int depth, int alpha, int beta, bool maximizingPlayer);

	void computerMakeMove(Bitboard& board, int currentPlayer);



private:

	std::unique_ptr<MagicBitboards> magicBitboard = std::make_unique<MagicBitboards>();

};