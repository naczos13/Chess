#pragma once
#include "Piece.h"

class Knight :
    public Piece
{
public:
    Knight(Team team, Point pos, SDL_Handler* handler);

    // calls name an
    void sayMyName();

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;
};

