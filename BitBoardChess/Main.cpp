#include <iostream>
#include <cstdint>
#include <vector>
#include <bit>
#include <bitset>
#include "Graphics.h"
#include "Chessboard.h"
#include "SDL.h"
#include "Pieces.h"


struct Move {

    int sourceSquare;
    int destSquare;
};

// Initialize an empty chess board
void initBoard(Bitboard& board) {
    for (int color = 0; color < 2; color++) {
        for (int type = 0; type < 7; type++) {
            board.pieces[color][type] = 0;
        }
    }
}

// Set a piece at a given square on the board
void setPiece(Bitboard& board, int square, int pieceType, int pieceColor) {
    board.pieces[pieceColor][pieceType] |= (1ULL << square);
}

void unsetPiece(Bitboard& board, int square, int pieceType, int pieceColor) {
    board.pieces[pieceColor][pieceType] &= ~(1ULL << square);
}

// Set up the starting position of the chessboard
void setupStartingPosition(Bitboard& board) {
    initBoard(board);
 
    // Placing white pieces
    for (int file = 0; file < 8; file++) {
        setPiece(board, file + 8, PAWN, WHITE); // White pawns
    }

    setPiece(board, 0, ROOK, WHITE); // White rooks
    setPiece(board, 1, KNIGHT, WHITE); // White knights
    setPiece(board, 2, BISHOP, WHITE); // White bishops
    setPiece(board, 3, QUEEN, WHITE); // White queen
    setPiece(board, 4, KING, WHITE); // White king
    setPiece(board, 5, BISHOP, WHITE); // White bishops
    setPiece(board, 6, KNIGHT, WHITE); // White knights
    setPiece(board, 7, ROOK, WHITE); // White rooks

    for (int file = 16; file < 64; file++)
    {
        setPiece(board, file, EMPTY, WHITE); //empty squares for white
    }


    // Placing black pieces
    for (int file = 0; file < 8; file++) {
        setPiece(board, file + 48, PAWN, BLACK); // Black pawns
    }
    setPiece(board, 56, ROOK, BLACK); // Black rooks
    setPiece(board, 57, KNIGHT, BLACK); // Black knights
    setPiece(board, 58, BISHOP, BLACK); // Black bishops
    setPiece(board, 59, QUEEN, BLACK); // Black queen
    setPiece(board, 60, KING, BLACK); // Black king
    setPiece(board, 61, BISHOP, BLACK); // Black bishops
    setPiece(board, 62, KNIGHT, BLACK); // Black knights
    setPiece(board, 63, ROOK, BLACK); // Black rooks

    for (int file = 0; file < 48; file++)
    {
        setPiece(board, file, EMPTY, BLACK); //empty squares for black
    }
}

// Print the board state
void printBoard(const Bitboard& board) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            bool empty = true;
            for (int color = 0; color < 2; color++) {
                for (int type = 0; type < 7; type++) {
                    if (board.pieces[color][type] & (1ULL << square)) {
                        char pieceChar = 'E';
                        switch (type) {
                        case EMPTY: break;
                        case PAWN: pieceChar = (color == WHITE) ? 'p' : 'P'; break;
                        case KNIGHT: pieceChar = (color == WHITE) ? 'n' : 'N'; break;
                        case BISHOP: pieceChar = (color == WHITE) ? 'b' : 'B'; break;
                        case ROOK: pieceChar = (color == WHITE) ? 'r' : 'R'; break;
                        case QUEEN: pieceChar = (color == WHITE) ? 'q' : 'Q'; break;
                        case KING: pieceChar = (color == WHITE) ? 'k' : 'K'; break;
                        }

                        if (type != EMPTY)
                        {
                            std::cout << pieceChar;
                            empty = false;
                        }                      
                        
                        break;
                    }
                }
            }

            if (empty) {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool isMoveLegal(const Bitboard& board, int sourceSquare, int destinationSquare) {
    // Determine the piece type and color on the source square
    int pieceType = EMPTY;
    int pieceColor = WHITE;
    //bounds check to make sure it stays within the top and bottom bounds of the chessboard
    if (destinationSquare < 0 || destinationSquare >= 64) {
        return false;
    }
    for (int color = 0; color < 2; color++) {
        for (int type = 1; type < 7; type++) {
            if (board.pieces[color][type] & (1ULL << sourceSquare)) {
                pieceType = type;
                pieceColor = color;
                goto Check;
            }
        }
    }

    // Perform legality check based on piece type, color, and the specific move
    Check:switch (pieceType) {
    case PAWN: {

        int direction = (pieceColor == WHITE) ? 1 : -1;

        //check to see if pawn can move one square up
        if (destinationSquare == sourceSquare + (8 * direction))
        {
            if (board.pieces[pieceColor][EMPTY] & 1ULL << destinationSquare && board.pieces[!pieceColor][EMPTY] & 1ULL << destinationSquare)
            {
                return true;
            }
        }
        // check to see if pawn can take a piece
        if (destinationSquare == sourceSquare + (7 * direction) || destinationSquare == sourceSquare + (9 * direction))
        {

            int fileDifferance = abs((destinationSquare % 8) - (sourceSquare % 8));

            //this check is needed to make sure pawns don't jump from one side to the other, i.e. white pawn going from 32 to 39
            if (fileDifferance != 1)
            {
                return false;
            }

            if (board.pieces[!pieceColor][EMPTY] & 1ULL << destinationSquare)
            {
                return false;
            }
            else
            {
                return true;
            }

        }
        //check to see if pawn can move two squares up
        if ((pieceColor == WHITE) && (sourceSquare / 8 == 1) || (pieceColor == BLACK) && (sourceSquare / 8 == 6))
        {            
            if (destinationSquare == sourceSquare + (16 * direction))
            {
                //check to make sure destination square is empty of white or black pieces
                if (board.pieces[pieceColor][EMPTY] & 1ULL << destinationSquare && board.pieces[!pieceColor][EMPTY] & 1ULL << destinationSquare)
                {
                    //check to make sure path is empty of white or black pieces
                    if (board.pieces[pieceColor][EMPTY] & 1ULL << (destinationSquare - (8 * direction)) && board.pieces[!pieceColor][EMPTY] & 1ULL << (destinationSquare - (8 * direction)))
                    {
                        return true;
                    }
                    
                }
            }
        }
       
    break;
    }
    case KNIGHT: {
        if (pieceColor == WHITE)
        {
            // Check if it's a valid knight move
            // Example: Allowing knight moves in L-shape
            int rankDiff = abs(destinationSquare / 8 - sourceSquare / 8);
            int fileDiff = abs(destinationSquare % 8 - sourceSquare % 8);
            if ((rankDiff == 2 && fileDiff == 1) || (rankDiff == 1 && fileDiff == 2)) {
                if (board.pieces[WHITE][EMPTY] & 1ULL << destinationSquare)
                {
                    return true;
                }
            }
        }
        else
        {
            int rankDiff = abs(destinationSquare / 8 - sourceSquare / 8);
            int fileDiff = abs(destinationSquare % 8 - sourceSquare % 8);
            if ((rankDiff == 2 && fileDiff == 1) || (rankDiff == 1 && fileDiff == 2)) {
                if (board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
                {
                    return true;
                }
            }
        }

        break;
    }
    case BISHOP: {
        // Check if it's a valid bishop move
        // Example: Allowing diagonal movements
        int rankDiff = abs(destinationSquare / 8 - sourceSquare / 8);
        int fileDiff = abs(destinationSquare % 8 - sourceSquare % 8);
        if (rankDiff == fileDiff) {
            // Check if there are no obstructions along the diagonal path
            int rankStep = (destinationSquare / 8 > sourceSquare / 8) ? 1 : -1;
            int fileStep = (destinationSquare % 8 > sourceSquare % 8) ? 1 : -1;
            int currentSquare = sourceSquare + 8 * rankStep + fileStep;
            while (currentSquare != destinationSquare) {
                if (board.pieces[WHITE][EMPTY] & (1ULL << currentSquare) && board.pieces[BLACK][EMPTY] & (1ULL << currentSquare)) {
                    currentSquare += 8 * rankStep + fileStep;
                }
                else {
                    return false;
                }
            }
            if (pieceColor == WHITE)
            {
                if (board.pieces[WHITE][EMPTY] & 1ULL << destinationSquare)
                {
                    return true;
                }
            }
            else
            {
                if (board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
                {
                    return true;
                }
            }
        }
        break;
    }
    case ROOK: {
        int sourceRank = sourceSquare / 8;
        int sourceFile = sourceSquare % 8;
        int destRank = destinationSquare / 8;
        int destFile = destinationSquare % 8;

        // Check if the move is horizontal or vertical
        if (sourceRank != destRank && sourceFile != destFile) {
            return false;
        }

        int rankDiff = std::abs(destRank - sourceRank);
        int fileDiff = std::abs(destFile - sourceFile);

        // Check if there are any pieces obstructing the path
        if (rankDiff > 0) {
            int rankStep = (destRank > sourceRank) ? 1 : -1;
            for (int rank = sourceRank + rankStep; rank != destRank; rank += rankStep) {
                int square = rank * 8 + sourceFile;
                if (board.pieces[WHITE][EMPTY] & (1ULL << square) && board.pieces[BLACK][EMPTY] & (1ULL << square)) {
                    // Empty square, continue checking
                    continue;
                }
                else {
                    // Square is occupied, illegal move
                    return false;
                }
            }
        }
        else if (fileDiff > 0) {
            int fileStep = (destFile > sourceFile) ? 1 : -1;
            for (int file = sourceFile + fileStep; file != destFile; file += fileStep) {
                int square = sourceRank * 8 + file;
                if (board.pieces[WHITE][EMPTY] & (1ULL << square) && board.pieces[BLACK][EMPTY] & (1ULL << square)) {
                    // Empty square, continue checking
                    continue;
                }
                else {
                    // Square is occupied, illegal move
                    return false;
                }
            }
        }
        
        if (pieceColor == WHITE)
        {
            if (board.pieces[WHITE][EMPTY] & (1ULL << destinationSquare)) {
                return true;
            }
        }

    }
    case QUEEN:
    {
        int sourceRank = sourceSquare / 8;
        int sourceFile = sourceSquare % 8;
        int destRank = destinationSquare / 8;
        int destFile = destinationSquare % 8;

        int rankDiff = std::abs(destRank - sourceRank);
        int fileDiff = std::abs(destFile - sourceFile);

        // Check if the move is horizontal, vertical or diagonal
        if (sourceRank != destRank && sourceFile != destFile) {
            if (rankDiff != fileDiff)
            {
                return false;
            }

        }

        // Check if there are any pieces obstructing the path
        if (rankDiff != fileDiff)
        {
            if (rankDiff > 0) {
                int rankStep = (destRank > sourceRank) ? 1 : -1;
                for (int rank = sourceRank + rankStep; rank != destRank; rank += rankStep) {
                    int square = rank * 8 + sourceFile;
                    if (board.pieces[WHITE][EMPTY] & (1ULL << square) && board.pieces[BLACK][EMPTY] & (1ULL << square)) {
                        // Empty square, continue checking
                        continue;
                    }
                    else {
                        // Square is occupied, illegal move
                        return false;
                    }
                }
            }
            else if (fileDiff > 0) {
                int fileStep = (destFile > sourceFile) ? 1 : -1;
                for (int file = sourceFile + fileStep; file != destFile; file += fileStep) {
                    int square = sourceRank * 8 + file;
                    if (board.pieces[WHITE][EMPTY] & (1ULL << square) && board.pieces[BLACK][EMPTY] & (1ULL << square)) {
                        // Empty square, continue checking
                        continue;
                    }
                    else {
                        // Square is occupied, illegal move
                        return false;
                    }
                }
            }
        }


        if (rankDiff == fileDiff) {
            // Check if there are no obstructions along the diagonal path
            int rankStep = (destinationSquare / 8 > sourceSquare / 8) ? 1 : -1;
            int fileStep = (destinationSquare % 8 > sourceSquare % 8) ? 1 : -1;
            int currentSquare = sourceSquare + 8 * rankStep + fileStep;
            while (currentSquare != destinationSquare) {
                if (board.pieces[WHITE][EMPTY] & (1ULL << currentSquare) && board.pieces[BLACK][EMPTY] & (1ULL << currentSquare)) {
                    
                    currentSquare += 8 * rankStep + fileStep;
                }
                else
                {
                    return false;
                }              
                
            }
        }

        if (pieceColor == WHITE)
        {
            if (board.pieces[WHITE][EMPTY] & (1ULL << destinationSquare)) {
                return true;
            }
        }
        else
        {
            if (board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
            {
                return true;
            }
        }
    }
    case KING : 
        {
        int sourceRank = sourceSquare / 8;
        int sourceFile = sourceSquare % 8;
        int destRank = destinationSquare / 8;
        int destFile = destinationSquare % 8;

        int rankDiff = std::abs(destRank - sourceRank);
        int fileDiff = std::abs(destFile - sourceFile);

        // Check if the move is within the king's range (1 square in any direction)
        if (rankDiff <= 1 && fileDiff <= 1) {
            if (pieceColor == WHITE)
            {
                // If the destination square is empty or occupied by an opponent's piece, the move is legal
                if (board.pieces[WHITE][EMPTY] & (1ULL << destinationSquare)) {
                    return true;
                }
            }
            else
            {
                // If the destination square is empty or occupied by an opponent's piece, the move is legal
                if (board.pieces[BLACK][EMPTY] & (1ULL << destinationSquare)) {
                    return true;
                }
            }

            }
        }

    }

    // The move is not legal for the given piece type
    return false;
}

int convert(std::string number) {
    if (number == "A1") return 0;
    if (number == "B1") return 1;
    if (number == "C1") return 2;
    if (number == "D1") return 3;
    if (number == "E1") return 4;
    if (number == "F1") return 5;
    if (number == "G1") return 6;
    if (number == "H1") return 7;
    if (number == "A2") return 8;
    if (number == "B2") return 9;
    if (number == "C2") return 10;
    if (number == "D2") return 11;
    if (number == "E2") return 12;
    if (number == "F2") return 13;
    if (number == "G2") return 14;
    if (number == "H2") return 15;
    if (number == "A3") return 16;
    if (number == "B3") return 17;
    if (number == "C3") return 18;
    if (number == "D3") return 19;
    if (number == "E3") return 20;
    if (number == "F3") return 21;
    if (number == "G3") return 22;
    if (number == "H3") return 23;
    if (number == "A4") return 24;
    if (number == "B4") return 25;
    if (number == "C4") return 26;
    if (number == "D4") return 27;
    if (number == "E4") return 28;
    if (number == "F4") return 29;
    if (number == "G4") return 30;
    if (number == "H4") return 31;
    if (number == "A5") return 32;
    if (number == "B5") return 33;
    if (number == "C5") return 34;
    if (number == "D5") return 35;
    if (number == "E5") return 36;
    if (number == "F5") return 37;
    if (number == "G5") return 38;
    if (number == "H5") return 39;
    if (number == "A6") return 40;
    if (number == "B6") return 41;
    if (number == "C6") return 42;
    if (number == "D6") return 43;
    if (number == "E6") return 44;
    if (number == "F6") return 45;
    if (number == "G6") return 46;
    if (number == "H6") return 47;
    if (number == "A7") return 48;
    if (number == "B7") return 49;
    if (number == "C7") return 50;
    if (number == "D7") return 51;
    if (number == "E7") return 52;
    if (number == "F7") return 53;
    if (number == "G7") return 54;
    if (number == "H7") return 55;
    if (number == "A8") return 56;
    if (number == "B8") return 57;
    if (number == "C8") return 58;
    if (number == "D8") return 59;
    if (number == "E8") return 60;
    if (number == "F8") return 61;
    if (number == "G8") return 62;
    if (number == "H8") return 63;

};

int makeMove(Bitboard& board, int sourceSquare, int destinationSquare) {


    if (!isMoveLegal(board, sourceSquare, destinationSquare)) {
        std::cout << std::endl << "illegal move detected" << std::endl;
        std::cout << std::endl;
        return 0;
    }
    // Determine the piece type and color on the source square
    int pieceType = EMPTY;
    int pieceColor = WHITE;
    for (int color = 0; color < 2; color++) {
        for (int type = 1; type < 7; type++) {
            if (board.pieces[color][type] & (1ULL << sourceSquare)) {
                pieceType = type;
                pieceColor = color;
                goto Move;
            }
        }
    }

    // Clear the source square on the corresponding bitboard
    Move:
    board.pieces[pieceColor][pieceType] &= ~(1ULL << sourceSquare);
    board.pieces[pieceColor][EMPTY] |= (1ULL << sourceSquare);

    // Set the destination square on the corresponding bitboard
    board.pieces[pieceColor][pieceType] |= (1ULL << destinationSquare);
    board.pieces[pieceColor][EMPTY] &= ~(1ULL << destinationSquare);

    // Check if a capture has occured
    for (int TYPE = 1; TYPE < 7; TYPE++)
    {
        if (board.pieces[pieceColor][pieceType] & board.pieces[!pieceColor][TYPE])
        {
            //capture has occurred
            board.pieces[!pieceColor][TYPE] &= ~(1ULL << destinationSquare);
            board.pieces[!pieceColor][EMPTY] |= (1ULL << destinationSquare);
        }
    }


    // Perform any additional updates based on the move, e.g., capturing, castling, en passant, promotion, etc.
    // ...
    return 1;
}


std::vector<Move> generateMoves(const Bitboard& board, int player)
{
    std::vector<Move> moves;
    for (int sourceSquare = 0; sourceSquare < 64; sourceSquare++) {
        // Check if the current square contains a piece of the player
        if (board.pieces[player][PAWN] & (1ULL << sourceSquare)) {
            // Generate pawn moves
            // Example: Allow pawn to move one square forward if the destination is empty
            int destinationSquare = sourceSquare + (player == WHITE ? 8 : -8);
            if (isMoveLegal(board, sourceSquare, destinationSquare)) {
                moves.push_back({ sourceSquare, destinationSquare });
            }
            destinationSquare = sourceSquare + (player == WHITE ? 7 : -7);
            if (isMoveLegal(board, sourceSquare, destinationSquare)) {
                moves.push_back({ sourceSquare, destinationSquare });
            }
            destinationSquare = sourceSquare + (player == WHITE ? 9 : -9);
            if (isMoveLegal(board, sourceSquare, destinationSquare)) {
                moves.push_back({ sourceSquare, destinationSquare });
            }
            destinationSquare = sourceSquare + (player == WHITE ? 16 : -16);
            if (isMoveLegal(board, sourceSquare, destinationSquare)) {
                moves.push_back({ sourceSquare, destinationSquare });
            }
            break;
        }
        if (board.pieces[player][KNIGHT] & (1ULL << sourceSquare))
        {
            // Define the possible knight move offsets
            int moveOffsets[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };

            for (int offset : moveOffsets)
            {
                if (isMoveLegal(board, sourceSquare, (sourceSquare + offset)))
                {
                    moves.push_back({ sourceSquare, (sourceSquare + offset) });
                }
            }

        }
        if (board.pieces[player][BISHOP] & (1ULL << sourceSquare))
        {
            // Define the possible bishop move directions (diagonal vectors)
            int moveDirections[4] = { -9, -7, 7, 9 };

            // Generate moves for each possible bishop move direction
            for (int direction : moveDirections) {

                int targetSquare = sourceSquare + direction;

                while((isMoveLegal(board, sourceSquare, targetSquare)) == true)
                {
                    moves.push_back({ sourceSquare, targetSquare });
                    targetSquare += direction;
                }
            }
        }
        if (board.pieces[player][ROOK] & (1ULL << sourceSquare))
        {
            // Define the possible bishop move directions (diagonal vectors)
            int moveDirections[4] = { -1, 1, 8, -8 };

            // Generate moves for each possible bishop move direction
            for (int direction : moveDirections) {

                int targetSquare = sourceSquare + direction;

                while ((isMoveLegal(board, sourceSquare, targetSquare)) == true)
                {
                    moves.push_back({ sourceSquare, targetSquare });
                    targetSquare += direction;
                }
            }
        }
        if (board.pieces[player][QUEEN] & (1ULL << sourceSquare))
        {
            // Define the possible bishop move directions (diagonal vectors)
            int moveDirections[8] = { -1, 1, 8, -8, -9, -7, 7, 9 };

            // Generate moves for each possible bishop move direction
            for (int direction : moveDirections) {

                int targetSquare = sourceSquare + direction;

                while ((isMoveLegal(board, sourceSquare, targetSquare)) == true)
                {
                    moves.push_back({ sourceSquare, targetSquare });
                    targetSquare += direction;
                }
            }
        }
        if (board.pieces[player][KING] & (1ULL << sourceSquare))
        {
            // Define the possible bishop move directions (diagonal vectors)
            int moveDirections[8] = { -1, 1, 8, -8, -9, -7, 7, 9 };

            // Generate moves for each possible bishop move direction
            for (int direction : moveDirections) {

                int targetSquare = sourceSquare + direction;

                if((isMoveLegal(board, sourceSquare, targetSquare)) == true)
                {
                    moves.push_back({ sourceSquare, targetSquare });
                }
            }
        }
    }

    return moves;
}

void undoMove(Bitboard& board, Move moveUndo)
{
    // Determine the piece type and color on the source square 70932793642713088
    int pieceType = EMPTY;
    int pieceColor = WHITE;
    for (int color = 0; color < 2; color++) {
        for (int type = 1; type < 7; type++) {
            if (board.pieces[color][type] & (1ULL << moveUndo.destSquare)) {
                pieceType = type;
                pieceColor = color;
                goto Move;
            }
        }
    }

    // Clear the source square on the corresponding bitboard
Move:
    board.pieces[pieceColor][pieceType] &= ~(1ULL << moveUndo.destSquare);
    board.pieces[pieceColor][EMPTY] |= (1ULL << moveUndo.destSquare);

    // Set the destination square on the corresponding bitboard
    board.pieces[pieceColor][pieceType] |= (1ULL << moveUndo.sourceSquare);
    board.pieces[pieceColor][EMPTY] &= ~(1ULL << moveUndo.sourceSquare);
}

int evaluatePosition(Bitboard& board)
{
    int whitePawnTotal = std::_Popcount(board.pieces[WHITE][PAWN]);
    int blackPawnTotal = std::_Popcount(board.pieces[BLACK][PAWN]);

    int whiteKnightTotal = std::_Popcount(board.pieces[WHITE][KNIGHT]);
    int blackKnightTotal = std::_Popcount(board.pieces[BLACK][KNIGHT]);

    int whiteBishopTotal = std::_Popcount(board.pieces[WHITE][BISHOP]);
    int blackBishopTotal = std::_Popcount(board.pieces[BLACK][BISHOP]);

    int whiteRookTotal = std::_Popcount(board.pieces[WHITE][ROOK]);
    int blackRookTotal = std::_Popcount(board.pieces[BLACK][ROOK]);

    int whiteQueenTotal = std::_Popcount(board.pieces[WHITE][QUEEN]);
    int blackQueenTotal = std::_Popcount(board.pieces[BLACK][QUEEN]);

    int whiteKingTotal = std::_Popcount(board.pieces[WHITE][KING]);
    int blackKingTotal = std::_Popcount(board.pieces[BLACK][KING]);

    int pawnScore = 1;
    int knightScore = 3;
    int bishopScore = 3;
    int rookScore = 5;
    int queenScore = 9;
    int kingScore = 1000;

    int whiteTotal = 
        (whitePawnTotal     *   pawnScore)      +
        (whiteKnightTotal   *   knightScore)    +
        (whiteBishopTotal   *   bishopScore)    +
        (whiteRookTotal     *   rookScore)      +
        (whiteQueenTotal    *   queenScore)     +
        (whiteKingTotal     *   kingScore);

    int blackTotal = 
        (blackPawnTotal     *   pawnScore)      +
        (blackKnightTotal   *   knightScore)    +
        (blackBishopTotal   *   bishopScore)    +
        (blackRookTotal     *   rookScore)      +
        (blackQueenTotal    *   queenScore)     +
        (blackKingTotal     *   kingScore);

    return whiteTotal - blackTotal;
}

int minimax(Bitboard& board, int depth, int alpha, int beta, bool maximizingPlayer) {
 
    // Base case: reached maximum depth or game over
    if (depth == 0) {
        return evaluatePosition(board);
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        // Generate all possible moves for the maximizing player
        std::vector<Move> moves = generateMoves(board, WHITE);

        for (const auto& move : moves) {

            Bitboard oldBoard = board;

            // Apply the move to the board
            makeMove(board, move.sourceSquare, move.destSquare);

            // Recursively call the minimax function for the next depth, switching to the opponent
            int eval = minimax(board, depth - 1, alpha, beta, false);

            board = oldBoard;

            // Update the maximum evaluation
            maxEval = std::max(maxEval, eval);

            // Alpha-beta pruning
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }

        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        // Generate all possible moves for the minimizing player
        std::vector<Move> moves = generateMoves(board, BLACK);

        for (const auto& move : moves) {
            //EXP SAVE BOARD
            Bitboard oldBoard = board;

            // Apply the move to the board
            makeMove(board, move.sourceSquare, move.destSquare);

            // Recursively call the minimax function for the next depth, switching to the opponent
            int eval = minimax(board, depth - 1, alpha, beta, true);

            //// Undo the move
            //undoMove(board, move);

            board = oldBoard;

            // Update the minimum evaluation
            minEval = std::min(minEval, eval);

            // Alpha-beta pruning
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }

        return minEval;
    }
}


void computerMakeMove(Bitboard& board, int currentPlayer) {
    // Generate all possible moves for the current player
    std::vector<Move> moves = generateMoves(board, currentPlayer);

    if (moves.empty())
    {
        return;
    }

    // Perform Minimax search to determine the best move
    int bestScore = INT_MAX;
    Move bestMove = {0, 0};

    for (const auto& move : moves) {
        //EXP
        Bitboard oldBoard = board;

        // Apply the move to the board
        makeMove(board, move.sourceSquare, move.destSquare);

        // white's turn now so maximizing is true (odd if CPU is black, even if CPU is white)
        int score = minimax(board, 4, INT_MIN, INT_MAX, true);

        //// Undo the move
        //undoMove(board, move);

        board = oldBoard;

        // Update the best move if a higher score is found
        if (score < bestScore) {
            bestScore = score; //DEBUG 2147483647 is coming up as a best score
            bestMove = move;
        }
    }
    makeMove(board, bestMove.sourceSquare, bestMove.destSquare);
}




int main(int argc, char* args[]) {

    Bitboard board;
    initBoard(board);
    setupStartingPosition(board);
    
    SDL_Init(SDL_INIT_EVERYTHING);
    Graphics graphics;
    Chessboard _chessboard(graphics, 0, 0);
    Pieces pieces(graphics);

    std::string sourceSquare;
    std::string destinationSquare;

    while (true)
    {
        graphics.clear();
        _chessboard.draw(graphics, 0, 0);

        pieces.draw(graphics, board);
        graphics.flip();

        SDL_Event event;

        if (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return 0;
            }
        }

        std::cout << "choose a source square, i.e. E2: ";
        std::cin >> sourceSquare;
        std::cout << std::endl;

        std::cout << "choose a destination square, i.e. E4: ";
        std::cin >> destinationSquare;
        std::cout << std::endl;

        int sourceSquareInt = convert(sourceSquare);
        int destSquareInt = convert(destinationSquare);

        if (makeMove(board, sourceSquareInt, destSquareInt) == 1)
        {
            graphics.clear();
            _chessboard.draw(graphics, 0, 0);

            pieces.draw(graphics, board);
            graphics.flip();
            computerMakeMove(board, 1);
        }
        
    }

    return 0;

}


