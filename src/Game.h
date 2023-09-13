#pragma once
#include "SDL_Handler.h"
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include <memory>

class Game
{
public:
	// Constructor
	Game(SDL_Handler* handler);

    // Destructor
    ~Game();

	// returns a the Piece in field (row, col)
    Piece* getPieceByPosition(int row, int col);

    //Moves a piece
    void move(Piece* piece, PossibleMove move);

    // returns m_turn
    Team getTurn() { return m_turn; };

    // true, if the move is valid  ;  false if not
    bool isValidMove(int x, int y, Piece* piece);

    // calculates all allowed moves of every piece
    void calcAllMoves();

    // light up the possible Moves
    void renderPossibleMoves(Piece* piece);

    // undos the renderPossibleMoves function
    void undoRenderPossibleMoves(Piece* piece);

    void createPieces();

    // 2D Field array, every Position has got a Team and a piece
    Piece* board[64];
    Piece** m_field = board;

private: 



    // disables enPassant for every Piece
    void disableEnPassant();

    // normal move
    void normal(int xStart, int yStart, int xEnd, int yEnd);

    // enpassant move
    void enPassant(int xStart, int yStart, int xEnd, int yEnd);

    // exchange move
    void exchange(int xStart, int yStart, int xEnd, int yEnd);

    // castles move
    void castles(int xStart, int yStart, int xEnd, int yEnd);

    // Background filename
    std::string m_backgroundFilename;

    // Decides whether if its black or whites turn
    Team m_turn{};

    // handler
    SDL_Handler* m_handler;

    // if true, disable en Passant! if false, dont
    bool m_checkEnPassant;

    // checks current game state, determines winner or remis
    void gameState();

    // Own the pieces, and ensure their release when game over
    std::vector<std::unique_ptr<Piece>> pieces;

    King* blackKing;
    King* whiteKing;




};

