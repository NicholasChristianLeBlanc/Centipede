#include "Flea.h"
#include <iostream>

int Flea::FLEA_WIDTH = 9;
int Flea::FLEA_HEIGHT = 8;

Flea::Flea(int stage) {
	mFlea = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 0, 63, 16, 8, 4, 1.5, AnimatedTexture::Horizontal);
	mFlea->Position(-FLEA_WIDTH * Graphics::windowScale, -FLEA_HEIGHT * Graphics::windowScale);

	mDeathAnimation = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 68, 63, 16, 8, 6, 0.5, AnimatedTexture::Horizontal);
	mDeathAnimation->SetWrapMode(AnimatedTexture::Once);
	mDeathAnimation->Position(-FLEA_WIDTH * Graphics::windowScale, -FLEA_HEIGHT * Graphics::windowScale);

	mCurrentState = offScreen; // set to offScreen because if the Flea gets destroyed then there's a chance it will spawn a new Flea IMMEDIATELY, whereas if the Flea goes off screen there is a slight delay
	mMovement = {
		0,
		5
	};

	levelClear = false;

	mVerticalMoveBounds = Vector2(1024.0f + FLEA_HEIGHT * Graphics::windowScale, 0.0f - FLEA_HEIGHT * Graphics::windowScale);
}

Flea::~Flea() {
	delete mFlea;
	mFlea = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;
}

void Flea::Update(int stage, int score, int grid[30][30], bool scorpionActive) {
	if (stage >= 2) { // change to 2
		if (mCurrentState == diving) {
			mFlea->Translate(mMovement, World);
			mFlea->Update();

			int fleaX = mFlea->Position().x / 32;
			int fleaY = (mFlea->Position().y - 11 * Graphics::windowScale) / 32;

			if (fleaX >= 0 && fleaX <= 30 && fleaY >= 0 && fleaY <= 27) {
				if (rand() % 8 == 0) {
					grid[fleaY][fleaX] = 4;
				}
			}

			if (mFlea->Position().y > mVerticalMoveBounds.x) {
				spawnTimerMax = ((rand() % 3) + 2) * 60;
				spawnTimer = 0;
				active = false;
				mCurrentState = offScreen;
			}
		}
		else if (mCurrentState == dead) {
			
			mDeathAnimation->Update();

			if (!mDeathAnimation->IsAnimating()) {
				mDeathAnimation->Position(-FLEA_WIDTH * Graphics::windowScale, -FLEA_HEIGHT * Graphics::windowScale);
			}
			
			if (!scorpionActive && !levelClear) {
				if ((rand() % (7 * 60)) + 1 == 1) {
					SpawnFlea();
				}
			}
		}
		else if (mCurrentState == offScreen) {
			mFlea->Position(-FLEA_WIDTH * Graphics::windowScale, -FLEA_HEIGHT * Graphics::windowScale);
			if (spawnTimer < spawnTimerMax) {
				spawnTimer++;
			}
			else {
				if (!scorpionActive) {
					if ((rand() % (7 * 60)) + 1 == 1) {
						SpawnFlea();
					}
				}
			}
		}
		else {
			std::cout << "Invalid Flea State" << std::endl;
		}
	}
}

void Flea::Render() {
	if (mCurrentState != dead && mCurrentState != offScreen) {
		mFlea->Scale(Graphics::windowScale);
		mFlea->Render();
	}
	else {
		mDeathAnimation->Scale(Graphics::windowScale);
		mDeathAnimation->Render();
	}
}

bool Flea::Active() {
	return active;
}

void Flea::SpawnFlea() {
	active = true;
	mFlea->Position((rand() % 29) * 8 * Graphics::windowScale + FLEA_WIDTH/2 * Graphics::windowScale, -FLEA_HEIGHT * Graphics::windowScale);
	mCurrentState = diving;
}

void Flea::Shot() {
	spawnTimer = 0;
	mDeathAnimation->ResetAnimation();
	mDeathAnimation->Position(mFlea->Position());
	mFlea->Position(-FLEA_WIDTH * Graphics::windowScale, -FLEA_HEIGHT * Graphics::windowScale);
	active = false;
	mCurrentState = dead;
}

void Flea::LevelClear() {
	spawnTimer = 0;
	mDeathAnimation->ResetAnimation();
	mDeathAnimation->Position(mFlea->Position());
	mFlea->Position(-FLEA_WIDTH * Graphics::windowScale, -FLEA_HEIGHT * Graphics::windowScale);
	levelClear = true;
	active = false;
	mCurrentState = dead;
}

Vector2 Flea::pos() {
	return mFlea->Position();
}

void Flea::PlayerCollide() {
	spawnTimer = 0;
	active = false;
	mCurrentState = offScreen;
}