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

std::vector<PossibleMove> Queen::getPhysicallyPossibleMoves(Piece** board)
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
		for (int multiplyer{ 1 }; multiplyer < 8; multiplyer++) { // in each direction the the bishop can possible make 7 moves

			int newX = m_posistion.x + dx * multiplyer;
			int newY = m_posistion.y + dy * multiplyer;

			// if pass the border, stop checking this direction
			if (newX < 0 || newX > 7 || newY < 0 || newY > 7)
				break;

			// if in the way is piece, 
			if (Piece* potential_piece = board[CoordToIndex(newX, newY)]) {
				// if the piece is in opossite team add this as possible move
				if (potential_piece->getTeam() != m_team)
				{
					posible_moves.emplace_back(newX, newY, this, MoveType::CAPTURE, potential_piece);
				}
				// then stop checking this direction
				break;
			}

			// if this is simply empty space, just add it
			posible_moves.emplace_back(newX, newY, this);
		}
	}

	return posible_moves;
}