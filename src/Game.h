#pragma once
#include "SDL_Handler.h"
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include <memory>
#include <optional>
#include <array>
#include <unordered_map>

// A hash function used to hash a pair of any kind
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<int>{}(static_cast<int>(p.first) + 1000);
        auto hash2 = std::hash<T2>{}(p.second);

        return hash1 + hash2;
    }
};

using PieceKey = std::pair<Team, PieceType>;
using TextureMap = std::unordered_map<PieceKey, SDL_Texture*, hash_pair>;

class Game
{
public:
	// Constructor
	Game(SDL_Handler* handler);

    // Destructor
    ~Game();

	// returns a the Piece in board (row, col)
    Piece* getPieceByPosition(int row, int col);

    //Moves a piece
    void move(PossibleMove& move);

    // returns m_turn
    Team getTurn() { return m_turn; };

    // return the move if exist in the pool of possible moves
    std::optional<PossibleMove> GetValidMove(const Point& endPoint, const std::vector<PossibleMove>& possibleMoves) const;

    // light up the possible Moves
    void renderPossibleMoves(const std::vector<PossibleMove>& possible);

    // undos the renderPossibleMoves function
    void undoRenderPossibleMoves(const std::vector<PossibleMove>& possible);

    void createPieces();

    void disableUnusedEnPassant();

    void changeTurn();

    void createPiece(const PieceType& type, const Team& team, const Point& position);

    // 2D board array, every Position has got a Team and a piece
    std::array<Piece*, 64> board;
    Piece** m_board = board.data();

private: 

    // exchange move
    void exchange(PossibleMove& move);

    // Background filename
    std::string m_backgroundFilename;

    // Decides whether if its black or whites turn
    Team m_turn{};

    // handler
    SDL_Handler* m_handler;

    // if true, disable en Passant! if false, dont
    bool m_checkEnPassant;

    // checks current game state, determines winner or remis
    void gameState();

    // Own the pieces, and ensure their release when game over
    std::vector<std::unique_ptr<Piece>> pieces;

    King* blackKing;
    King* whiteKing;

    TextureMap textures;

    void loadPiecesTextures();

};

