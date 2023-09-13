#include "King.h"
#include "Pawn.h"
#include <iostream>
#include <stdlib.h>

King::King(Team team, Point pos, SDL_Handler* handler)
	:Piece(team, pos, handler, KING), m_check(false)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/Chess_kdt60.png";
	}
	else
	{
		filename = "../res/Chess_klt60.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);

	render();
}

void King::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK KING" << std::endl;
	}
	else
	{
		std::cout << "WHTIE KING" << std::endl;
	}
}

std::vector<PossibleMove> King::calcPossibleMoves(Piece** field, bool checkCheck)
{
	std::vector<PossibleMove> moves;
	/*bool castles = true;
	bool enemyPlace = false;

	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			if (m_pos.x + dx >= 0 && m_pos.x + dx <= 7 && m_pos.y + dy >= 0 && m_pos.y + dy <= 7)
			{
				if (field[CoordToIndex(m_pos.x + dx, m_pos.y + dy)] != nullptr)
				{
					if (field[CoordToIndex(m_pos.x + dx, m_pos.y + dy)]->getTeam() != m_team)
					{
						moves = simulateMove(moves,
							PossibleMove{ m_pos.x + dx, m_pos.y + dy, MoveType::NORMAL },
							getOwnKing(field),
							field,
							checkCheck);
					}
				}
				else
				{
					moves = simulateMove(moves,
									 PossibleMove{ m_pos.x + dx, m_pos.y + dy, MoveType::NORMAL },
									 getOwnKing(field),
									 field,
									 checkCheck);
				}
			}
		}
	}

	if (!m_hasMoved)
	{
		for (int i = 0; i <= 7; i += 7)
		{
			for (int j = 0; j <= 7; j += 7)
			{
				castles = true;
				if (field[CoordToIndex(i, j)] != nullptr)
				{
					if (field[CoordToIndex(i, j)]->getTeam() == m_team && field[CoordToIndex(i, j)]->getType() == ROOK && !field[CoordToIndex(i, j)]->m_hasMoved)
					{
						int a, b, c;
						if (i == 0)
						{
							a = 1;
							b = 2;
							c = 3;
						}
						else
						{
							a = 5;
							b = 6;
							c = 6;
						}
						if (field[CoordToIndex(a, j)] == nullptr && field[CoordToIndex(b, j)] == nullptr && field[CoordToIndex(c, j)] == nullptr)
						{
							for (int k = 0; k < 8; k++)
							{
								for (int l = 0; l < 8; l++)
								{
									if (field[CoordToIndex(k, l)] != nullptr)
									{
										if (field[CoordToIndex(k, l)]->getTeam() != m_team)
										{
											std::vector<PossibleMove> notPossible = field[CoordToIndex(k, l)]->getPossibleMoves(field);
											for (const auto& [XCoord, YCoord, MoveType] : notPossible)
											{
												if (i == 0)
												{
													if ((XCoord == 4 && YCoord == j)
														|| (XCoord == 2 && YCoord == j)
														|| (XCoord == 3 && YCoord == j))
													{
														castles = false;
													}
												}
												else
												{
													if ((XCoord == 5 && YCoord == j)
														|| (XCoord == 6 && YCoord == j)
														|| (XCoord == 4 && YCoord == j))
													{
														castles = false;
													}
												}
											}
										}
									}
								}
							}
							if (castles)
							{
								moves.push_back(PossibleMove{ i, j, MoveType::CASTLE });
							}
						}
					}
				}
			}
		}
	}*/
	return moves;
}

void King::setCheck(Piece** field, int x, int y)
{
	bool check = false;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field[CoordToIndex(i, j)] != nullptr)
			{
				if (field[CoordToIndex(i, j)]->getTeam() != m_team)
				{
					if (field[CoordToIndex(i, j)]->getType() == KING)
					{
						if (abs(field[CoordToIndex(i, j)]->getPos().x - x) <= 1 && abs(field[CoordToIndex(i, j)]->getPos().y - y) <= 1)
						{
							check = true;
						}

					}
					else if (field[CoordToIndex(i, j)]->getType() == PAWN)
					{
						int dy_pawn;
						if (field[CoordToIndex(i, j)]->getTeam() == WHITE)
						{
							dy_pawn = 1;
						}
						else
						{
							dy_pawn = -1;
						}
						if ((x == field[CoordToIndex(i, j)]->getPos().x + 1 || x == field[CoordToIndex(i, j)]->getPos().x - 1) && y == field[CoordToIndex(i, j)]->getPos().y + dy_pawn)
						{
							check = true;
						}
					}
					else
					{
						field[CoordToIndex(i, j)]->calcPossibleMoves(field, false);
						std::vector<PossibleMove> notPossible = field[CoordToIndex(i, j)]->getPossibleMoves(field);
						for (const auto& [XCoord, YCoord, MoveType] : notPossible)
						{
							if (XCoord == x && YCoord == y)
							{
								check = true;
							}
						}
					}
				}
			}
		}
	}
	
	m_check = check;
}

std::vector<Point> King::getPhysicallyPossiblePositions(Piece** field) const
{
	std::vector<Point> posible_positions;
	return posible_positions;
}

