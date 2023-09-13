#include "Game.h"
#include <iostream>

Game::Game(SDL_Handler* handler)
       :m_turn(Team::WHITE),
        m_handler(handler),
        m_checkEnPassant(true)
{
    createPieces();
    calcAllMoves();
}

void Game::createPieces()
{
    pieces.push_back(std::make_unique<Queen>(Team::WHITE, Point(4, 7), m_handler));

    // Create pawns
    for (int x{}; x < 8; x++) {
        // white
        pieces.push_back(std::make_unique<Pawn>(Team::WHITE, Point(x, 1), m_handler));
        m_field[CoordToIndex(x,1)] = pieces.back().get();
        // black
        pieces.push_back(std::make_unique<Pawn>(Team::BLACK, Point(x, 6), m_handler));
        m_field[CoordToIndex(x,6)] = pieces.back().get();
    }

    // Create Rook
    // White
    pieces.push_back(std::make_unique<Rook>(Team::WHITE, Point(0, 0), m_handler));
    m_field[CoordToIndex(0,0)] = pieces.back().get();
    pieces.push_back(std::make_unique<Rook>(Team::WHITE, Point(7, 0), m_handler));
    m_field[CoordToIndex(7, 0)] = pieces.back().get();
    // Black
    pieces.push_back(std::make_unique<Rook>(Team::BLACK, Point(0, 7), m_handler));
    m_field[CoordToIndex(0, 7)] = pieces.back().get();
    pieces.push_back(std::make_unique<Rook>(Team::BLACK, Point(7, 7), m_handler));
    m_field[CoordToIndex(7, 7)] = pieces.back().get();

    // Create Knight
    //White
    pieces.push_back(std::make_unique<Knight>(Team::WHITE, Point(1, 0), m_handler));
    m_field[CoordToIndex(1, 0)] = pieces.back().get();
    pieces.push_back(std::make_unique<Knight>(Team::WHITE, Point(6, 0), m_handler));
    m_field[CoordToIndex(6, 0)] = pieces.back().get();
    //Black
    pieces.push_back(std::make_unique<Knight>(Team::BLACK, Point(1, 7), m_handler));
    m_field[CoordToIndex(1, 7)] = pieces.back().get();
    pieces.push_back(std::make_unique<Knight>(Team::BLACK, Point(6, 7), m_handler));
    m_field[CoordToIndex(6, 7)] = pieces.back().get();

    //Create Bishop
    //White
    pieces.push_back(std::make_unique<Bishop>(Team::WHITE, Point(2, 0), m_handler));
    m_field[CoordToIndex(2, 0)] = pieces.back().get();
    pieces.push_back(std::make_unique<Bishop>(Team::WHITE, Point(5, 0), m_handler));
    m_field[CoordToIndex(5, 0)] = pieces.back().get();
    //Black
    pieces.push_back(std::make_unique<Bishop>(Team::BLACK, Point(2, 7), m_handler));
    m_field[CoordToIndex(2, 7)] = pieces.back().get();
    pieces.push_back(std::make_unique<Bishop>(Team::BLACK, Point(5, 7), m_handler));
    m_field[CoordToIndex(5, 0)] = pieces.back().get();

    // Create King
    //Black
    pieces.push_back(std::make_unique<King>(Team::BLACK, Point(3, 7), m_handler));
    m_field[CoordToIndex(3, 7)] = pieces.back().get();
    blackKing = static_cast<King*>(pieces.back().get());

    //White
    pieces.push_back(std::make_unique<King>(Team::WHITE, Point(3, 0), m_handler));
    m_field[CoordToIndex(3, 0)] = pieces.back().get();
    whiteKing = static_cast<King*>(pieces.back().get());

    // Create Queen
    // Black
    pieces.push_back(std::make_unique<Queen>(Team::BLACK, Point(4, 7), m_handler));
    m_field[CoordToIndex(4, 7)] = pieces.back().get();
    // WHITE
    pieces.push_back(std::make_unique<Queen>(Team::WHITE, Point(4, 0), m_handler));
    m_field[CoordToIndex(4, 0)] = pieces.back().get();

    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            m_field[CoordToIndex(j, i)] = nullptr;
        }
    }
}

Game::~Game()
{
}


Piece* Game::getPieceByPosition(int row, int col)
{
    return m_field[row * 8 + col];
}


void Game::move(Piece* start, PossibleMove move)
{
    if (m_checkEnPassant)
    {
        disableEnPassant();
    }
    else
    {
        m_checkEnPassant = true;
    }

    switch (move.MoveType)
    {
        case MoveType::NORMAL:
            normal(start->getPos().x, start->getPos().y, move.XCoord, move.YCoord);
            break;
        case MoveType::CASTLE:
            castles(start->getPos().x, start->getPos().y, move.XCoord, move.YCoord);
            break;
        case MoveType::ENPASSANT:
            enPassant(start->getPos().x, start->getPos().y, move.XCoord, move.YCoord);
            break;
        case MoveType::NEWPIECE:
            exchange(start->getPos().x, start->getPos().y, move.XCoord, move.YCoord);
            break;
        default:
            break;
    }

    gameState();
}


void Game::normal(int xStart, int yStart, int xEnd, int yEnd)
{
    m_field[CoordToIndex(xEnd, yEnd)] = getPieceByPosition(xStart, yStart);
    m_field[CoordToIndex(xEnd, yEnd)]->m_hasMoved = true;
    m_field[CoordToIndex(xStart, yStart)] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_field[CoordToIndex(xEnd, yEnd)]->setPosition(Point(xEnd, yEnd));
    if (m_field[CoordToIndex(xEnd, yEnd)] != nullptr)
    {
        m_handler->undoPieceRender(xEnd, yEnd);
    }
    m_field[CoordToIndex(xEnd, yEnd)]->render();

    if (m_field[CoordToIndex(xEnd, yEnd)]->getType() == PieceType::PAWN)
    {
        if (abs(yEnd - yStart) == 2)
        {
            if (xEnd - 1 >= 0)
            {
                if (m_field[CoordToIndex(xEnd - 1, yEnd)] != nullptr)
                {
                    if (m_field[CoordToIndex(xEnd - 1, yEnd)]->getType() == PieceType::PAWN)
                    {
                        Pawn* pwn = static_cast<Pawn*>(m_field[CoordToIndex(xEnd - 1, yEnd)]);
                        pwn->setEnPassant(std::pair<bool, int>(true, -1));
                        m_checkEnPassant = false;
                    }
                }
            }

            if (xEnd + 1 <= 7)
            {
                if (m_field[CoordToIndex(xEnd + 1, yEnd)] != nullptr)
                {
                    if (m_field[CoordToIndex(xEnd + 1, yEnd)]->getType() == PieceType::PAWN)
                    {
                        Pawn* pwn = static_cast<Pawn*>(m_field[CoordToIndex(xEnd + 1, yEnd)]);
                        pwn->setEnPassant(std::pair<bool, int>(true, 1));
                        m_checkEnPassant = false;
                    }
                }
            }
        }
    }
}


void Game::enPassant(int xStart, int yStart, int xEnd, int yEnd)
{
    Pawn* pawn_start = static_cast<Pawn*>(m_field[CoordToIndex(xStart, yStart)]);
    m_field[CoordToIndex(xEnd, yEnd - pawn_start->m_dy)] = nullptr;
    m_field[CoordToIndex(xEnd, yEnd)] = getPieceByPosition(xStart, yStart);
    m_field[CoordToIndex(xEnd, yEnd)]->m_hasMoved = true;
    m_field[CoordToIndex(xStart, yStart)] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_handler->undoPieceRender(xEnd, yEnd - pawn_start->m_dy);
    m_field[CoordToIndex(xEnd, yEnd)]->setPosition(Point(xEnd, yEnd));
    m_field[CoordToIndex(xEnd, yEnd)]->render();
}


void Game::exchange(int xStart, int yStart, int xEnd, int yEnd)
{
    SDL_Texture* text_rook = m_handler->loadImage("../res/Chess_rlt60.png");
    SDL_Texture* text_knight = m_handler->loadImage("../res/Chess_nlt60.png");
    SDL_Texture* text_bishop = m_handler->loadImage("../res/Chess_blt60.png");
    SDL_Texture* text_queen = m_handler->loadImage("../res/Chess_qlt60.png");
    int y_draw = 0;
    Team team = Team::WHITE;

    if (m_field[CoordToIndex(xStart, yStart)]->getTeam() == Team::BLACK)
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

    m_field[CoordToIndex(xEnd, yEnd)] = clickedPiece;
    m_field[CoordToIndex(xStart, yStart)] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_handler->renderBackground();
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_field[CoordToIndex(i, j)] != nullptr)
            {
                m_field[CoordToIndex(i, j)]->render();
            }
        }
    }
    
    SDL_DestroyTexture(text_rook);
    SDL_DestroyTexture(text_bishop);
    SDL_DestroyTexture(text_knight);
    SDL_DestroyTexture(text_queen);
}


void Game::castles(int xStart, int yStart, int xEnd, int yEnd)
{
    if (xEnd == 0)
    {
        m_field[CoordToIndex(2, yEnd)] = m_field[CoordToIndex(4, yEnd)];
        m_field[CoordToIndex(3, yEnd)] = m_field[CoordToIndex(0, yEnd)];
        m_field[CoordToIndex(2, yEnd)]->m_hasMoved = true;
        m_field[CoordToIndex(3, yEnd)]->m_hasMoved = true;
        m_field[CoordToIndex(2, yEnd)]->setPosition(Point(2, yEnd));
        m_field[CoordToIndex(3, yEnd)]->setPosition(Point(3, yEnd));
        m_field[CoordToIndex(4, yEnd)] = nullptr;
        m_field[CoordToIndex(0, yEnd)] = nullptr;
        m_handler->undoPieceRender(4, yEnd);
        m_handler->undoPieceRender(0, yEnd);
        m_field[CoordToIndex(2, yEnd)]->render();
        m_field[CoordToIndex(3, yEnd)]->render();
    }
    else
    {
        m_field[CoordToIndex(6, yEnd)] = m_field[CoordToIndex(4, yEnd)];
        m_field[CoordToIndex(5, yEnd)] = m_field[CoordToIndex(7, yEnd)];
        m_field[CoordToIndex(6, yEnd)]->m_hasMoved = true;
        m_field[CoordToIndex(5, yEnd)]->m_hasMoved = true;
        m_field[CoordToIndex(6, yEnd)]->setPosition(Point(6, yEnd));
        m_field[CoordToIndex(5, yEnd)]->setPosition(Point(5, yEnd));
        m_field[CoordToIndex(4, yEnd)] = nullptr;
        m_field[CoordToIndex(7, yEnd)] = nullptr;
        m_handler->undoPieceRender(4, yEnd);
        m_handler->undoPieceRender(7, yEnd);
        m_field[CoordToIndex(6, yEnd)]->render();
        m_field[CoordToIndex(5, yEnd)]->render();
    }
}

void Game::gameState()
{
    bool lost = true;
    King* pivot = blackKing;

    if (m_turn == Team::BLACK)
    {
        pivot = whiteKing;
    }

    pivot->setCheck(m_field, whiteKing->getPos().x, whiteKing->getPos().y);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_field[CoordToIndex(i, j)] != nullptr)
            {
                if (m_field[CoordToIndex(i, j)]->getTeam() != m_turn)
                {
                    if (!m_field[CoordToIndex(i, j)]->getPossibleMoves(m_field, true).empty())
                    {
                        lost = false;
                    }
                }
            }
        }
    }

    if (pivot->getCheck() && lost)
    {
        if (m_turn == Team::BLACK)
        {
            std::cout << "Black wins!";
        }
        else
        {
            std::cout << "White wins!";
        }
    }
    else if (lost)
    {
        if (m_turn == Team::BLACK)
        {
            std::cout << "Remis!";
        }
        else
        {
            std::cout << "Remis!";
        }
    }
    if (m_turn == Team::BLACK)
    {
        m_turn = Team::WHITE;
    }
    else
    {
        m_turn = Team::BLACK;
    }

}


void Game::disableEnPassant()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_field[CoordToIndex(i, j)] != nullptr)
            {
                if (m_field[CoordToIndex(i, j)]->getType() == PieceType::PAWN)
                {
                    Pawn* pwn = static_cast<Pawn*>(m_field[CoordToIndex(i, j)]);
                    pwn->setEnPassant(std::pair<bool, int>(false, 0));
                }
            }
        }
    }
}


void Game::renderPossibleMoves(Piece* piece)
{
    std::vector<PossibleMove> possible = piece->getPossibleMoves(m_field, true);
    SDL_Rect rectangle;
    for (const auto& [XCoord, YCoord, MoveType] : possible) {
        if ((XCoord % 2 == 0 && YCoord % 2 == 1) || (XCoord % 2 == 1 && YCoord % 2 == 0))
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 0, 134, 139, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 164, 211, 238, 255);
        }
        rectangle = { XCoord * m_handler->SCREEN_WIDTH / 8,
                      YCoord * m_handler->SCREEN_HEIGHT / 8,
                      m_handler->SCREEN_WIDTH / 8,
                      m_handler->SCREEN_HEIGHT / 8 };
        SDL_RenderFillRect(m_handler->m_renderer, &rectangle);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (m_field[CoordToIndex(i, j)] != nullptr)
                {
                    m_field[CoordToIndex(i, j)]->render();
                }
            }
        }
    } 
}

void Game::undoRenderPossibleMoves(Piece* piece)
{
    std::vector<PossibleMove> possible = piece->getPossibleMoves(m_field);
    for (const auto& [XCoord, YCoord, MoveType] : possible) {
        if ((XCoord % 2 == 0 && YCoord % 2 == 1) || (XCoord % 2 == 1 && YCoord % 2 == 0))
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 155, 103, 60, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 255, 255, 255, 255);
        }
        SDL_Rect rectangle = { XCoord * m_handler->SCREEN_WIDTH / 8,
                                  YCoord * m_handler->SCREEN_HEIGHT / 8,
                                  m_handler->SCREEN_WIDTH / 8,
                                  m_handler->SCREEN_HEIGHT / 8 };
        SDL_RenderFillRect(m_handler->m_renderer, &rectangle);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (m_field[CoordToIndex(i, j)] != nullptr)
                {
                    m_field[CoordToIndex(i, j)]->render();
                }
            }
        }
    }
}

void Game::calcAllMoves()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_field[CoordToIndex(i, j)] != nullptr)
            {
                m_field[CoordToIndex(i, j)]->calcPossibleMoves(m_field, true);
            }
        }
    }
}

bool Game::isValidMove(int x, int y, Piece* piece)
{
    std::vector<PossibleMove> list = piece->getPossibleMoves(m_field);
    for (const auto& [XCoord, YCoord, MoveType] : list) {
        if (XCoord == x && YCoord == y)
        {
            return true;
        }
    }
    return false;
}
