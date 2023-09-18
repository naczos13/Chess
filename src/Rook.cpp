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
	std::vector<PossibleMove> posible_moves;

	// Rook can move vertical and horizontal
	Point moveDirection[4] = {
		{  0,  1},	// up
		{  0, -1},	// down
		{  1,  0},	// right
		{ -1,  0}	// right up
	};

	for (const auto [dx, dy] : moveDirection) {
		for (int multiplyer{ 1 }; multiplyer < 8; multiplyer++) { // in each direction the the bishop can possible make 7 moves

			Point newPosition{ m_posistion.x + dx * multiplyer, m_posistion.y + dy * multiplyer };

			// if pass the border, stop checking this direction
			if (newPosition.x < 0 || newPosition.x > 7 || newPosition.y < 0 || newPosition.y > 7)
				break;

			// if in the way is piece, 
			if (Piece* potential_piece = board[CoordToIndex(newPosition)]) {
				// if the piece is in opossite team add this as possible move
				if (potential_piece->getTeam() != m_team)
				{
					PossibleMove move = PossibleMove(newPosition, this);
					move.addPieceToCapture(potential_piece);
					posible_moves.push_back(move);
				}
				// then stop checking this direction
				break;
			}

			// if this is simply empty space, just add it
			posible_moves.emplace_back(newPosition, this);
		}
	}

	return posible_moves;
}