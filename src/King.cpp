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

std::vector<PossibleMove> King::getPhysicallyPossibleMoves(Piece** board)
{
	std::vector<PossibleMove> posible_positions;
	return posible_positions;
}

