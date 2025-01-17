#pragma once
#include "Piece.h"

class King :
    public Piece
{
public:
    King(Team team, Point pos, SDL_Texture* texture);

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) override;

    void tryCastle(Piece** board, std::vector<PossibleMove>& posibleMoves);

    bool checkIfPiecesBeetweenRookAndKing(Piece** board, Piece* rook);
};

