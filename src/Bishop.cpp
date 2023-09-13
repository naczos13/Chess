#include "Bishop.h"
#include <iostream>
#include <array>

Bishop::Bishop(Team team, Point pos, SDL_Handler* handler)
	:Piece(team, pos, handler, BISHOP)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "../res/Chess_bdt60.png";
	}
	else
	{
		filename = "../res/Chess_blt60.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);
	render();
}

void Bishop::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK BISHOP" << std::endl;
	}
	else
	{
		std::cout << "WHTIE BISHOP" << std::endl;
	}
}

std::vector<PossibleMove> Bishop::calcPossibleMoves(Piece** board, bool checkCheck)
{
	std::vector<PossibleMove> posible_moves;

	std::vector<Point> posible_positions = getPhysicallyPossiblePositions(board);

	for (const Point& newPosition : posible_positions)
	{
		// simulate the move
		// need to check this because maybe this move can led to own checkmate
		if (!moveMakeMyKingToBeCheck(board, getOwnKing(board), &newPosition, this))
		{
			posible_moves.emplace_back(PossibleMove{ newPosition.x, newPosition.y, MoveType::NORMAL });
		}
	}

	return posible_moves;
}

std::vector<Point> Bishop::getPhysicallyPossiblePositions(Piece** board) const
{
	std::vector<Point> posible_positions;

	// Bishop can move by diagonal
	Point moveDirection[4] = {
		{-1, -1},	// left down
		{-1,  1},	// left up
		{ 1, -1},	// right down
		{ 1,  1}	// right up
	};

	for (const auto [dx, dy] : moveDirection) {
		for (int multiplyer{ 1 }; multiplyer < 8; multiplyer++) { // in each direction the the bishop can possible make 7 moves

			int newX = m_pos.x + dx * multiplyer;
			int newY = m_pos.y + dy * multiplyer;

			// if pass the border, stop checking this direction
			if (newX < 0 || newX > 7 || newY < 0 || newY > 7)
				break;

			// if in the way is piece, 
			if (const Piece* potential_piece = board[CoordToIndex(newX, newY)]) {
				// if the piece is in opossite team add this as possible move
				if (potential_piece->getTeam() != m_team)
				{
					posible_positions.emplace_back(newX, newY);
				}
				// then stop checking this direction
				break;
			}

			// if this is simply empty space, just add it
			posible_positions.emplace_back(newX, newY);
		}
	}

	return posible_positions;
}


