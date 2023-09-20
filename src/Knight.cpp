#include "Knight.h"
#include <iostream>

Knight::Knight(Team team, Point pos, SDL_Texture* texture)
	:Piece(team, pos, PieceType::KNIGHT, texture) {}

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