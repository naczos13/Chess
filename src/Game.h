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
#include <optional>

struct ValidMove
{
    bool status;
    PossibleMove move;
};

class Game
{
public:
	// Constructor
	Game(SDL_Handler* handler);

    // Destructor
    ~Game();

	// returns a the Piece in board (row, col)
    Piece* getPieceByPosition(int row, int col);

    // return possible moves per piece
    std::vector<PossibleMove> getPosibleMovesForPiece(Piece* piece) const;

    //Moves a piece
    void move(Piece* piece, PossibleMove move);

    // returns m_turn
    Team getTurn() { return m_turn; };

    std::optional<PossibleMove> GetValidMove(const Point& endPoint, const std::vector<PossibleMove>& possibleMoves) const;

    // calculates all allowed moves of every piece
    void calcAllMoves();

    // light up the possible Moves
    void renderPossibleMoves(const std::vector<PossibleMove>& possible);

    // undos the renderPossibleMoves function
    void undoRenderPossibleMoves(const std::vector<PossibleMove>& possible);

    void createPieces();

    // 2D board array, every Position has got a Team and a piece
    Piece* board[64];
    Piece** m_board = board;

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

