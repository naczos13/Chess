#include "Pawn.h"
#include <iostream>
#include <list>

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

void Pawn::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<PossibleMove> moves;

	if (m_pos.y + m_dy == 0 || m_pos.y + m_dy == 7)
	{
		if (field[m_pos.x][m_pos.y + m_dy] == nullptr)
		{
			moves = pushMove(moves,
				PossibleMove{ m_pos.x, m_pos.y + m_dy, MoveType::NEWPIECE },
				getOwnKing(field),
				field,
				checkCheck);
		}
	}
	else
	{
		if (field[m_pos.x][m_pos.y + m_dy] == nullptr)
		{
			moves = pushMove(moves,
				PossibleMove{ m_pos.x, m_pos.y + m_dy, MoveType::NORMAL },
				getOwnKing(field),
				field,
				checkCheck);
		}
	}

	if ((m_pos.y + 2 * m_dy >= 0) && (m_pos.y + 2 * m_dy <= 7))
	{
		if (field[m_pos.x][m_pos.y + 2 * m_dy] == nullptr && !m_hasMoved)
		{
			moves = pushMove(moves,
				PossibleMove(m_pos.x, m_pos.y + 2 * m_dy, MoveType::NORMAL),
				getOwnKing(field),
				field,
				checkCheck);
		}
	}

	if (m_pos.x + 1 <= 7)
	{
		if (field[m_pos.x + 1][m_pos.y + m_dy] != nullptr)
		{
			if (field[m_pos.x + 1][m_pos.y + m_dy]->getTeam() != m_team)
			{
				if (m_pos.y + m_dy == 0 || m_pos.y + m_dy == 7)
				{
					moves = pushMove(moves,
						PossibleMove(m_pos.x + 1, m_pos.y + m_dy, MoveType::NEWPIECE),
						getOwnKing(field),
						field,
						checkCheck);
				}
				else
				{
					moves = pushMove(moves,
						PossibleMove(m_pos.x + 1, m_pos.y + m_dy, MoveType::NORMAL),
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}
	if (m_pos.x - 1 >= 0)
	{
		if (field[m_pos.x - 1][m_pos.y + m_dy] != nullptr)
		{
			if (field[m_pos.x - 1][m_pos.y + m_dy]->getTeam() != m_team)
			{
				if (m_pos.y + m_dy == 0 || m_pos.y + m_dy == 7)
				{
					moves = pushMove(moves,
						PossibleMove(m_pos.x - 1, m_pos.y + m_dy, MoveType::NEWPIECE),
						getOwnKing(field),
						field,
						checkCheck);
				}
				else
				{
					moves = pushMove(moves,
						PossibleMove(m_pos.x - 1, m_pos.y + m_dy, MoveType::NORMAL),
						getOwnKing(field),
						field,
						checkCheck);
				}
			}
		}
	}

	if (m_enPassant == std::pair<bool, int>(true, -1))
	{
		moves = pushMove(moves,
			PossibleMove(m_pos.x + 1, m_pos.y + m_dy, MoveType::ENPASSANT),
			getOwnKing(field),
			field,
			checkCheck);
	}
	if (m_enPassant == std::pair<bool, int>(true, 1))
	{
		moves = pushMove(moves,
			PossibleMove(m_pos.x - 1, m_pos.y + m_dy, MoveType::ENPASSANT),
			getOwnKing(field),
			field,
			checkCheck);
	}
	m_possibleMoves = moves;
}






