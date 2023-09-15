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

std::vector<PossibleMove> Pawn::getPhysicallyPossibleMoves(Piece** board) const
{
	std::vector<PossibleMove> posible_positions;

	// single forward - only if the place is empty
	PossibleMove singleForwardMove = { m_posistion.x, m_posistion.y + yDirection, MoveType::NORMAL};
	Piece* singleForwadPiece = board[CoordToIndex(singleForwardMove.XCoord, singleForwardMove.YCoord)];
	if (!singleForwadPiece)
		posible_positions.emplace_back(singleForwardMove);

	// double forward - only if this is first move and in the way are no pieces
	PossibleMove doubleForward = { m_posistion.x, m_posistion.y + 2 * yDirection , MoveType::NORMAL};
	if (!m_hasMoved && // only if this is firs move
		!singleForwadPiece && // only if the way is empty
		!board[CoordToIndex(doubleForward.XCoord, doubleForward.YCoord)])
	{
		posible_positions.emplace_back(doubleForward);
	}

	// capture - only if still in bounds and there is a enemy piece
	std::array<int, 2> verticalDirection{ -1, 1 };
	for (const int dx : verticalDirection)
	{
		PossibleMove moveVertical{ m_posistion.x + dx, m_posistion.y + yDirection , MoveType::NORMAL };
		if (moveVertical.XCoord >= 0 && moveVertical.XCoord < 8)
		{
			Piece* toCapture = board[CoordToIndex(moveVertical.XCoord, moveVertical.YCoord)];
			if (toCapture && toCapture->getTeam() != getTeam())
			{
				posible_positions.push_back(moveVertical);
			}
		}
	}

	// enpassant
	//TODO

	return posible_positions;
}




