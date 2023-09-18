#include "Game.h"
#include <iostream>

Game::Game(SDL_Handler* handler)
       :m_turn(Team::WHITE),
        m_handler(handler),
        m_checkEnPassant(true)
{
    createPieces();
}

void Game::createPiece(const PieceType& type, const Team& team, const Point& position)
{
    switch (type)
    {
    case PieceType::PAWN:
    {
        pieces.push_back(std::make_unique<Pawn>(team, position, m_handler));
        break;
    }
    case PieceType::BISHOP:
    {
        pieces.push_back(std::make_unique<Bishop>(team, position, m_handler));
        break;
    }
    case PieceType::KNIGHT:
    {
        pieces.push_back(std::make_unique<Knight>(team, position, m_handler));
        break;
    }
    case PieceType::ROOK:
    {
        pieces.push_back(std::make_unique<Rook>(team, position, m_handler));
        break;
    }
    case PieceType::QUEEN:
    {
        pieces.push_back(std::make_unique<Queen>(team, position, m_handler));
        break;
    }
    case PieceType::KING:
    {
        pieces.push_back(std::make_unique<King>(team, position, m_handler));
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
        pawn->m_canBeCaptureByEnPassant = nullptr;
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
    pieceToMove->m_hasMoved = true;
    m_board[CoordToIndex(placeToMove)] = pieceToMove;
    m_board[CoordToIndex(oldPlace)] = nullptr;

    m_handler->renderBackground();
    for (const auto& piece : pieces)
    {
        piece->render();
    }

    disableUnusedEnPassant();

    if (move.doubleMove)
    {
        for (auto pieceThatCanMakeEnPassant : move.PiecesToInfluence)
        {
            if (Pawn* pawnThatCanMakeEnPassant = dynamic_cast<Pawn*>(pieceThatCanMakeEnPassant))
                pawnThatCanMakeEnPassant->m_canBeCaptureByEnPassant = move.PieceToMove;
        }
    }

    changeTurn();

    gameState();
}

void Game::exchange(int xStart, int yStart, int xEnd, int yEnd)
{
    SDL_Texture* text_rook = m_handler->loadImage("../res/Chess_rlt60.png");
    SDL_Texture* text_knight = m_handler->loadImage("../res/Chess_nlt60.png");
    SDL_Texture* text_bishop = m_handler->loadImage("../res/Chess_blt60.png");
    SDL_Texture* text_queen = m_handler->loadImage("../res/Chess_qlt60.png");
    int y_draw = 0;
    Team team = Team::WHITE;

    if (m_board[CoordToIndex(xStart, yStart)]->getTeam() == Team::BLACK)
    {
        text_rook = m_handler->loadImage("../res/Chess_rdt60.png");
        text_knight = m_handler->loadImage("../res/Chess_ndt60.png");
        text_bishop = m_handler->loadImage("../res/Chess_bdt60.png");
        text_queen = m_handler->loadImage("../res/Chess_qdt60.png");
        y_draw = 3 * m_handler->SCREEN_HEIGHT / 4;
        team = Team::BLACK;
    }

    SDL_SetRenderDrawColor(m_handler->m_renderer, 155, 103, 60, 255);
    SDL_Rect rectangle = {0,
                          y_draw,
                          m_handler->SCREEN_WIDTH / 4,
                          m_handler->SCREEN_HEIGHT / 4 };
    SDL_RenderFillRect(m_handler->m_renderer, &rectangle);
    SDL_Rect src = { 0, 0, 60, 60 };
    m_handler->DrawRectangle(src, rectangle, text_rook);

    SDL_SetRenderDrawColor(m_handler->m_renderer, 255, 255, 255, 255);
    rectangle.x = m_handler->SCREEN_WIDTH / 4;
    SDL_RenderFillRect(m_handler->m_renderer, &rectangle);
    m_handler->DrawRectangle(src, rectangle, text_knight);

    SDL_SetRenderDrawColor(m_handler->m_renderer, 155, 103, 60, 255);
    rectangle.x = 2 * m_handler->SCREEN_WIDTH / 4;
    SDL_RenderFillRect(m_handler->m_renderer, &rectangle);
    m_handler->DrawRectangle(src, rectangle, text_bishop);

    SDL_SetRenderDrawColor(m_handler->m_renderer, 255, 255, 255, 255);
    rectangle.x = 3 * m_handler->SCREEN_WIDTH / 4;
    SDL_RenderFillRect(m_handler->m_renderer, &rectangle);
    m_handler->DrawRectangle(src, rectangle, text_queen);

    bool quit = false;
    int x = -1;
    int y = -1;

    Piece* clickedPiece = nullptr;

    std::cout << m_handler;
    
    while (quit == false)
    {
        while (SDL_PollEvent(&m_handler->m_event))
        {
            if (m_handler->m_event.type == SDL_QUIT)
            {
                quit = true;
            }

            if (m_handler->m_event.type == SDL_MOUSEBUTTONDOWN)
            {
                x = m_handler->m_event.button.x / 160;
                y = m_handler->m_event.button.y / 160;
                
                if (y >= y_draw / 160 && y < y_draw / 160 + 1)
                {
                    if (x < m_handler->SCREEN_WIDTH / 640)
                    {
                        clickedPiece = new Rook(team ,Point(xEnd, yEnd), m_handler);
                    }
                    else if (x < 2 * m_handler->SCREEN_WIDTH / 640)
                    {
                        clickedPiece = new Knight(team ,Point(xEnd, yEnd), m_handler);
                    }
                    else if (x < 3 * m_handler->SCREEN_WIDTH / 640)
                    {
                        clickedPiece = new Bishop(team ,Point(xEnd, yEnd), m_handler);
                    }
                    else if (x <= 4 * m_handler->SCREEN_WIDTH / 640)
                    {
                        clickedPiece = new Queen(team ,Point(xEnd, yEnd), m_handler);
                    }
                    std::cout << x << " " << m_handler->SCREEN_WIDTH / 640 << std::endl;
                }
            }
            
            if (m_handler->m_event.type == SDL_MOUSEBUTTONUP && clickedPiece != nullptr)
            {
                quit = true;
            }
        }
    }

    m_board[CoordToIndex(xEnd, yEnd)] = clickedPiece;
    m_board[CoordToIndex(xStart, yStart)] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_handler->renderBackground();
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_board[CoordToIndex(i, j)] != nullptr)
            {
                m_board[CoordToIndex(i, j)]->render();
            }
        }
    }
    
    SDL_DestroyTexture(text_rook);
    SDL_DestroyTexture(text_bishop);
    SDL_DestroyTexture(text_knight);
    SDL_DestroyTexture(text_queen);
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
            if (piece->getTeam() == m_turn)
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

void Game::renderPossibleMoves(const std::vector<PossibleMove>& possible)
{
    SDL_Rect rectangle;
    for (const auto& move : possible) {
        if ((move.XCoord % 2 == 0 && move.YCoord % 2 == 1) || (move.XCoord % 2 == 1 && move.YCoord % 2 == 0))
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 0, 134, 139, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 164, 211, 238, 255);
        }
        rectangle = { move.XCoord * m_handler->SCREEN_WIDTH / 8,
                      move.YCoord * m_handler->SCREEN_HEIGHT / 8,
                      m_handler->SCREEN_WIDTH / 8,
                      m_handler->SCREEN_HEIGHT / 8 };
        SDL_RenderFillRect(m_handler->m_renderer, &rectangle);

        // TODO FIX the rendering

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (m_board[CoordToIndex(i, j)] != nullptr)
                {
                    m_board[CoordToIndex(i, j)]->render();
                }
            }
        }
    } 
}

void Game::undoRenderPossibleMoves(const std::vector<PossibleMove>& possible)
{
    for (const auto& move : possible) {
        if ((move.XCoord % 2 == 0 && move.YCoord % 2 == 1) || (move.XCoord % 2 == 1 && move.YCoord % 2 == 0))
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 155, 103, 60, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 255, 255, 255, 255);
        }
        SDL_Rect rectangle = { move.XCoord * m_handler->SCREEN_WIDTH / 8,
                                  move.YCoord * m_handler->SCREEN_HEIGHT / 8,
                                  m_handler->SCREEN_WIDTH / 8,
                                  m_handler->SCREEN_HEIGHT / 8 };
        SDL_RenderFillRect(m_handler->m_renderer, &rectangle);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (m_board[CoordToIndex(i, j)] != nullptr)
                {
                    m_board[CoordToIndex(i, j)]->render();
                }
            }
        }
    }
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
