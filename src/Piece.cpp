#include "Piece.h"
#include "King.h"
#include <string>
#include <iostream>

int CoordToIndex(int x, int y) {
	return x * 8 + y;
}

int CoordToIndex(Point p) {
	return p.x * 8 + p.y;
}

bool Piece::moveMakeMyKingToBeCheck(Piece** board, const King* king, const Point* move, Piece* CurrentPiece)
{
	// make this move, loop over all the enemy pieces and check if this can lead to checkmate of own king, then revert the move

	// what I need
	// - make a move
	// - list of enemy Pieces
	// - function that check if piece can capture a king
	// - revert the move

	bool captureTheKing = false;

	// # Make a move
	Piece* capturePiece = board[CoordToIndex(move->x, move->y)];
	const Point prevPiecePosition = CurrentPiece->getPosition();
	CurrentPiece->setPosition(*move);
	board[CoordToIndex(move->x, move->y)] = CurrentPiece;
	board[CoordToIndex(prevPiecePosition.x, prevPiecePosition.y)] = nullptr;

	for (int x{ 0 }; x < 8; x++)
	{
		for (int y{ 0 }; y < 8; y++)
		{
			Piece* potentialEnemy = board[CoordToIndex(x, y)];
			if (potentialEnemy && potentialEnemy->getTeam() != king->getTeam())
			{
				if (potentialEnemy->canEliminateKing(board, king))
				{
					// revert the move
					board[CoordToIndex(move->x, move->y)] = capturePiece;
					CurrentPiece->setPosition(prevPiecePosition);
					board[CoordToIndex(prevPiecePosition.x, prevPiecePosition.y)] = CurrentPiece;
					return true;
				}
			}
		}
	}

	// revert the move
	board[CoordToIndex(move->x, move->y)] = capturePiece;
	CurrentPiece->setPosition(prevPiecePosition);
	board[CoordToIndex(prevPiecePosition.x, prevPiecePosition.y)] = CurrentPiece;
	return false;
}

bool Piece::canEliminateKing(Piece** board, const Piece* king)
{
	std::vector<PossibleMove> posible_moves = getPhysicallyPossibleMoves(board);
	for (const auto& move : posible_moves)
	{
		const Piece* possiblePiece = board[CoordToIndex(move.XCoord, move.YCoord)];
		if (possiblePiece && possiblePiece == king)
			return true;
	}
	return false;
}

std::vector<PossibleMove> Piece::getPossibleMoves(Piece** board)
{
	std::vector<PossibleMove> posible_moves;

	for (const PossibleMove& move : getPhysicallyPossibleMoves(board))
	{
		// simulate the move
		// need to check this because maybe this move can led to own checkmate
		Point Target = { move.XCoord, move.YCoord };
		if (!moveMakeMyKingToBeCheck(board, getOwnKing(board), &Target, this))
		{
			posible_moves.emplace_back(move);
		}
	}

	return posible_moves;
}

King* Piece::getOwnKing(Piece** board)
{
	if (m_ownKing)
		return m_ownKing;

	for  (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[CoordToIndex(i, j)] != nullptr)
			{
				if (board[CoordToIndex(i, j)]->getTeam() == m_team && board[CoordToIndex(i, j)]->getType() == PieceType::KING)
				{
					m_ownKing = static_cast<King*>(board[CoordToIndex(i, j)]);
					return m_ownKing;
				}
			}
		}
	}
	return m_ownKing;
}

Piece::Piece(Team team, Point pos, PieceType type, SDL_Texture* texture)
	:m_team(team), m_posistion(pos), m_texture(texture), m_hasMoved(false), m_type(type)
{ }


