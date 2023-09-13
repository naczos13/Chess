#pragma once
#include "Piece.h"
class Queen :
    public Piece
{
public:
    Queen(Team team, Point pos, SDL_Handler* handler);

    // prints name of piece
    void sayMyName();

    // calculates the possible moves
    std::vector<PossibleMove> calcPossibleMoves(Piece** field, bool checkCheck) override;

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** field) const override;
};

