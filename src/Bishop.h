#pragma once
#include "Piece.h"
#include "King.h"
class Bishop :
    public Piece
{
public:
    //Constructor
    Bishop(Team team, Point pos, SDL_Handler* handler);

    // used to debug some stuff
    void sayMyName();

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;
};

