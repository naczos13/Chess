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
	std::vector<PossibleMove> posible_moves;

	// Queen can move vertical and horizontal and diagonal
	Point moveDirection[8] = {
		{  0,  1},	// up
		{  0, -1},	// down
		{  1,  0},	// right
		{ -1,  0},	// right up
		{ -1, -1},	// left down
		{ -1,  1},	// left up
		{  1, -1},	// right down
		{  1,  1}	// right up
	};

	for (const auto [dx, dy] : moveDirection) {

		int newX = m_posistion.x + dx;
		int newY = m_posistion.y + dy;

		// if pass the border, stop checking this direction
		if (newX < 0 || newX > 7 || newY < 0 || newY > 7)
			continue;

		// if in the way is piece, 
		if (Piece* potential_piece = board[CoordToIndex(newX, newY)]) {
			// if the piece is in opossite team add this as possible move
			if (potential_piece->getTeam() != m_team)
			{
				posible_moves.emplace_back(newX, newY, this, MoveType::CAPTURE, potential_piece);
			}
			// then stop checking this direction
			continue;
		}

		// if this is simply empty space, just add it
		posible_moves.emplace_back(newX, newY, this);
	}

	return posible_moves;
}

