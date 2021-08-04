#include "GameManager.h"
#include <iostream>
#include "AnimatedTexture.h"

namespace SDLFramework {
	GameManager* GameManager::sInstance = nullptr;

	GameManager* GameManager::Instance() {
		if (sInstance == nullptr) {
			sInstance = new GameManager();
		}
		return sInstance;
	}

	void GameManager::Release() {
		delete sInstance;
		sInstance = nullptr;
	}

	void GameManager::Run() {

		// main game loop
		while (!mQuit) {
			mTimer->Update();

			// event processing loop
			while (SDL_PollEvent(&mEvent)) {
				switch (mEvent.type) {
				case SDL_QUIT:
					mQuit = true;
					break;
				}
			}

			if (mTimer->DeltaTime() >= 1.0f / FRAME_RATE) {
				Update();
				LateUpdate();
				Render();
				mTimer->Reset();
			}
		}
	}

	void GameManager::Update() { 
		mInputManager->Update(); 
		mLevel->Update();
	}
	
	void GameManager::LateUpdate() { 
		mInputManager->UpdatePrevInput(); 
	}

	void GameManager::Render() { 
		mGraphics->ClearBackBuffer(); 
		mLevel->Render();  
		mGraphics->Render();
	}

	GameManager::GameManager() : mQuit(false) {

		mLevel = new Level(1);

		// create Graphics singleton    
		mGraphics = Graphics::Instance();

		// verify Graphics module is ready    
		if (!Graphics::Initialized()) {
			mQuit = true;
		}

		mAssetManager = AssetManager::Instance();
		mTimer = Timer::Instance();
		mInputManager = InputManager::Instance();   
		mAudioManager = AudioManager::Instance();
	}

	GameManager::~GameManager() {

		// release modules
		Timer::Release();
		mTimer = nullptr;  

		AssetManager::Release(); 
		mAssetManager = nullptr;

		    
		Graphics::Release();
		mGraphics = nullptr;

		InputManager::Release();    
		mInputManager = nullptr;

		AudioManager::Release();    
		mAudioManager = nullptr;    
		
		InputManager::Release();    
		mInputManager = nullptr;

		// Quit SDL subsystems    
		SDL_Quit();
	}
}