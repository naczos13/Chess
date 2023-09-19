#include "Pawn.h"
#include <iostream>
#include <array>

Pawn::Pawn(Team team, Point pos, SDL_Texture* texture)
	: Piece(team, pos, PieceType::PAWN, texture)
{
	if (team == Team::BLACK)
	{
		yDirection = -1;
	}
	else
	{
		yDirection = 1;
	}

	//render();
}

bool Pawn::canEnPassant(Piece* toCapture)
{
	if (!toCapture)
		return false;

	if (toCapture != m_canBeCaptureByEnPassant)
		return false;

	if (toCapture->getType() != PieceType::PAWN)
		return false;

	if (toCapture->getTeam() == getTeam())
		return false;

	if (getTeam() == Team::BLACK && getPosition().y != 3)
		return false;

	if (getTeam() == Team::WHITE && getPosition().y != 4)
		return false;

	return true;
}

void Pawn::tryToMakeDoubleMove(Piece** board, std::vector<PossibleMove>& possibleMoves, Piece* singleForwardPiece)
{
	Point doubleForward = { m_posistion.x, m_posistion.y + 2 * yDirection };
	if (!m_hasMoved && // only if this is first move
		!singleForwardPiece && // only if the way is empty
		!board[CoordToIndex(doubleForward.x, doubleForward.y)])
	{
		// Check if this move can make the pawn sensitive to enPassant
		Point leftNeighborPosition = { doubleForward.x - 1, doubleForward.y };
		Piece* leftNeighbor = nullptr;
		if (leftNeighborPosition.x > 0)
			leftNeighbor = board[CoordToIndex(leftNeighborPosition)];

		Point rightNeighborPosition = { doubleForward.x + 1, doubleForward.y };
		Piece* rightNeighbor = nullptr;
		if (rightNeighborPosition.x < 8)
			rightNeighbor = board[CoordToIndex(rightNeighborPosition)];

		PossibleMove doubleMove = PossibleMove(doubleForward, this);
		doubleMove.doubleMove = true;
		doubleMove.addPieceToInfuence(leftNeighbor);
		doubleMove.addPieceToInfuence(rightNeighbor);

		possibleMoves.push_back(doubleMove);
	}
}

std::vector<PossibleMove> Pawn::getPhysicallyPossibleMoves(Piece** board)
{
	std::vector<PossibleMove> posible_moves;

	// single forward - only if the place is empty
	Point singleForwardMove = { m_posistion.x, m_posistion.y + yDirection};
	Piece* singleForwadPiece = board[CoordToIndex(singleForwardMove.x, singleForwardMove.y)];
	if (!singleForwadPiece)
	{
		PossibleMove move = PossibleMove(singleForwardMove, this);
		if (singleForwardMove.y == 0 || singleForwardMove.y == 7)
			move.promoteThePawn = true;
		posible_moves.push_back(move);
	}
		

	// double forward - only if this is first move and in the way are no pieces
	// Also Check if this move can make the pawn sensitive to enPassant
	tryToMakeDoubleMove(board, posible_moves, singleForwadPiece);

	// capture - only if still in bounds and there is a enemy piece
	std::array<int, 2> verticalDirection{ -1, 1 };
	for (const int dx : verticalDirection)
	{
		Point moveVertical{ m_posistion.x + dx, m_posistion.y + yDirection};
		if (moveVertical.x >= 0 && moveVertical.x < 8)
		{
			Piece* toCapture = board[CoordToIndex(moveVertical.x, moveVertical.y)];
			if (toCapture && toCapture->getTeam() != getTeam())
			{
				PossibleMove move = PossibleMove(moveVertical, this);
				move.addPieceToCapture(toCapture);
				if (moveVertical.y == 0 || moveVertical.y == 7)
					move.promoteThePawn = true;
				posible_moves.push_back(move);
			}

			// enpassant
			Piece* toEnPassant = board[CoordToIndex(moveVertical.x, moveVertical.y - yDirection)];
			if (canEnPassant(toEnPassant))
			{
				PossibleMove move = PossibleMove(moveVertical, this);
				move.addPieceToCapture(toEnPassant);
				if (moveVertical.y == 0 || moveVertical.y == 7)
					move.promoteThePawn = true;
				posible_moves.push_back(move);
			}
		}
	}

	return posible_moves;
}
