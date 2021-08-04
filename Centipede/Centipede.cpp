#include "Centipede.h"
#include <iostream>

int Centipede::CENTIPEDE_WIDTH = 8;
int Centipede::CENTIPEDE_HEIGHT = 8;

Centipede::Centipede(int stage, bool middleSpawn, int spawnDelay, bool linked, bool spawn) {
	//	have all of the texture classes linked to the appropriate animated textures (head, body, left, right, when the centipede is moving down, up)
	//	HandleSpawnState();
	if (spawn) {
		mHead = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 0, 18, 16, 8, 8, 0.5, AnimatedTexture::Horizontal);
		mHead->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);
		mHead->Scale(Graphics::windowScale);

		mHeadDiving = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 32, 27, 16, 8, 4, 0.5, AnimatedTexture::Horizontal);
		mHeadDiving->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);
		mHeadDiving->Scale(Graphics::windowScale);

		mBody = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 0, 36, 16, 8, 8, 0.5, AnimatedTexture::Horizontal);
		mBody->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);
		mBody->Scale(Graphics::windowScale);

		mBodyDiving = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 32, 45, 16, 8, 4, 0.5, AnimatedTexture::Horizontal);
		mBodyDiving->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);
		mBodyDiving->Scale(Graphics::windowScale);

		mDeathAnimation = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 68, 63, 16, 8, 6, 0.25, AnimatedTexture::Horizontal);
		mDeathAnimation->SetWrapMode(AnimatedTexture::Once);
		mDeathAnimation->Scale(Graphics::windowScale);
		mDeathAnimation->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);

		mCentipede = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 0, 18, 16, 8);
		mCentipede->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);
		mCentipede->Scale(Graphics::windowScale);

		mHorizontalMoveBounds = (0, Graphics::SCREEN_WIDTH * Graphics::windowScale);
		mVerticalMoveBounds = (-CENTIPEDE_HEIGHT * Graphics::windowScale, Graphics::SCREEN_HEIGHT * Graphics::windowScale);

		if (linked) {
			mMovementSpeed = 3.0f;
		}
		else {
			mMovementSpeed = 4.0f;
		}

		isLinked = linked;
		isActive = true;

		delay = spawnDelay;
		destroyCounter = 0;

		playerHit = false;

		if (middleSpawn) {
			mMiddleSpawn = true;
		}
		else {
			mMiddleSpawn = false;
		}

		int spawnCounter = 0;

		mCurrentState = spawned;
	}
	else {
		mCentipede = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 0, 18, 16, 8);
		mCentipede->Position(-CENTIPEDE_WIDTH * 3 * Graphics::windowScale, -CENTIPEDE_HEIGHT * 3 * Graphics::windowScale);
		mCentipede->Scale(Graphics::windowScale);

		isActive = false;
	}
}

void Centipede::Update(int grid[30][30], Player* player, Centipede* nextPart, Centipede* lastPart) {
	//	have every single body/head check the space in front of them if there is either a mushroom or another centipede body/head in front of it
	//	if another centipede body/head is in front of them, render the part as a body
	//	if a mushroom is in front of the part, move down 1 tile and switch directions on the x axis
	//	if a mushroom is poisoned when the part reaches it, make isPoisoned = true
	//	if isPoisioned = true make the centipede dive to the bottom of the map
	//	once the centipede reaches the bottom switch directions on the y and go back to normal on the x until it reaches level 6
	//	once the centipede reaches level 6 go back to normal behavior
	//	CheckCollision
	if (player->WasHit() && !playerHit && isActive) {
		mDeathAnimation->ResetAnimation();
		mDeathAnimation->Position(mCentipede->Position());
		playerHit = true;
		mCurrentState = destroy;
	}
	
	if (isLinked && nextPart->Active() 
		&& nextPart->GetPosition().x >= mCentipede->Position().x - (CENTIPEDE_WIDTH * 2) * Graphics::windowScale
		&& nextPart->GetPosition().x <= mCentipede->Position().x + (CENTIPEDE_WIDTH * 2) * Graphics::windowScale
		&& nextPart->GetPosition().y >= mCentipede->Position().y - (CENTIPEDE_HEIGHT * 2) * Graphics::windowScale
		&& nextPart->GetPosition().y <= mCentipede->Position().y + (CENTIPEDE_HEIGHT * 2) * Graphics::windowScale) {
		isHead = false;
	}
	else {
		isHead = true;
	}

	if (lastPart->Active() == false && nextPart->Active() == false) {
		isLinked = false;
	}

	if (isLinked) {
		mMovementSpeed = 3.0f;
	}
	else {
		mMovementSpeed = 4.0f;
	}

	CheckCollision(player, grid);

	if (mCurrentState == spawned) {
		if (spawnCounter >= delay * 10) {
			if (mMiddleSpawn) {
				mCentipede->Position((Graphics::SCREEN_WIDTH * Graphics::windowScale) / 2 - (CENTIPEDE_WIDTH * Graphics::windowScale) / 2, 0 + (CENTIPEDE_HEIGHT / 2) * Graphics::windowScale);
				
				if (isHead) {
					mHead->Position(mCentipede->Position().x, mCentipede->Position().y);
				}
				else {
					mBody->Position(mCentipede->Position().x, mCentipede->Position().y);
				}

				mCurrentMoveSpeed = -mMovementSpeed;
			}
			else {
				int randomSpawnColumn = rand() % 29;
				if (randomSpawnColumn == 15) {
					randomSpawnColumn = rand() % 29;
				}
				else {
					mCentipede->Position((randomSpawnColumn * 8) * Graphics::windowScale + CENTIPEDE_WIDTH / 2 * Graphics::windowScale, 0 + (CENTIPEDE_HEIGHT / 2) * Graphics::windowScale);

					if (isHead) {
						mHead->Position(mCentipede->Position().x, mCentipede->Position().y);
					}
					else {
						mBody->Position(mCentipede->Position().x, mCentipede->Position().y);
					}
				}

				if (rand() % 2 == 1) {
					mCurrentMoveSpeed = mMovementSpeed;
				}
				else {
					mCurrentMoveSpeed = -mMovementSpeed;
				}
			}

			mCurrentState = wander;
		}
		else {
			spawnCounter++;
		}
	}
	else if (mCurrentState == wander) {
		mCentipede->Translate(Vector2(mCurrentMoveSpeed, 0), World);
		
		if (mCentipede->Position().x - (CENTIPEDE_WIDTH / 2) * Graphics::windowScale <= mHorizontalMoveBounds.y) {
			mCentipede->Position(mHorizontalMoveBounds.y + (CENTIPEDE_WIDTH / 2) * Graphics::windowScale, mCentipede->Position().y + CENTIPEDE_HEIGHT * Graphics::windowScale);
			mCurrentMoveSpeed = mMovementSpeed;
		}
		else if (mCentipede->Position().x + (CENTIPEDE_WIDTH / 2) * Graphics::windowScale >= mHorizontalMoveBounds.x) {
			mCentipede->Position(mHorizontalMoveBounds.x - (CENTIPEDE_WIDTH / 2) * Graphics::windowScale, mCentipede->Position().y + CENTIPEDE_HEIGHT * Graphics::windowScale);
			mCurrentMoveSpeed = -mMovementSpeed;
		}

		int centipedeX = mCentipede->Position().x / 32;
		int centipedeY = mCentipede->Position().y / 32;

		if (grid[centipedeY][centipedeX] != 0 && grid[centipedeY][centipedeX] <= 4) {
			mCentipede->Position(mCentipede->Position().x, mCentipede->Position().y + CENTIPEDE_HEIGHT * Graphics::windowScale);
			mCurrentMoveSpeed = -mCurrentMoveSpeed;
		}
		else if (grid[centipedeY][centipedeX] != 0 && grid[centipedeY][centipedeX] > 4 && grid[centipedeY][centipedeX] <= 8) {
			floorHit = false;
			mCurrentState = poisoned;
		}

		if (isHead) {
			mHead->Position(mCentipede->Position().x, mCentipede->Position().y);
			mHead->Update();
		}
		else {
			mBody->Position(mCentipede->Position().x, mCentipede->Position().y);
			mBody->Update();
		}
	}
	else if (mCurrentState == poisoned) {
		if (floorHit != true) {
			mCurrentMoveSpeed = mMovementSpeed;
			mCentipede->Translate(Vector2(0, mCurrentMoveSpeed), World);
			
			if (isHead) {
				mHeadDiving->Position(mCentipede->Position().x, mCentipede->Position().y);
				mHeadDiving->Update();
			}
			else {
				mBodyDiving->Position(mCentipede->Position().x, mCentipede->Position().y);
				mBodyDiving->Update();
			}

			if (mCentipede->Position().y + CENTIPEDE_HEIGHT * Graphics::windowScale >= mVerticalMoveBounds.x - CENTIPEDE_HEIGHT * Graphics::windowScale) {
				mCentipede->Position(mCentipede->Position().x, mVerticalMoveBounds.x - CENTIPEDE_HEIGHT * Graphics::windowScale - 12 * Graphics::windowScale);
				floorHit = true;
			}
		}
		else {
			mCentipede->Translate(Vector2(mCurrentMoveSpeed, 0), World);

			if (mCentipede->Position().x - (CENTIPEDE_WIDTH / 2) * Graphics::windowScale <= mHorizontalMoveBounds.y) {
				mCentipede->Position(mHorizontalMoveBounds.y + (CENTIPEDE_WIDTH / 2) * Graphics::windowScale, mCentipede->Position().y - CENTIPEDE_HEIGHT * Graphics::windowScale);
				mCurrentMoveSpeed = mMovementSpeed;
			}
			else if (mCentipede->Position().x + (CENTIPEDE_WIDTH / 2) * Graphics::windowScale >= mHorizontalMoveBounds.x) {
				mCentipede->Position(mHorizontalMoveBounds.x - (CENTIPEDE_WIDTH / 2) * Graphics::windowScale, mCentipede->Position().y - CENTIPEDE_HEIGHT * Graphics::windowScale);
				mCurrentMoveSpeed = -mMovementSpeed;
			}

			int centipedeX = mCentipede->Position().x / 32;
			int centipedeY = mCentipede->Position().y / 32;

			if (grid[centipedeY][centipedeX] != 0 && grid[centipedeY][centipedeX] <= 4) {
				mCentipede->Position(mCentipede->Position().x, mCentipede->Position().y - CENTIPEDE_HEIGHT * Graphics::windowScale);
				mCurrentMoveSpeed = -mCurrentMoveSpeed;
			}

			if (isHead) {
				mHead->Position(mCentipede->Position().x, mCentipede->Position().y);
				mHead->Update();
			}
			else {
				mBody->Position(mCentipede->Position().x, mCentipede->Position().y);
				mBody->Update();
			}
			
			if (centipedeY <= 24) {
				mCurrentState = wander;
			}
		}
	}
	else if (mCurrentState == destroy) {
		if (destroyCounter >= delay * 7) {
			mCurrentState = dead;
		}
		else {
			destroyCounter++;
			
			if (isHead) {
				mHead->Update();
			}
			else {
				mBody->Update();
			}
		}
	}
	else if (mCurrentState == dead) {
		mDeathAnimation->Update();

		if (!mDeathAnimation->IsAnimating()) {
			mDeathAnimation->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);
		}
	}
	else {
		std::cout << "Invalid Centipede State" << std::endl;
	}
}

void Centipede::CheckCollision(Player* player, int grid[30][30]) {
	//	for each part of the centipede on screen
	//	check if bullet is colliding with them/in the same space as them
	//	if bullet collides destroy that part and spawn a mushroom in that grid location

	//	for each part check if player position = part position
	//	if collision then pause all part movement and desrtoy all parts

	if (player->GetBullet()->Active() 
		&& player->GetBullet()->Position().x >= mCentipede->Position().x - (CENTIPEDE_WIDTH / 2) * Graphics::windowScale
		&& player->GetBullet()->Position().x <= mCentipede->Position().x + (CENTIPEDE_WIDTH / 2) * Graphics::windowScale
		&& player->GetBullet()->Position().y <= mCentipede->Position().y + (CENTIPEDE_HEIGHT / 2) * Graphics::windowScale
		&& player->GetBullet()->Position().y >= mCentipede->Position().y - (CENTIPEDE_HEIGHT / 2) * Graphics::windowScale) {

		player->AddScore(300);
		player->GetBullet()->Reload();
		Shot(grid);
	}
	if (player->Position().x >= mCentipede->Position().x - (CENTIPEDE_WIDTH / 2) * Graphics::windowScale
		&& player->Position().x <= mCentipede->Position().x + (CENTIPEDE_WIDTH / 2) * Graphics::windowScale
		&& player->Position().y <= mCentipede->Position().y + (CENTIPEDE_HEIGHT / 2) * Graphics::windowScale
		&& player->Position().y >= mCentipede->Position().y - (CENTIPEDE_HEIGHT / 2) * Graphics::windowScale) {

		player->Hit();
	}
}

void Centipede::Render() {
	//	for each part of the centipede, render the appropriate texture
	if (mCurrentState != dead) {
		if (isHead) {
			if (mCurrentState != poisoned || floorHit) {
				if (mCurrentMoveSpeed >= 0) {
					mHead->Render(SDL_FLIP_HORIZONTAL);
				}
				else {
					mHead->Render();
				}
			}
			else {
				mHeadDiving->Render();
			}
		}
		else {
			if (mCurrentState != poisoned || floorHit) {
				if (mCurrentMoveSpeed >= 0) {
					mBody->Render(SDL_FLIP_HORIZONTAL);
				}
				else {
					mBody->Render();
				}
			}
			else {
				mBodyDiving->Render();
			}
		}
	}
	else {
		mDeathAnimation->Render();
	}
}
									
Centipede::~Centipede() {
	//	delete everything instanciated

	delete mHead;
	mHead = nullptr;

	delete mHeadDiving;
	mHeadDiving = nullptr;

	delete mBody;
	mBody = nullptr;

	delete mBodyDiving;
	mBodyDiving = nullptr;

	delete mCentipede;
	mCentipede = nullptr;
}

Vector2 Centipede::GetPosition() {
	return mCentipede->Position();
}

bool Centipede::Active() {
	return isActive;
}

void Centipede::Shot(int grid[30][30]) {
	int centipedeX = mCentipede->Position().x / 32;
	int centipedeY = mCentipede->Position().y / 32;

	if (centipedeX <= 30 && centipedeY >= 0 && centipedeY <= 29) {
		grid[centipedeY][centipedeX] = 4;
	}

	isActive = false;

	mDeathAnimation->Position(mCentipede->Position());

	mHead->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);
	mBody->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);
	mCentipede->Position(-CENTIPEDE_WIDTH * Graphics::windowScale, -CENTIPEDE_HEIGHT * Graphics::windowScale);

	mCurrentState = dead;
}