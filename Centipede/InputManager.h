#pragma once

#include <SDL.h>
#include "MathHelper.h"
#include <string>

namespace SDLFramework { 
	class InputManager { 
	public:    
		enum MouseButton { Left = 0, Right, Middle, Back, Forward };

	private:        
		static InputManager* sInstance;
		const Uint8* mKeyboardState; 

		Uint8* mPrevKeyboardState; 

		int mKeyLength;

		Uint32 mPrevMouseState;    
		Uint32 mMouseState; 

		int mMouseXPos;    
		int mMouseYPos;

	public:        
		static InputManager* Instance();        
		static void Release(); 

		//Keyboard
		bool KeyDown(SDL_Scancode scancode);    
		bool KeyPressed(SDL_Scancode scancode);    
		bool KeyReleased(SDL_Scancode scancode);

		//Mouse
		bool MouseButtonDown(MouseButton button);    
		bool MouseButtonPressed(MouseButton button);    
		bool MouseButtonReleased(MouseButton button);    
		Vector2 MousePosition();

		void Update();    
		void UpdatePrevInput();

	private:    
		InputManager();    
		~InputManager();
	}; 
}