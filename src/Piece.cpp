#include "Piece.h"
#include "King.h"
#include <string>
#include <iostream>

std::vector<PossibleMove> Piece::pushMove(std::vector<PossibleMove> moveList,
											PossibleMove move,
											King* king,
											Piece* field[8][8],
											bool checkCheck)
{
	if (!checkCheck)
	{
		moveList.push_back(move);
	}
	else
	{
		bool enemyPlace = true;
		king->setCheck(field, king->getPos().first, king->getPos().second);
		Piece* zwisch = &(*field[move.XCoord][move.YCoord]);
		enemyPlace = false;

		if (field[move.XCoord][move.YCoord] != nullptr)
		{
			enemyPlace = true;
			field[move.XCoord][move.YCoord] = nullptr;
		}

		std::swap(field[move.XCoord][move.YCoord], field[m_pos.first][m_pos.second]);
		if (m_type == KING)
		{
			king->setCheck(field, move.XCoord, move.YCoord);
		}
		else
		{
			king->setCheck(field, king->getPos().first, king->getPos().second);
		}
		std::swap(field[move.XCoord][move.YCoord], field[m_pos.first][m_pos.second]);

		if (enemyPlace)
		{
			field[move.XCoord][move.YCoord] = &(*zwisch);
		}
		if (!king->getCheck())
		{
			moveList.push_back(move);
		}
		king->setCheck(field, king->getPos().first, king->getPos().second);
	}
	return moveList;
}

King* Piece::getOwnKing(Piece* field[8][8])
{
	for  (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field[i][j] != nullptr)
			{
				if (field[i][j]->getTeam() == m_team && field[i][j]->getType() == PieceType::KING)
				{
					King* ret = static_cast<King*>(field[i][j]);
					return ret;
				}
			}
		}
	}
	return nullptr;
}

Piece::Piece(Team team, std::pair<int, int> pos, SDL_Handler* handler, PieceType type)
	:m_team(team), m_pos(pos), m_handler(handler), m_texture(NULL), m_hasMoved(false), m_type(type)
{
}

Piece::Piece(const Piece& piece)
	:m_team(piece.m_team), m_pos(piece.m_pos), m_handler(piece.m_handler), m_texture(NULL), m_hasMoved(false), m_type(piece.m_type)
{
}

Piece::~Piece()
{
	SDL_DestroyTexture(m_texture);

	m_handler->undoPieceRender(m_pos.first, m_pos.second);
}


void Piece::render()
{
	SDL_Rect src = {0, 0, 60, 60};
	if (m_handler == nullptr)
	{
		sayMyName();
	}
	SDL_Rect dest = { m_handler->SCREEN_WIDTH / 8 * m_pos.first - 2,
					  m_handler->SCREEN_HEIGHT / 8 * m_pos.second,
					  m_handler->SCREEN_WIDTH / 8,
					  m_handler->SCREEN_HEIGHT / 8 };
	m_handler->DrawRectangle(src, dest, m_texture);
}

void Piece::sayMyName()
{
	std::cout << "No Piece here" << std::endl;
}
