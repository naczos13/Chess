#include "King.h"
#include "Pawn.h"
#include <iostream>
#include <stdlib.h>

King::King(Team team, Point pos, SDL_Texture* texture)
	:Piece(team, pos, PieceType::KING, texture)
{
	//render();
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

		Point newPosition{ m_posistion.x + dx, m_posistion.y + dy };

		// if pass the border, stop checking this direction
		if (newPosition.x < 0 || newPosition.x > 7 || newPosition.y < 0 || newPosition.y > 7)
			continue;

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
			continue;
		}

		// if this is simply empty space, just add it
		posible_moves.emplace_back(newPosition, this);
	}

	return posible_moves;
}

