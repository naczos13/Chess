#pragma once
#include "Piece.h"
class Queen :
    public Piece
{
public:
    Queen(Team team, Point pos, SDL_Handler* handler);

    // prints name of piece
    void sayMyName();

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;
};

