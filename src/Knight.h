#pragma once
#include "Piece.h"

class Knight :
    public Piece
{
public:
    Knight(Team team, Point pos, SDL_Handler* handler);

    // calls name an
    void sayMyName();

    // calculates the possible moves
    std::vector<PossibleMove> calcPossibleMoves(Piece** board, bool checkCheck) override;

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;
};

