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

std::vector<PossibleMove> Knight::getPhysicallyPossibleMoves(Piece** board)
{
	std::vector<PossibleMove> posible_moves;

	// Rook can move vertical and horizontal
	Point moveDirection[8] = {
		{ -2, -1},	// long up short left
		{ -2,  1},	// long up short right
		{  2, -1},	// long down short left
		{  2,  1},	// long down short right

		{ -1, -2},	// short up long left
		{ -1,  2},	// short up long right
		{  1, -2},	// long down long left
		{  1,  2}	// long down long right
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