#include "Queen.h"
#include <iostream>

Queen::Queen(Team team, Point pos, SDL_Handler* handler)
	:Piece(team, pos, handler, QUEEN)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/Chess_qdt60.png";
	}
	else
	{
		filename = "../res/Chess_qlt60.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);
	render();
}


void Queen::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK QUEEN" << std::endl;
	}
	else
	{
		std::cout << "WHTIE QUEEN" << std::endl;
	}
}

std::vector<PossibleMove> Queen::calcPossibleMoves(Piece** board, bool checkCheck)
{
	std::vector<PossibleMove> moves;
	/*int dx_copy;
	int dy_copy;
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			dx_copy = dx;
			dy_copy = dy;
			while (board[CoordToIndex(m_pos.x + dx_copy, m_pos.y + dy_copy)] == nullptr)
			{
				moves = simulateMove(moves,
					PossibleMove(m_pos.x + dx_copy, m_pos.y + dy_copy, MoveType::NORMAL),
					getOwnKing(board),
					board,
					checkCheck);
				if (dx_copy < 0)
				{
					dx_copy -= 1;
				}
				else if (dx_copy > 0)
				{
					dx_copy += 1;
				}
				if (dy_copy < 0)
				{
					dy_copy -= 1;
				}
				else if (dy_copy > 0)
				{
					dy_copy += 1;
				}
			}
			if (board[CoordToIndex(m_pos.x + dx_copy, m_pos.y + dy_copy)] != nullptr
				&& (m_pos.x + dx_copy >= 0 && m_pos.x + dx_copy <= 7 && m_pos.y + dy_copy >= 0 && m_pos.y + dy_copy <= 7))
			{
				if (board[CoordToIndex(m_pos.x + dx_copy, m_pos.y + dy_copy)]->getTeam() != m_team)
				{
					moves = simulateMove(moves,
						PossibleMove(m_pos.x + dx_copy, m_pos.y + dy_copy, MoveType::NORMAL),
						getOwnKing(board),
						board,
						checkCheck);
				}
			}
		}
	}*/

	return moves;
}

std::vector<Point> Queen::getPhysicallyPossiblePositions(Piece** board) const
{
	std::vector<Point> posible_positions;
	return posible_positions;
}