#include "Pawn.h"
#include <iostream>
#include <array>

Pawn::Pawn(Team team, Point pos, SDL_Handler* handler)
	:Piece(team, pos, handler, PAWN), m_enPassant(std::pair<bool, int>(false, 0))
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/Chess_pdt60.png";
	}
	else
	{
		filename = "../res/Chess_plt60.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);

	if (team == BLACK)
	{
		yDirection = -1;
	}
	else
	{
		yDirection = 1;
	}

	render();
}

std::vector<PossibleMove> Pawn::getPhysicallyPossibleMoves(Piece** board)
{
	std::vector<PossibleMove> posible_moves;

	// single forward - only if the place is empty
	Point singleForwardMove = { m_posistion.x, m_posistion.y + yDirection};
	Piece* singleForwadPiece = board[CoordToIndex(singleForwardMove.x, singleForwardMove.y)];
	if (!singleForwadPiece)
		posible_moves.emplace_back(singleForwardMove, this);

	// double forward - only if this is first move and in the way are no pieces
	Point doubleForward = { m_posistion.x, m_posistion.y + 2 * yDirection};
	if (!m_hasMoved && // only if this is firs move
		!singleForwadPiece && // only if the way is empty
		!board[CoordToIndex(doubleForward.x, doubleForward.y)])
	{
		posible_moves.emplace_back(doubleForward, this);
	}

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
				posible_moves.emplace_back(moveVertical, this, MoveType::CAPTURE, toCapture);
			}
		}
	}

	// enpassant
	//TODO

	return posible_moves;
}




