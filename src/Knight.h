#pragma once
#include "Piece.h"

class Knight :
    public Piece
{
public:
    Knight(Team team, Point pos, SDL_Handler* handler);

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) override;
};

