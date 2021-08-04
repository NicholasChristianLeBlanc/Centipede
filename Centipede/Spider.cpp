#include "Spider.h"
#include <string>
#include <time.h>
#include <iostream>

int Spider::SPIDER_WIDTH = 15;
int Spider::SPIDER_HEIGHT = 8;

Spider::Spider(int stage) {
	mSpider = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 0, 54, 16, 8, 8, 0.5, AnimatedTexture::Horizontal);
	mSpider->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

	mDeathAnimation = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 34, 0, 16, 8, 7, 0.5, AnimatedTexture::Horizontal);
	mDeathAnimation->SetWrapMode(AnimatedTexture::Once);

	farScore = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 34, 9, 16, 8);
	farScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

	midScore = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 51, 9, 16, 8);
	midScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

	closeScore = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 68, 9, 16, 8);
	closeScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

	spawnHeight = 0;
	spawnTimer = 0;
	spawnTimeMax = 0;

	wanderTimer = 0;
	wanderTimeMax = 0;

	scoreTimer = 0;
	scoreTimerMax = 2 * 60;
	
	mSpeedX = 2;
	mSpeedY = 2;

	mCurrentState = dead;

	centipedeCollision = false;

	active = false;

	srand(time(NULL));
}

Spider::~Spider() {
	delete mSpider;
	mSpider = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;

	delete farScore;
	farScore = nullptr;

	delete midScore;
	midScore = nullptr;

	delete closeScore;
	closeScore = nullptr;
}

void Spider::Update(int stage, int score, int grid[30][30]) {
	if (stage >= 1) {
		if (mCurrentState == spawned) {
			
			mMovement = {
				(float)mSpeedX,
				(float)mSpeedY
			};
			
			if (spawnTimer < spawnTimeMax) {
				mSpider->Translate(mMovement, World);
				mSpider->Update();

				spawnTimer++;

				if (mSpider->Position().y > mVerticalMoveBounds.y) {
					mSpider->Position(mSpider->Position().x, mVerticalMoveBounds.y - 1);
					mCurrentState = wander;
				}
			}
			else {
				mCurrentState = wander;
			}
		}
		else if (mCurrentState == wander) {
			// moving at either a 45 degree angle or straight up and down

			// get a randomizer to choose if the spider is moving at either 45 degrees or vertically
			// randomize how far it will move at 45 or vertically
			// if the spider hits the top (level 12) or bottom boundary (level 0) make the spider change which direction it is travelling at vertically
			// if the spider collides with the centipede make the spider change directions horizontally

			if (mSpider->Position().y > mVerticalMoveBounds.y) {
				mSpeedY *= -1;
			}
			else if (mSpider->Position().y < mVerticalMoveBounds.x) {
				mSpeedY *= -1;
			}

			if (mSpider->Position().x > mHorizontalMoveBounds.y || mSpider->Position().x < mHorizontalMoveBounds.x) {
				spawnTimer = 0;
				mCurrentState = offScreen;
			}

			if (wanderTimer >= wanderTimeMax) {
				wanderTimeMax = (rand() % 4 + 1) * 60;
				wanderTimer = 0;

				if ((rand() % 2) == 0) {
					if (leftSpawn == true && centipedeCollision == false) {
						mSpeedX = 2;
					}
					else if (leftSpawn == true && centipedeCollision == true) {
						mSpeedX = -2;
					}
					else if (leftSpawn == false && centipedeCollision == false) {
						mSpeedX = -2;
					}
					else if (leftSpawn == false && centipedeCollision == true) {
						mSpeedX = 2;
					}
				}
				else {
					mSpeedX = 0;
				}

				if (rand() % 2 == 0) {
					mSpeedY = 2;
				}
				else {
					mSpeedY = -2;
				}
			}
			else {

				mMovement = {
					(float)mSpeedX,
					(float)mSpeedY
				};

				mSpider->Translate(mMovement, World);
				mSpider->Update();
				wanderTimer++;
			}
		}
		else if (mCurrentState == poisoned) {
			// diving straight down

			// if the spider is poisoned make it dive straight down to level 0 (very bottom of the grid) where it will then act normally again

			mSpeedX = 0;
			mSpeedY = 1;

			mMovement = {
					(float)mSpeedX,
					(float)mSpeedY
			};

			mSpider->Translate(mMovement, World);
			mSpider->Update();

			if (mSpider->Position().y > mVerticalMoveBounds.y) {
				mCurrentState = wander;
			}
			else if (mSpider->Position().y < mVerticalMoveBounds.x) {
				mCurrentState = wander;
			}
		}
		else if (mCurrentState == dead) {
			// when dead it will take 4 seconds to spawn the next spider
			spawnTimeMax = 4 * 60;

			spawnTimer++;
			
			active = false;

			mDeathAnimation->Position(mSpider->Position());
			mDeathAnimation->Update();

			if (mDeathAnimation->IsAnimating() == false) {
				scoreTimer++;

				mDeathAnimation->Active(false);
				mDeathAnimation->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

				if (scoreTimer < scoreTimerMax) {
					if (farScore->Active() == true) {
						farScore->Position(mSpider->Position());
					}
					else if (midScore->Active() == true) {
						midScore->Position(mSpider->Position());
					}
					else if (closeScore->Active() == true) {
						closeScore->Position(mSpider->Position());
					}
				}
				else {
					farScore->Active(false);
					farScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

					midScore->Active(false);
					midScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

					closeScore->Active(false);
					closeScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);
				}
			}

			if (spawnTimer >= spawnTimeMax) {
				SpawnSpider(score);
			}
		}
		else if (mCurrentState == offScreen) {
			// when the spider walks off the screen safely another spider will spawn 2 seconds after
			spawnTimeMax = 2 * 60;

			spawnTimer++;

			active = false;

			if (spawnTimer >= spawnTimeMax) {
				SpawnSpider(score);
			}
		}
		else {
			std::cout << "Invalid Spider State" << std::endl;
		}

		int spiderX = mSpider->Position().x / 32;
		int spiderY = mSpider->Position().y / 32;

		if (grid[spiderY][spiderX] != 0 && grid[spiderY][spiderX] < 5 && spiderX >= 0 && spiderX <= 30 && spiderY >= 0 && spiderY <= 29) {
			grid[spiderY][spiderX] = 0;
		}
		else if (grid[spiderY][spiderX] >= 5 && grid[spiderY][spiderX] <= 8 && spiderX >= 0 && spiderX <= 30 && spiderY >= 0 && spiderY <= 29) {
			grid[spiderY][spiderX] = 0;
			mCurrentState = poisoned;
		}
	}
}

void Spider::Render() {
	if (mCurrentState != dead && active == true) {
		mSpider->Scale(Graphics::windowScale);
		mSpider->Render();
	}
	else {
		mDeathAnimation->Scale(Graphics::windowScale);
		mDeathAnimation->Render();

		farScore->Scale(Graphics::windowScale);
		farScore->Render();

		midScore->Scale(Graphics::windowScale);
		midScore->Render();

		closeScore->Scale(Graphics::windowScale);
		closeScore->Render();
	}
}

void Spider::LevelClear() {
	spawnTimer = 0;
	mDeathAnimation->ResetAnimation();
	mCurrentState = dead;
}

void Spider::Shot(int shotScore) {
	spawnTimer = 0;
	scoreTimer = 0;
	mDeathAnimation->ResetAnimation();

	if (shotScore == 300) {
		farScore->Active(true);
	}
	else if (shotScore == 600) {
		midScore->Active(true);
	}
	else if (shotScore == 900) {
		closeScore->Active(true);
	}

	mCurrentState = dead;
}

void Spider::PlayerCollide() {
	spawnTimer = 0;
	mCurrentState = offScreen;
}

Vector2 Spider::pos() {
	return mSpider->Position();
}

bool Spider::Active() {
	return active;
}

void Spider::SpawnSpider(int score) {
	if (score < 80000) {
		// max level 12

		mVerticalMoveBounds = Vector2(1024.0f - (100 + (SPIDER_HEIGHT / 2)) * Graphics::windowScale, 1006.0f - (SPIDER_HEIGHT / 2) * 4);

		// spawning the spider
		spawnHeight = 13;
	}
	else if (score >= 80000 && score < 100000) {
		// max level 11

		mVerticalMoveBounds = Vector2(1024.0f - (92 + (SPIDER_HEIGHT / 2)) * Graphics::windowScale, 1006.0f - (SPIDER_HEIGHT / 2) * 4);

		spawnHeight = 12;
	}
	else if (score >= 100000 && score < 120000) {
		// max level 10

		mVerticalMoveBounds = Vector2(1024.0f - (84 + (SPIDER_HEIGHT / 2)) * Graphics::windowScale, 1006.0f - (SPIDER_HEIGHT / 2) * 4);

		spawnHeight = 11;
	}
	else if (score >= 120000 && score < 140000) {
		// max level 9

		mVerticalMoveBounds = Vector2(1024.0f - (76 + (SPIDER_HEIGHT / 2)) * Graphics::windowScale, 1006.0f - (SPIDER_HEIGHT / 2) * 4);

		spawnHeight = 10;
	}
	else if (score >= 140000 && score < 160000) {
		// max level 8

		mVerticalMoveBounds = Vector2(1024.0f - (68 + (SPIDER_HEIGHT / 2)) * Graphics::windowScale, 1006.0f - (SPIDER_HEIGHT / 2) * 4);

		spawnHeight = 9;
	}
	else if (score <= 160000 && score < 860000) {
		// max level 7

		mVerticalMoveBounds = Vector2(1024.0f - (60 + (SPIDER_HEIGHT / 2)) * Graphics::windowScale, 1006.0f - (SPIDER_HEIGHT / 2) * 4);

		spawnHeight = 8;
	}
	else {
		// max level 12

		mVerticalMoveBounds = Vector2(1024.0f - (100 + (SPIDER_HEIGHT / 2)) * Graphics::windowScale, 1006.0f - (SPIDER_HEIGHT / 2) * 4);

		spawnHeight = 13;
	}

	mHorizontalMoveBounds = Vector2(0 - SPIDER_WIDTH * Graphics::windowScale, Graphics::SCREEN_WIDTH * Graphics::windowScale + SPIDER_WIDTH * Graphics::windowScale + 5);
 
	if ((rand() % 2) == 0) {
		mSpider->Position(-SPIDER_WIDTH * Graphics::windowScale, ((spawnHeight * 8) + SPIDER_HEIGHT / 2) * Graphics::windowScale);

		mSpeedX = 2;
		mSpeedY = 2;

		leftSpawn = true;
	}
	else {
		mSpider->Position(Graphics::SCREEN_WIDTH * Graphics::windowScale + SPIDER_WIDTH, ((spawnHeight * 8) + SPIDER_HEIGHT / 2) * Graphics::windowScale);

		mSpeedX = -2;
		mSpeedY = 2;

		leftSpawn = false;
	}

	spawnTimeMax = ((rand() % 5) + 2) * 60;
	spawnTimer = 0;

	scoreTimer = 0;

	centipedeCollision = false;
	active = true;

	mSpider->Active(true);
	mSpider->ResetAnimation();

	mDeathAnimation->Active(false);

	farScore->Active(false);
	farScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

	midScore->Active(false);
	midScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

	closeScore->Active(false);
	closeScore->Position(-SPIDER_WIDTH * Graphics::windowScale, -SPIDER_HEIGHT * Graphics::windowScale);

	mCurrentState = spawned;
}