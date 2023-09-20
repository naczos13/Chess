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
		: XCoord{ x }, YCoord{ y }, PieceToMove{ pieceToMove }, promoteThePawn{ false }  {};

	PossibleMove(Point moveTo, Piece* pieceToMove)
		: XCoord{ moveTo.x }, YCoord{ moveTo.y }, PieceToMove{ pieceToMove }, promoteThePawn{ false } {};

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

	void makeAPromoteMove()
	{
		promoteThePawn = true;
	}

	bool isPromoteThePawn()
	{
		return promoteThePawn;
	}
	
	int XCoord, YCoord;
	Piece* PieceToMove = nullptr;
	std::vector<Piece*> PiecesToInfluence;
	Piece* PieceToCapture = nullptr;
	bool capturePiece = false;
	bool doubleMove = false;

private:
	bool promoteThePawn = false;
};

class Piece
{
public:

	// Constructor
	Piece(Team team, Point pos, PieceType type, SDL_Texture* texture);

	// Copy-Constructor
	Piece(const Piece& other) = delete;

	// Copy-Assigment
	Piece operator=(const Piece& other) = delete;

	// Move-Contructor
	Piece(Piece&& other) = delete;

	// Move-Assigment
	Piece operator=(Piece&& other) = delete;

	// Destructor
	~Piece() = default;

	// returns list of possible Moves
	std::vector<PossibleMove> getPossibleMoves(Piece** board);

	// return whether BLACK or WHITE
	Team getTeam() const { return m_team; };

	// sets new position
	void setPosition(Point newPos) { m_posistion = newPos; };

	// return position of piece
	Point getPosition() const { return m_posistion; };

	// returns type of piece
	PieceType getType() const { return m_type; };

	// Check if specific move expose own King to be capture
	bool moveMakeMyKingToBeCheck(Piece** board, const King* king, const Point* move, Piece* CurrentPiece);

	// Check the possible move without checking for safty of own King
	virtual std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) = 0;

	// Check if this piece can capture the Enemy King
	bool canEliminateKing(Piece** board, const Piece* king);

	// Check if the Piece is still in the game
	bool isActive() const { return m_stillInGame; };

	// Deactivate the Piece from the game, when was capture or Promoted
	void deactivate() { m_stillInGame = false; };

	// Get the SDL texture
	SDL_Texture* getTexture() const { return m_texture; };

	// Give information that the Piece is already moved
	void setPieceIsMoved() { m_hasMoved = true; };

protected:

	// true, if piece has moved
	bool m_hasMoved;

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
