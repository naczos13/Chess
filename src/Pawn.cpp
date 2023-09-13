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
		m_dy = -1;
	}
	else
	{
		m_dy = 1;
	}

	render();
}

void Pawn::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK PAWN" << std::endl;
	}
	else
	{
		std::cout << "WHITE PAWN" << std::endl;
	}
}

std::vector<PossibleMove> Pawn::calcPossibleMoves(Piece** board, bool checkCheck)
{
	std::vector<PossibleMove> posible_moves;

	std::vector<Point> posible_positions = getPhysicallyPossiblePositions(board);

	for (const Point& newPosition : posible_positions)
	{
		// simulate the move
		// need to check this because maybe this move can led to own checkmate
		if (!moveMakeMyKingToBeCheck(board, getOwnKing(board), &newPosition, this))
		{
			posible_moves.emplace_back(PossibleMove{ newPosition.x, newPosition.y, MoveType::NORMAL });
		}
	}

	return posible_moves;
}

std::vector<Point> Pawn::getPhysicallyPossiblePositions(Piece** board) const
{
	std::vector<Point> posible_positions;

	// single forward - only if the place is empty
	Point singleForwardMove = { m_pos.x, m_pos.y + m_dy };
	Piece* singleForwadPiece = board[CoordToIndex(singleForwardMove.x, singleForwardMove.y)];
	if (!singleForwadPiece)
		posible_positions.emplace_back(singleForwardMove);

	// double forward - only if this is first move and in the way are no pieces
	Point doubleForward = { m_pos.x, m_pos.y + 2 * m_dy };
	if (!m_hasMoved && // only if this is firs move
		!singleForwadPiece && // only if the way is empty
		!board[CoordToIndex(doubleForward.x, doubleForward.y)])
	{
		posible_positions.emplace_back(doubleForward);
	}

	// capture - only if still in bounds and there is a enemy piece
	std::array<int, 2> verticalDirection{ -1, 1 };
	for (const int dx : verticalDirection)
	{
		Point moveVertical{ m_pos.x + dx, m_pos.y + m_dy };
		if (moveVertical.x >= 0 && moveVertical.x < 8)
		{
			Piece* toCapture = board[CoordToIndex(moveVertical.x, moveVertical.y)];
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




