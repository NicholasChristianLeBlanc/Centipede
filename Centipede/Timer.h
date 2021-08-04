#pragma once

#include <SDL.h>

namespace SDLFramework {
	class Timer {
	private:
		static Timer* sInstance;
		unsigned int mStartTicks;
		unsigned int mElapsedTicks;
		float mDeltaTime;
		float mTimeScale;

	public:
		static Timer* Instance();
		static void Release();
		void Reset();
		float DeltaTime();
		void TimeScale(float ts);
		float TimeScale();
		void Update();

	private:
		Timer();
		~Timer();
	};
}