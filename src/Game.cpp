#include "Game.h"
#include <iostream>

Game::Game(SDL_Handler* handler)
       :m_turn(Team::WHITE),
        m_handler(handler)
{
    m_handler->prepareBackgroundRender();
    m_handler->loadPiecesTextures();
    createPieces();
    m_handler->refreshTheWindow(pieces);
}

void Game::createPiece(const PieceType& type, const Team& team, const Point& position)
{
    switch (type)
    {
    case PieceType::PAWN:
    {
        pieces.push_back(std::make_unique<Pawn>(team, position, m_handler->textures.at({team, type})));
        break;
    }
    case PieceType::BISHOP:
    {
        pieces.push_back(std::make_unique<Bishop>(team, position, m_handler->textures.at({ team, type })));
        break;
    }
    case PieceType::KNIGHT:
    {
        pieces.push_back(std::make_unique<Knight>(team, position, m_handler->textures.at({ team, type })));
        break;
    }
    case PieceType::ROOK:
    {
        pieces.push_back(std::make_unique<Rook>(team, position, m_handler->textures.at({ team, type })));
        break;
    }
    case PieceType::QUEEN:
    {
        pieces.push_back(std::make_unique<Queen>(team, position, m_handler->textures.at({ team, type })));
        break;
    }
    case PieceType::KING:
    {
        pieces.push_back(std::make_unique<King>(team, position, m_handler->textures.at({ team, type })));
        if (team == Team::BLACK)
        {
            blackKing = static_cast<King*>(pieces.back().get());
        }
        else if (team == Team::WHITE)
        {
            whiteKing = static_cast<King*>(pieces.back().get());
        }
        break;
    }
    default:
        std::cout << "unrecognised Piece type" << std::endl;
    }

    m_board[CoordToIndex(position)] = pieces.back().get();
}

void Game::createPieces()
{
    Point position;

    // Create pawns
    for (int x{}; x < 8; x++) {
        // white
        createPiece(PieceType::PAWN, Team::WHITE, { x, 1 });
        // black
        createPiece(PieceType::PAWN, Team::BLACK, { x, 6 });
    }

    // Create Rook
    // White
    createPiece(PieceType::ROOK, Team::WHITE, { 0, 0 });
    createPiece(PieceType::ROOK, Team::WHITE, { 7, 0 });
    // Black
    createPiece(PieceType::ROOK, Team::BLACK, { 0, 7 });
    createPiece(PieceType::ROOK, Team::BLACK, { 7, 7 });

    // Create KNIGHT
    // White
    createPiece(PieceType::KNIGHT, Team::WHITE, { 1, 0 });
    createPiece(PieceType::KNIGHT, Team::WHITE, { 6, 0 });
    // Black
    createPiece(PieceType::KNIGHT, Team::BLACK, { 1, 7 });
    createPiece(PieceType::KNIGHT, Team::BLACK, { 6, 7 });

    // Create BISHOP
    // White
    createPiece(PieceType::BISHOP, Team::WHITE, { 2, 0 });
    createPiece(PieceType::BISHOP, Team::WHITE, { 5, 0 });
    // Black
    createPiece(PieceType::BISHOP, Team::BLACK, { 2, 7 });
    createPiece(PieceType::BISHOP, Team::BLACK, { 5, 7 });

    // Create KING
    // White
    createPiece(PieceType::KING, Team::WHITE, { 4, 0 });
    // Black
    createPiece(PieceType::KING, Team::BLACK, { 4, 7 });

    // Create QUEEN
    // White
    createPiece(PieceType::QUEEN, Team::WHITE, { 3, 0 });
    // Black
    createPiece(PieceType::QUEEN, Team::BLACK, { 3, 7 });

}

Game::~Game()
{

}

Piece* Game::getPieceByPosition(int row, int col)
{
    return m_board[row * 8 + col];
}

void Game::disableUnusedEnPassant()
{
    for (const auto& piece : pieces)
    {
        if (piece->getTeam() != getTurn())
            continue;

        if (piece->getType() != PieceType::PAWN)
            continue;

        Pawn* pawn = static_cast<Pawn*>(piece.get());
        pawn->canBeCaptureByEnPassantByPiece = nullptr;
    }
}

void Game::move(PossibleMove& move)
{
    if (move.capturePiece) {
        Piece* capturedPiece = move.PieceToCapture;
        capturedPiece->deactivate();
        m_board[CoordToIndex(capturedPiece->getPosition())] = nullptr;
    }

    Piece* pieceToMove = move.PieceToMove;
    Point oldPlace = pieceToMove->getPosition();
    Point placeToMove = { move.XCoord, move.YCoord };
    pieceToMove->setPosition(placeToMove);
    pieceToMove->setPieceIsMoved();
    m_board[CoordToIndex(placeToMove)] = pieceToMove;
    m_board[CoordToIndex(oldPlace)] = nullptr;

    m_handler->refreshTheWindow(pieces);
   

    disableUnusedEnPassant();

    if (move.doubleMove)
    {
        for (auto pieceThatCanMakeEnPassant : move.PiecesToInfluence)
        {
            if (Pawn* pawnThatCanMakeEnPassant = dynamic_cast<Pawn*>(pieceThatCanMakeEnPassant))
                pawnThatCanMakeEnPassant->canBeCaptureByEnPassantByPiece = move.PieceToMove;
        }
    }

    if (move.isPromoteThePawn())
    {
        std::cout << "Pawn To promote" << std::endl;
        exchange(move);
    }

    changeTurn();

    gameState();
}

void Game::exchange(PossibleMove& move)
{
    PieceType pieceType = m_handler->chooseNewPiece(getTurn());

    createPiece(pieceType, getTurn(), move.getDestination());
    move.PieceToMove->deactivate();
    
    m_handler->refreshTheWindow(pieces);
}

void Game::changeTurn()
{
    // Change the turn
    if (m_turn == Team::BLACK)
    {
        m_turn = Team::WHITE;
    }
    else
    {
        m_turn = Team::BLACK;
    }
}

void Game::gameState()
{
    // when is game over?
    // when the team which turn is have no possible moves
    // how to check it?
    // Get the current team
    // iterate over the board and check if any piece can make a move

    for (int i{}; i < 64; i++)
    {
        if (Piece* piece = m_board[i])
        {
            if (piece->getTeam() == m_turn && piece->isActive())
            {
                if (!piece->getPossibleMoves(m_board).empty())
                {
                    return;
                }
            }
        }
    }

    //TODO How to check for a draft

    if (m_turn == Team::WHITE)
    {
        std::cout << "Black wins!\n";
    }
    else
    {
        std::cout << "White wins!\n";
    }

    std::cout << "Press Enter to exit the game";

    std::cin.get();
    SDL_Event quitEvent;
    quitEvent.type = SDL_QUIT;
    SDL_PushEvent(&quitEvent);
}

std::optional<PossibleMove> Game::GetValidMove(const Point& endPoint, const std::vector<PossibleMove>& possibleMoves) const
{
    for (const auto& move : possibleMoves) {
        if (move.XCoord == endPoint.x && move.YCoord == endPoint.y)
        {
            return move;
        }
    }
    return std::nullopt;
}
