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

std::vector<PossibleMove> Rook::getPhysicallyPossibleMoves(Piece** board)
{
	std::vector<PossibleMove> posible_positions;
	return posible_positions;
}