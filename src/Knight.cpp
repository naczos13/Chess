#include "Knight.h"
#include <iostream>

Knight::Knight(Team team, Point pos, SDL_Handler* handler)
	:Piece(team, pos, handler, KNIGHT)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/Chess_ndt60.png";
	}
	else
	{
		filename = "../res/Chess_nlt60.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);
	render();
}

void Knight::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK KNIGHT" << std::endl;
	}
	else
	{
		std::cout << "WHTIE KNIGHT" << std::endl;
	}
}

void Knight::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<PossibleMove> moves;
	
	for (int dx = -2; dx <= 2 ; dx += 4)
	{
		for (int dy = -1; dy <= 1; dy += 2)
		{
			if (m_pos.x + dx >= 0 && m_pos.x + dx <= 7 && m_pos.y + dy >= 0 && m_pos.y + dy <= 7)
			{
				if (field[m_pos.x + dx][m_pos.y + dy] == nullptr)
				{
					moves = pushMove(moves,
						PossibleMove{ m_pos.x + dx, m_pos.y + dy, MoveType::NORMAL },
									 getOwnKing(field),
									 field,
									 checkCheck);
				}
				else if (field[m_pos.x + dx][m_pos.y + dy] != nullptr)
				{
					if (field[m_pos.x + dx][m_pos.y + dy]->getTeam() != m_team)
					{
						moves = pushMove(moves,
							PossibleMove{ m_pos.x + dx, m_pos.y + dy, MoveType::NORMAL },
										 getOwnKing(field),
										 field,
										 checkCheck);
					}
				}
			}
		}
	}

	for (int dy = -2; dy <= 2; dy += 4)
	{
		for (int dx = -1; dx <= 1; dx += 2)
		{
			if (m_pos.x + dx >= 0 && m_pos.x + dx <= 7 && m_pos.y + dy >= 0 && m_pos.y + dy <= 7)
			{
				if (field[m_pos.x + dx][m_pos.y + dy] == nullptr)
				{
					moves = pushMove(moves,
						PossibleMove{ m_pos.x + dx, m_pos.y + dy, MoveType::NORMAL },
						getOwnKing(field),
						field,
						checkCheck);
				}
				else if (field[m_pos.x + dx][m_pos.y + dy] != nullptr)
				{
					if (field[m_pos.x + dx][m_pos.y + dy]->getTeam() != m_team)
					{
						moves = pushMove(moves,
							PossibleMove{ m_pos.x + dx, m_pos.y + dy, MoveType::NORMAL },
							getOwnKing(field),
							field,
							checkCheck);
					}
				}
			}
		}
	}

	m_possibleMoves = moves;
}