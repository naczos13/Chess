#pragma once
#include "Piece.h"

class Rook :
    public Piece
{
public:
    // Constructor
    Rook(Team team, Point pos, SDL_Handler* handler);

    // prints name of piece
    void sayMyName();

    // Destructor
    ~Rook();

    // calculates the possible moves
    std::vector<PossibleMove> calcPossibleMoves(Piece** board, bool checkCheck) override;

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;
};



