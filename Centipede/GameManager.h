#pragma once

#include <SDL.h>
#include "Texture.h"
#include "Timer.h"
#include "GameEntity.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Random.h"
#include "Level.h"

namespace SDLFramework {
	class GameManager {
	private:
		// singleton    
		static GameManager* sInstance;

		// loop control    
		bool mQuit;

		// modules    
		Graphics* mGraphics;
		SDL_Event mEvent;

		const int FRAME_RATE = 60;
		Timer* mTimer;

		AssetManager* mAssetManager;
		InputManager* mInputManager;
		AudioManager* mAudioManager;

		Texture* mTex;
		Texture* mFontTex;

		Level* mLevel;

	public:
		static GameManager* Instance();
		static void Release();

		void Run();
		void Update();
		void LateUpdate();
		void Render();
		GameManager();
		~GameManager();
	};
}