#pragma once

#include <utility>
#include <vector>
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

enum class Team { NONE=0, BLACK, WHITE };

enum class PieceType { EMPTY=0, PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN };

class King;

class Piece;

struct PossibleMove
{
	PossibleMove(int x, int y, Piece* pieceToMove) 
		: XCoord{ x }, YCoord{ y }, PieceToMove{ pieceToMove } {};

	PossibleMove(Point moveTo, Piece* pieceToMove)
		: XCoord{ moveTo.x }, YCoord{ moveTo.y }, PieceToMove{ pieceToMove } {};

	void addPieceToInfuence(Piece* piece)
	{
		PiecesToInfluence.push_back(piece);
	}

	void addPieceToCapture(Piece* piece)
	{
		PieceToCapture = piece;
		capturePiece = true;
	}

	Point getDestination()
	{
		return { XCoord, YCoord };
	}
	
	int XCoord, YCoord;
	Piece* PieceToMove = nullptr;
	std::vector<Piece*> PiecesToInfluence;
	Piece* PieceToCapture = nullptr;
	bool capturePiece = false;
	bool doubleMove = false;
	bool promoteThePawn = false;
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
	Piece(Team team, Point pos, PieceType type, SDL_Texture* texture);

	// Empty Constructor
	Piece();

	// Copy-Constructor
	Piece(const Piece& piece);

	// Destructor
	~Piece();

	// true, if piece has moved
	bool m_hasMoved;

	// returns type of piece
	PieceType getType() const { return m_type; };

	bool moveMakeMyKingToBeCheck(Piece** board, const King* king, const Point* move, Piece* CurrentPiece);

	virtual std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) = 0;

	bool canEliminateKing(Piece** board, const Piece* king);

	bool isActive() const { return m_stillInGame; };

	void deactivate() { m_stillInGame = false; };

	SDL_Texture* getTexture() const { return m_texture; };

protected:

	// texture of this piece
	SDL_Texture* m_texture;

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
