#pragma once
#include "Piece.h"

class Rook :
    public Piece
{
public:
    // Constructor
    Rook(Team team, Point pos, SDL_Handler* handler);

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;
};



