#include "Rook.h"
#include <iostream>

Rook::Rook(Team team, Point pos, SDL_Handler* handler)
	:Piece(team, pos, handler, ROOK)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/Chess_rdt60.png";
	}
	else
	{
		filename = "../res/Chess_rlt60.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);

	render();
}

void Rook::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK ROOK" << std::endl;
	}
	else
	{
		std::cout << "WHITE ROOK" << std::endl;
	}
}

std::vector<PossibleMove> Rook::calcPossibleMoves(Piece** board, bool checkCheck)
{
	std::vector<PossibleMove> moves;

	/*int dx_copy;
	int dy_copy;
	for (int dx = -1; dx <= 1; dx ++)
	{
		for (int dy = -1; dy <= 1; dy ++)
		{
			if (dy == 0 || dx == 0)
			{
				dx_copy = dx;
				dy_copy = dy;
				while (board[CoordToIndex(m_pos.x + dx_copy, m_pos.y + dy_copy)] == nullptr
					&& (m_pos.x + dx_copy >= 0 && m_pos.x + dx_copy <= 7 && m_pos.y + dy_copy >= 0 && m_pos.y + dy_copy <= 7))
				{
					moves = simulateMove(moves,
									 PossibleMove{ m_pos.x + dx_copy, m_pos.y + dy_copy, MoveType::NORMAL },
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
										 PossibleMove{ m_pos.x + dx_copy, m_pos.y + dy_copy, MoveType::NORMAL },
										 getOwnKing(board),
										 board,
										 checkCheck);
					}
				}
			}
		}
	}*/

	return moves;
}


Rook::~Rook()
{
}


std::vector<Point> Rook::getPhysicallyPossiblePositions(Piece** board) const
{
	std::vector<Point> posible_positions;
	return posible_positions;
}