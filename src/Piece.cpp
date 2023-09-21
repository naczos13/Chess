#include "Piece.h"
#include "King.h"
#include <string>
#include <iostream>
#include <array>

int CoordToIndex(int x, int y) {
	return x * 8 + y;
}

int CoordToIndex(Point p) {
	return p.x * 8 + p.y;
}

std::array<Piece*, 64> Piece::createSimulatedBoard(Piece** board, PossibleMove& move)
{
	std::array<Piece*, 64> boardCopy;
	for (size_t i{}; i < 64; i++)
	{
		boardCopy[i] = board[i];
	}

	boardCopy[CoordToIndex(move.PieceToMove->getPosition())] = nullptr;
	if (move.capturePiece)
		boardCopy[CoordToIndex(move.PieceToCapture->getPosition())] = nullptr;
	boardCopy[CoordToIndex(move.getDestination())] = move.PieceToMove;

	return boardCopy;
}

bool Piece::moveMakeMyKingToBeCheck(Piece** board, const King* king, PossibleMove& move)
{
	bool captureTheKing = false;

	std::array<Piece*, 64> boardCopy = createSimulatedBoard(board, move);

	for (Piece* potentialEnemy : boardCopy)
	{
		if (potentialEnemy && potentialEnemy->getTeam() != king->getTeam())
		{
			if (potentialEnemy->canEliminateKing(boardCopy.data(), king))
			{
				return true;
			}
		}
	}

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

	for (PossibleMove& move : getPhysicallyPossibleMoves(board))
	{
		// simulate the move
		// need to check this because maybe this move can led to own checkmate
		if (!moveMakeMyKingToBeCheck(board, getOwnKing(board), move))
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


