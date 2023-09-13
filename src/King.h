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

    // controls whether this piece is checked or not
    void setCheck(Piece** board, int x, int y);

    // get check
    bool getCheck() { return m_check; };

    // Get the vector of Physically Possible Positions in next move, without checking for checkmate
    std::vector<Point> getPhysicallyPossiblePositions(Piece** board) const override;

private:

    // true, if the king is on any of the other teams movelist's
    bool m_check;
};

