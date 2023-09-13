#pragma once
#include "Piece.h"

class Pawn :
    public Piece
{
public:
    // constructor
    Pawn(Team team, Point pos, SDL_Handler* handler);

    // prints name of piece
    void sayMyName();

    // calculates possible moves
    virtual std::vector<PossibleMove> calcPossibleMoves(Piece** board, bool checkCheck) override;

    // direction the pawn moves
    int m_dy;

    // getter m_enPassant
    std::pair<bool, int> getEnPassant() { return m_enPassant; };

    // set m_enPassant
    void setEnPassant(std::pair<bool, int> modifi) { m_enPassant = modifi; };

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;

    // Just made a double move
    bool m_justMadeDoubleMove;

private:
    // if true, en passant is possible in the int direction
    std::pair<bool, int> m_enPassant;
};

