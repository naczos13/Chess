#pragma once
#include "Piece.h"
class Queen :
    public Piece
{
public:
    Queen(Team team, Point pos, SDL_Texture* texture);

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) override;
};

