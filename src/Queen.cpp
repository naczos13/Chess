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

std::vector<Point> Queen::getPhysicallyPossiblePositions(Piece** board) const
{
	std::vector<Point> posible_positions;
	return posible_positions;
}