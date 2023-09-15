#pragma once
#include "Piece.h"
#include "King.h"
class Bishop :
    public Piece
{
public:
    //Constructor
    Bishop(Team team, Point pos, SDL_Handler* handler);

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) const override;
};

