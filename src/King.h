#pragma once
#include "Piece.h"

class King :
    public Piece
{
public:
    King(Team team, Point pos, SDL_Handler* handler);

    // calls type and color to debug
    void sayMyName();

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;

};

