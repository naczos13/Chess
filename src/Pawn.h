#pragma once
#include "Piece.h"

class Pawn :
    public Piece
{
public:
    // constructor
    Pawn(Team team, Point pos, SDL_Texture* texture);

    // direction the pawn moves
    int yDirection;

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<PossibleMove> getPhysicallyPossibleMoves(Piece** board) override;

    bool canEnPassant(Piece* toCapture);

    Piece* canBeCaptureByEnPassantByPiece = nullptr;

    void tryToMakeDoubleMove(Piece** board, std::vector<PossibleMove>& possibleMoves, Piece* singleForwardPiece);
};

