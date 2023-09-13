#pragma once
#include "Piece.h"

class King :
    public Piece
{
public:
    King(Team team, Point pos, SDL_Handler* handler);

    // calls type and color to debug
    void sayMyName();

    // calculates the possible moves
    std::vector<PossibleMove> calcPossibleMoves(Piece** board, bool checkCheck) override;

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;

};

