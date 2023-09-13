#include "King.h"
#include "Pawn.h"
#include <iostream>
#include <stdlib.h>

King::King(Team team, Point pos, SDL_Handler* handler)
	:Piece(team, pos, handler, KING)
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

std::vector<PossibleMove> King::calcPossibleMoves(Piece** board, bool checkCheck)
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
				if (board[CoordToIndex(m_pos.x + dx, m_pos.y + dy)] != nullptr)
				{
					if (board[CoordToIndex(m_pos.x + dx, m_pos.y + dy)]->getTeam() != m_team)
					{
						moves = simulateMove(moves,
							PossibleMove{ m_pos.x + dx, m_pos.y + dy, MoveType::NORMAL },
							getOwnKing(board),
							board,
							checkCheck);
					}
				}
				else
				{
					moves = simulateMove(moves,
									 PossibleMove{ m_pos.x + dx, m_pos.y + dy, MoveType::NORMAL },
									 getOwnKing(board),
									 board,
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
				if (board[CoordToIndex(i, j)] != nullptr)
				{
					if (board[CoordToIndex(i, j)]->getTeam() == m_team && board[CoordToIndex(i, j)]->getType() == ROOK && !board[CoordToIndex(i, j)]->m_hasMoved)
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
						if (board[CoordToIndex(a, j)] == nullptr && board[CoordToIndex(b, j)] == nullptr && board[CoordToIndex(c, j)] == nullptr)
						{
							for (int k = 0; k < 8; k++)
							{
								for (int l = 0; l < 8; l++)
								{
									if (board[CoordToIndex(k, l)] != nullptr)
									{
										if (board[CoordToIndex(k, l)]->getTeam() != m_team)
										{
											std::vector<PossibleMove> notPossible = board[CoordToIndex(k, l)]->getPossibleMoves(board);
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

std::vector<Point> King::getPhysicallyPossiblePositions(Piece** board) const
{
	std::vector<Point> posible_positions;
	return posible_positions;
}

