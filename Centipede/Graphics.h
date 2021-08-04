#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

namespace SDLFramework {
	class Graphics {
	public:
		static const short SCREEN_WIDTH = 240;    
		static const short SCREEN_HEIGHT = 256;
		const char* WINDOW_TITLE = "Centipede";

		static int windowScale;

	private:
		static Graphics* sInstance;
		static bool sInitialized;
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;

	public:
		static Graphics* Instance();
		static void Release();
		static bool Initialized();

		void ClearBackBuffer();
		void Render();

		SDL_Texture* LoadTexture(std::string path);
		SDL_Texture* CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color);

		void DrawTexture(SDL_Texture* tex, SDL_Rect* srcRect = nullptr, SDL_Rect* dstRect = nullptr, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void DrawLine(float startX, float startY, float endX, float endY);

	private:
		Graphics();
		~Graphics();
		bool Init();
	};
}