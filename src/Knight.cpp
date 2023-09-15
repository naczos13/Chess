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

std::vector<Point> Knight::getPhysicallyPossiblePositions(Piece** board) const
{
	std::vector<Point> posible_positions;
	return posible_positions;
}