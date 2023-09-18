#include "Bishop.h"
#include <iostream>
#include <array>

Bishop::Bishop(Team team, Point pos, SDL_Handler* handler, SDL_Texture* texture)
	:Piece(team, pos, handler, BISHOP, texture)
{
	m_handler = handler;
	render();
}

std::vector<PossibleMove> Bishop::getPhysicallyPossibleMoves(Piece** board)
{
	std::vector<PossibleMove> posible_moves;

	// Bishop can move by diagonal
	Point moveDirection[4] = {
		{-1, -1},	// left down
		{-1,  1},	// left up
		{ 1, -1},	// right down
		{ 1,  1}	// right up
	};

	for (const auto [dx, dy] : moveDirection) {
		for (int multiplyer{ 1 }; multiplyer < 8; multiplyer++) { // in each direction the the bishop can possible make 7 moves

			Point newPosition = { m_posistion.x + dx * multiplyer, m_posistion.y + dy * multiplyer };

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


