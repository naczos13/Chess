#pragma once
#include "Piece.h"

class Rook :
    public Piece
{
public:
    // Constructor
    Rook(Team team, Point pos, SDL_Handler* handler, SDL_Texture* texture);

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) override;
};



