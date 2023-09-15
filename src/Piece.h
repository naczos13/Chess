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

enum MoveType { NORMAL, CAPTURE, CASTLE, ENPASSANT, NEWPIECE, INIT };

class King;

class Piece;

struct PossibleMove
{
	PossibleMove(int x, int y, Piece* pieceToMove, MoveType mt = MoveType::NORMAL, Piece* pieceToCapture=nullptr) 
		: XCoord{ x }, YCoord{ y }, PieceToMove{ pieceToMove }, SecondPiecesToMove{ nullptr }, MoveType{ mt	}, PieceToCapture{ pieceToCapture }
	{};

	PossibleMove(Point moveTo, Piece* pieceToMove, MoveType mt = MoveType::NORMAL, Piece* pieceToCapture = nullptr)
		: XCoord{ moveTo.x }, YCoord{ moveTo.y }, PieceToMove{ pieceToMove }, SecondPiecesToMove{ nullptr }, MoveType{ mt }, PieceToCapture{ pieceToCapture }
	{};

	PossibleMove(int x, int y, Piece* firstPieceToMove, Piece* secondPieceToMove, MoveType mt)
		: XCoord{ x }, YCoord{ y }, PieceToMove{ firstPieceToMove }, SecondPiecesToMove{ secondPieceToMove }, MoveType{ mt }, PieceToCapture{ nullptr }
	{};
	
	int XCoord, YCoord;
	MoveType MoveType;
	Piece* PieceToMove = nullptr;
	Piece* SecondPiecesToMove = nullptr;
	Piece* PieceToCapture = nullptr;
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

	bool moveMakeMyKingToBeCheck(Piece** board, const King* king, const Point* move, Piece* CurrentPiece);

	virtual std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) = 0;

	bool canEliminateKing(Piece** board, const Piece* king);

	bool isActive() const { return m_stillInGame; };

	void deactivate() { m_stillInGame = false; };

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

	bool m_stillInGame = true;

public:
	static int s_piece_counter;
};
