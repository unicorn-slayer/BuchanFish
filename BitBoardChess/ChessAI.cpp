#include <iostream>
#include <cstdint>
#include <vector>
#include <bit>
#include <bitset>

bool quit = false;

// Constants for piece types
enum PieceType {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

// Constants for piece colors
enum PieceColor {
    WHITE,
    BLACK
};

// Bitboard representation of the chess board
struct Bitboard {
    uint64_t pieces[2][7]; // Two-dimensional array for piece types and colors
};

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
        setPiece(board, file, EMPTY, WHITE);
    }

    //unsetPiece(board, 24, EMPTY, WHITE);
    //unsetPiece(board, 27, EMPTY, WHITE);
    //unsetPiece(board, 28, EMPTY, WHITE);
    //unsetPiece(board, 18, EMPTY, WHITE);

    //setPiece(board, 8, EMPTY, WHITE);
    //setPiece(board, 11, EMPTY, WHITE);
    //setPiece(board, 12, EMPTY, WHITE);
    //setPiece(board, 1, EMPTY, WHITE);

    //unsetPiece(board, 8, PAWN, WHITE);
    //unsetPiece(board, 11, PAWN, WHITE);
    //unsetPiece(board, 12, PAWN, WHITE);
    //unsetPiece(board, 1, KNIGHT, WHITE);

    //setPiece(board, 24, PAWN, WHITE);
    //setPiece(board, 27, PAWN, WHITE);
    //setPiece(board, 28, PAWN, WHITE);
    //setPiece(board, 18, KNIGHT, WHITE);



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
        setPiece(board, file, EMPTY, BLACK);
    }

    //unsetPiece(board, 48, PAWN, BLACK);
    //unsetPiece(board, 49, PAWN, BLACK);
    //unsetPiece(board, 50, PAWN, BLACK);

    //setPiece(board, 48, EMPTY, BLACK);
    //setPiece(board, 49, EMPTY, BLACK);
    //setPiece(board, 50, EMPTY, BLACK);

    //unsetPiece(board, 32, EMPTY, BLACK);
    //unsetPiece(board, 41, EMPTY, BLACK);
    //unsetPiece(board, 34, EMPTY, BLACK);

    //setPiece(board, 32, PAWN, BLACK);
    //setPiece(board, 41, PAWN, BLACK);
    //setPiece(board, 34, PAWN, BLACK);

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
    //bounds check to make sure it stays within the horizontal walls of the chessboard
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
    // Check if it's a valid pawn move
    // Example: Only allowing pawns to move one square forward
    if (pieceColor == WHITE) {
        if (destinationSquare == sourceSquare + 8)
        {
            if (board.pieces[WHITE][EMPTY] & 1ULL << destinationSquare && board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
            {
                return true;
            }
        }
        if (destinationSquare == sourceSquare + 7)
        {
            for (int i = 0; i < 8; i++)
            {
                if (sourceSquare == (i * 8))
                {
                    return false;
                }
            }
            if (board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
            {
                return false;
            }
            else
            {
                return true;
            }

        }
        if (destinationSquare == sourceSquare + 9)
        {
            for (int i = 0; i < 8; i++)
            {
                if (sourceSquare == ((i * 8) + 7))
                {
                    return false;
                }
            }
            if (board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        if (sourceSquare > 7 && sourceSquare < 16)
        {
            if (destinationSquare == sourceSquare + 16)
            {
                if (board.pieces[WHITE][EMPTY] & 1ULL << destinationSquare && board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
                {
                    return true;
                }
            }
        }
    }
    else {
        if (destinationSquare == sourceSquare - 8)
        {
            if (board.pieces[WHITE][EMPTY] & 1ULL << destinationSquare && board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
            {
                return true;
            }
        }
        if (destinationSquare == sourceSquare - 7)
        {
            for (int i = 0; i < 8; i++)
            {
                if (sourceSquare == ((i * 8) + 7))
                {
                    return false;
                }
            }
            if (board.pieces[WHITE][EMPTY] & 1ULL << destinationSquare)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        if (destinationSquare == sourceSquare - 9)
        {
            for (int i = 0; i < 8; i++)
            {
                if (sourceSquare == (i * 8))
                {
                    return false;
                }
            }

            if (board.pieces[WHITE][EMPTY] & 1ULL << destinationSquare)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        if (sourceSquare > 47 && sourceSquare < 56)
        {
            if (destinationSquare == sourceSquare - 16)
            {
                if (board.pieces[WHITE][EMPTY] & 1ULL << (destinationSquare + 8) && board.pieces[BLACK][EMPTY] & 1ULL << (destinationSquare + 8))
                {
                    if (board.pieces[WHITE][EMPTY] & 1ULL << destinationSquare && board.pieces[BLACK][EMPTY] & 1ULL << destinationSquare)
                    {
                        return true;
                    }
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
case KING:
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

void makeMove(Bitboard& board, int sourceSquare, int destinationSquare) {

    if (!isMoveLegal(board, sourceSquare, destinationSquare)) {
        std::cout << std::endl << "illegal move detected" << std::endl;
        std::cout << std::endl;
        return;
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

                while ((isMoveLegal(board, sourceSquare, targetSquare)) == true)
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

                if ((isMoveLegal(board, sourceSquare, targetSquare)) == true)
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

    int whiteTotal = (whitePawnTotal * pawnScore) +
        (whiteKnightTotal * knightScore) +
        (whiteBishopTotal * bishopScore) +
        (whiteRookTotal * rookScore) +
        (whiteQueenTotal * queenScore) +
        (whiteKingTotal * kingScore);

    int blackTotal = (blackPawnTotal * pawnScore) +
        (blackKnightTotal * knightScore) +
        (blackBishopTotal * bishopScore) +
        (blackRookTotal * rookScore) +
        (blackQueenTotal * queenScore) +
        (blackKingTotal * kingScore);

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

void gameOver()
{
    std::cout << "game over" << std::endl;
    quit = true;
}

void computerMakeMove(Bitboard& board, int currentPlayer) {
    // Generate all possible moves for the current player
    std::vector<Move> moves = generateMoves(board, currentPlayer);

    if (moves.empty())
    {
        gameOver();
        return;
    }

    // Perform Minimax search to determine the best move
    int bestScore = INT_MAX;
    Move bestMove = { 0, 0 };

    for (const auto& move : moves) {
        //EXP
        Bitboard oldBoard = board;

        // Apply the move to the board
        makeMove(board, move.sourceSquare, move.destSquare);

        // white's turn now so maximizing is true (odd if CPU is black, even if CPU is white)
        int score = minimax(board, 5, INT_MIN, INT_MAX, true);

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


