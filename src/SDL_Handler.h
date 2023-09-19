#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "Piece.h"
#include <memory>

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

struct color {
	int r, g, b;
};

class SDL_Handler
{
public:

	// Screen size
	const int SCREEN_WIDTH = 480;
	const int SCREEN_HEIGHT = 480;
	const int CELL_WIDTH = SCREEN_WIDTH / 8;
	const int CELL_HEIGHT = SCREEN_HEIGHT / 8;


	// constructor
	SDL_Handler();

	// window we'll be rendering to
	SDL_Window* m_window;

	// Renderer
	SDL_Renderer* m_renderer;

	// Event (Mouseclick etc)
	SDL_Event m_event;
	
	// cleans up SDL stuff
	void cleanUp();

	// destructor
	~SDL_Handler();

	// renders the source rectangle of the texture to dest rectangle
	void DrawRectangle(SDL_Rect source, SDL_Rect dest, SDL_Texture* text);

	// removes PieceRender
	void undoPieceRender(int x, int y);

	// gets filename, return texture of the jpg file
	SDL_Texture* loadImage(std::string filename);

	//renders the background of the board
	void prepareBackgroundRender();

	void loadPiecesTextures();

	void prepareToRenderPieces(const std::vector<std::unique_ptr<Piece>>& pieces);

	void renderPossibleMoves(const std::vector<std::unique_ptr<Piece>>& pieces, const std::vector<PossibleMove>& possible);

	void refreshTheWindow(const std::vector<std::unique_ptr<Piece>>& pieces);

	PieceType chooseNewPiece(Team turn);

	TextureMap textures;

private:
	const color DARK{ 155, 103, 60 };
	const color LIGHT{ 255, 255, 255 };
};

