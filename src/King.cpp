#include "King.h"
#include "Rook.h"
#include <iostream>
#include <stdlib.h>
#include <array>

King::King(Team team, Point pos, SDL_Texture* texture)
	:Piece(team, pos, PieceType::KING, texture) {}


bool King::checkIfPiecesBeetweenRookAndKing(Piece** board, Piece* rook)
{
	int startX;
	int endX;
	if (rook->getPosition().x < getPosition().x)
	{
		startX = rook->getPosition().x + 1;
		endX = getPosition().x - 1;
	}
	else
	{
		startX = getPosition().x + 1;
		endX = rook->getPosition().x - 1;
	}

	for (int x = startX; x <= endX; x++)
	{
		if (board[CoordToIndex(x, getPosition().y)])
			return true;
	}

	return false;

}

void King::tryCastle(Piece** board, std::vector<PossibleMove>& posibleMoves)
{
	// Your king can not have moved
	if (m_hasMoved)
		return;

	// Your king can NOT be in check
	//TODO The rucursive bomb :D
	/*for (int i{}; i < 64; i++)
	{
		Piece* potentialEnemy = board[i];
		if (potentialEnemy && potentialEnemy->getTeam() != getTeam())
		{
			if (potentialEnemy->canEliminateKing(board, this))
			{
				return;
			}
		}
	}*/

	// for specific rook
	std::array<Point, 2> RookPositions;
	RookPositions[0] = { 0, getPosition().y };
	RookPositions[1] = { 7, getPosition().y };

	// No pieces can be between the king and rook
	for (Point RookPosition : RookPositions)
	{
		Rook* rook = dynamic_cast<Rook*>(board[CoordToIndex(RookPosition)]);
		// must be Rook
		if (!rook)
			continue;

		// Rook can not be moved
		if (rook->hasAlreadyMoved())
			continue;

		// no piece between rook and king
		if (checkIfPiecesBeetweenRookAndKing(board, rook))
			continue;

		// TODO check if all field in which the king will be moved are in check

		int kingTargetXPositon = rook->getPosition().x > getPosition().x ? 6 : 2;
		int RookTargetXPositon = rook->getPosition().x > getPosition().x ? 5 : 3;
		PossibleMove move(kingTargetXPositon, getPosition().y, this);
		move.castling = true;
		move.PieceToCastle = rook;
		move.RookTargetPosition = { RookTargetXPositon, getPosition().y };
		posibleMoves.push_back(move);
	}
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

	// Castle
	tryCastle(board, posible_moves);

	return posible_moves;
}

