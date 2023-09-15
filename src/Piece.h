#pragma once

#include <utility>
#include <vector>
#include "SDL_Handler.h"
#include <SDL.h>

struct Point
{
	Point(int x, int y) : x{ x }, y{ y } {};
	Point(const Point& other) : x{ other.x }, y{ other.y } {};
	Point() : x{}, y{} {};
	int x{}, y{};
};

int CoordToIndex(int x, int y);

int CoordToIndex(Point p);

enum Team { NONE=0, BLACK, WHITE };

enum PieceType { EMPTY=0, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN };

enum MoveType { NORMAL, CASTLE, ENPASSANT, NEWPIECE, INIT };

class King;

class Piece;

struct PossibleMove
{
	PossibleMove(int x, int y, MoveType mt) : XCoord{ x }, YCoord{ y }, MoveType{ mt } {};
	int XCoord, YCoord;
	MoveType MoveType;
	std::vector<Piece*> PiecesToMove;
	Piece* PieceToCapture;
};

class Piece
{
public:

	// returns list of possible Moves
	std::vector<PossibleMove> getPossibleMoves(Piece** board, bool checkCheck=true);

	// return whether BLACK or WHITE
	Team getTeam() const { return m_team; };

	// sets new position
	void setPosition(Point newPos) { m_posistion = newPos; };

	// return position of piece
	Point getPosition() const { return m_posistion; };

	// Constructor
	Piece(Team team, Point pos, SDL_Handler* handler, PieceType type);

	// Empty Constructor
	Piece();

	// Copy-Constructor
	Piece(const Piece& piece);

	// Destructor
	~Piece();

	//render this piece
	void render();

	// true, if piece has moved
	bool m_hasMoved;

	// returns type of piece
	PieceType getType() const { return m_type; };

	bool moveMakeMyKingToBeCheck(Piece** board, const King* king, const Point* move, Piece* CurrentPiece) const;

	virtual std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) const = 0;

	bool canEliminateKing(Piece** board, const Piece* king) const;

protected:

	// texture of this piece
	SDL_Texture* m_texture;

	// SDL Handler
	SDL_Handler* m_handler;

	// Team this piece plays for
	Team m_team;
 
	// The Type of Piece
	PieceType m_type;

	// List of possible Moves this piece can do <row, col>
	std::vector<PossibleMove> m_possibleMoves;
	
	// Position of the piece
	Point m_posistion;

	// returns king of own team from board
	King* getOwnKing(Piece** board);

	King* m_ownKing = nullptr;

public:
	static int s_piece_counter;
};
