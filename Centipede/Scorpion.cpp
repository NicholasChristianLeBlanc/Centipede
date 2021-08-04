#include "Scorpion.h"
#include <random>
#include <string>
#include <iostream>

int Scorpion::SCORPION_WIDTH = 15;
int Scorpion::SCORPION_HEIGHT = 8;

Scorpion::Scorpion(int stage) {
	mScorpion = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 0, 72, 16, 8, 4, 1, AnimatedTexture::Horizontal);
	mScorpion->Position(-SCORPION_WIDTH * Graphics::windowScale, -SCORPION_HEIGHT * Graphics::windowScale);

	mLeftScorpion = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 105, 72, 16, 8, 4, 1, AnimatedTexture::Horizontal);
	mScorpion->Position(-SCORPION_WIDTH * Graphics::windowScale, -SCORPION_HEIGHT * Graphics::windowScale);

	mDeathAnimation = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 34, 0, 16, 8, 7, 0.5, AnimatedTexture::Horizontal);
	mDeathAnimation->SetWrapMode(AnimatedTexture::Once);
	mDeathAnimation->Position(-SCORPION_WIDTH * Graphics::windowScale, -SCORPION_HEIGHT * Graphics::windowScale);

	mScore = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 68, 9, 16, 8);
	mScore->Position(-SCORPION_WIDTH * Graphics::windowScale, -SCORPION_HEIGHT * Graphics::windowScale);

	spawnHeight = 0;
	spawnTimer = 0;

	scoreTimer = 0;
	scoreTimerMax = 2 * 60;

	mCurrentState = dead;

	active = false;
	levelClear = false;

	srand(time(NULL));
}

Scorpion::~Scorpion() {
	delete mScorpion;
	mScorpion = nullptr;

	delete mLeftScorpion;
	mLeftScorpion = nullptr;

	delete mScore;
	mScore = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;
}

void Scorpion::Update(int stage, int score, int grid[30][30], bool fleaActive) {
	if (stage >= 3) { // change this to 3

		if (mCurrentState == wander) {
			if (leftSpawn == false) {
				mScorpion->Translate(mMovement, World);
				mScorpion->Update();

				if (mScorpion->Position().x > mHorizontalMoveBounds.y || mScorpion->Position().x < mHorizontalMoveBounds.x) {
					spawnTimer = 0;
					mCurrentState = offScreen;
				}
			}
			else if (leftSpawn == true) {
				mLeftScorpion->Translate(mMovement, World);
				mLeftScorpion->Update();

				if (mLeftScorpion->Position().x > mHorizontalMoveBounds.y || mLeftScorpion->Position().x < mHorizontalMoveBounds.x) {
					spawnTimer = 0;
					mCurrentState = offScreen;
				}
			}
		}
		else if (mCurrentState == dead) {
			// when dead it will take 4 seconds to spawn the next spider
			spawnTimeMax = 7 * 60;

			spawnTimer++;

			active = false;

			if (leftSpawn == false) {
				mDeathAnimation->Position(mScorpion->Position());
			}
			else {
				mDeathAnimation->Position(mLeftScorpion->Position());
			}
			mDeathAnimation->Update();

			if (mDeathAnimation->IsAnimating() == false) {

				mDeathAnimation->Active(false);

				if (!levelClear) {
					if (leftSpawn == false) {
						mScore->Position(mScorpion->Position());
					}
					else {
						mScore->Position(mLeftScorpion->Position());
					}
				}
			}

			if (fleaActive) {
				spawnTimer = 0;
			}
			else {
				if (spawnTimer >= spawnTimeMax) {
					SpawnScorpion();
				}
			}
		}
		else if (mCurrentState == offScreen) {
			// when the spider walks off the screen safely another spider will spawn 2 seconds after
			spawnTimeMax = 5 * 60;

			spawnTimer++;

			active = false;

			if (fleaActive) {
				spawnTimer = 0;
			}
			else {
				if (spawnTimer >= spawnTimeMax) {
					SpawnScorpion();
				}
			}
		}
		else {
			std::cout << "Invalid Scorpion State" << std::endl;
		}

		int scorpionX = mScorpion->Position().x / 32;
		int scorpionY = mScorpion->Position().y / 32;

		if (grid[scorpionY][scorpionX] != 0 && grid[scorpionY][scorpionX] < 5 && scorpionX >= 0 && scorpionX <= 30 && scorpionY >= 0 && scorpionY <= 29) {
			grid[scorpionY][scorpionX] += 4;
		}

	// spawns at any level from the very top (level 30) to level 13 and moves across the screen

	// convert Scorpion position to graph position
	// if graph[scorpionY][scorpionX] != 0
	// then check what number is in the position and change that mushroom to a poisoned mushroom

	// if the scorpion gets hit by a bullet then change its current state to dead
	}
}

void Scorpion::Render() {
	// if dead then display points for killing the scorpion for set amount of time on the last position it was at
	// else display the scorpion's walking animation
	if (mCurrentState != dead && active == true) {
		if (leftSpawn == false) {
			mScorpion->Scale(Graphics::windowScale);
			mScorpion->Render();
		}
		else {
			mLeftScorpion->Scale(Graphics::windowScale);
			mLeftScorpion->Render();
		}
	}
	else {
		if (mDeathAnimation->IsAnimating()) {
			mDeathAnimation->Scale(Graphics::windowScale);
			mDeathAnimation->Render();
		}
		else {
			if (scoreTimer < scoreTimerMax) {
				mScore->Scale(Graphics::windowScale);
				mScore->Render();

				scoreTimer++;
			}
		}
	}
}

void Scorpion::Shot() {
	spawnTimer = 0;
	scoreTimer = 0;
	mDeathAnimation->ResetAnimation();
	mCurrentState = dead;
}

void Scorpion::LevelClear() {
	levelClear = true;
	spawnTimer = 0;
	mDeathAnimation->ResetAnimation();
	mCurrentState = dead;
}

Vector2 Scorpion::pos() {
	if (leftSpawn == false) {
		return mScorpion->Position();
	}
	else {
		return mLeftScorpion->Position();
	}
}

bool Scorpion::Active() {
	return active;
}

void Scorpion::SpawnScorpion() {
	spawnHeight = rand() % 17 + 1;
	
	if ((rand() % 2) == 0) {
		mLeftScorpion->Position(-SCORPION_WIDTH * Graphics::windowScale, ((spawnHeight * 8) + SCORPION_HEIGHT / 2) * Graphics::windowScale);

		mMovement = {
			1.5,
			0
		};

		leftSpawn = true;
	}
	else {
		mScorpion->Position(Graphics::SCREEN_WIDTH * Graphics::windowScale + SCORPION_WIDTH, ((spawnHeight * 8) + SCORPION_HEIGHT / 2) * Graphics::windowScale);

		mMovement = {
			-1.5,
			0
		};

		leftSpawn = false;
	}

	mHorizontalMoveBounds = Vector2(0 - SCORPION_WIDTH * Graphics::windowScale, Graphics::SCREEN_WIDTH * Graphics::windowScale + SCORPION_WIDTH * Graphics::windowScale + 5);

	active = true;

	mScore->Position(-SCORPION_WIDTH * Graphics::windowScale, -SCORPION_HEIGHT * Graphics::windowScale);

	mCurrentState = wander;
}