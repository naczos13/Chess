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
#include <array>

class Game
{
public:
	// Constructor
	Game(SDL_Handler* handler);

    // Destructor
    ~Game();

	// returns a the Piece in board (row, col)
    Piece* getPieceByPosition(int row, int col);

    //Moves a piece
    void move(PossibleMove& move);

    // returns m_turn
    Team getTurn() { return m_turn; };

    // return the move if exist in the pool of possible moves
    std::optional<PossibleMove> GetValidMove(const Point& endPoint, const std::vector<PossibleMove>& possibleMoves) const;

    // light up the possible Moves
    void renderPossibleMoves(const std::vector<PossibleMove>& possible);

    // undos the renderPossibleMoves function
    void undoRenderPossibleMoves(const std::vector<PossibleMove>& possible);

    void createPieces();

    void disableUnusedEnPassant();

    void changeTurn();

    void createPiece(const PieceType& type, const Team& team, const Point& position);

    // 2D board array, every Position has got a Team and a piece
    std::array<Piece*, 64> board;
    Piece** m_board = board.data();

private: 

    // exchange move
    void exchange(int xStart, int yStart, int xEnd, int yEnd);

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

