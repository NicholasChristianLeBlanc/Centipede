#include "Player.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"
#include <iostream>
#include <string>

int Player::SHIP_WIDTH = 7;
int Player::SHIP_HEIGHT = 8;

void Player::HandleMovement(int grid[30][30]) {

	Vector2 pos = mInput->MousePosition();

	if (pos.x < mHorizontalMoveBounds.x) {
		pos.x = mHorizontalMoveBounds.x;
	}
	else if (pos.x > mHorizontalMoveBounds.y) {
		pos.x = mHorizontalMoveBounds.y;
	}

	if (pos.y < mVerticalMoveBounds.x) {
		pos.y = mVerticalMoveBounds.x;
	}
	else if (pos.y > mVerticalMoveBounds.y) {
		pos.y = mVerticalMoveBounds.y;

	}
	Position(pos);
}

void Player::HandleFiring() {
	if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
		for (int i = 0; i < MAX_BULLETS; ++i) {
			if (!mBullets[i]->Active()) {
				mBullets[i]->Fire(Position());
				mAudio->PlaySFX("SFX/Laser.mp3", 0, 2);

				mFiringPosition = Position();
				break;
			}
		}
	}
	if (mInput->MouseButtonPressed(InputManager::Left)) {
		for (int i = 0; i < MAX_BULLETS; ++i) {
			if (!mBullets[i]->Active()) {
				mBullets[i]->Fire(Position());
				mAudio->PlaySFX("SFX/Laser.mp3", 0, 2);

				mFiringPosition = Position();
				break;
			}
		}
	}
}

Vector2 Player::FiringPosition() {
	return mFiringPosition;
}

Player::Player(int stage) {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();

	mVisible = true;
	mAnimating = false;
	//mWasHit = false;

	mScore = 0;
	mLives = 3;

	mShip = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 21, 9, 7, 8);
	mShip->Parent(this);
	mShip->Position(Vec2_Zero);

	mHorizontalMoveBounds = Vector2(2.0f + (SHIP_WIDTH/2) * Graphics::windowScale, 959.0f - (SHIP_WIDTH/2) * 4);
	mVerticalMoveBounds = Vector2(1024.0f - (56 + (SHIP_HEIGHT/2)) * Graphics::windowScale, 1006.0f - (SHIP_HEIGHT/2) * 4);

	mDeathAnimation = new AnimatedTexture("SHEETS\\" + std::to_string(stage) + ".png", 34, 0, 16, 8, 7, 1, AnimatedTexture::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(AnimatedTexture::Once);

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i] = new Bullet(stage);
	}

	mFiringPosition = {
		0,
		0 
	};
}

Player::~Player() {
	mTimer = nullptr;
	mInput = nullptr;
	mAudio = nullptr;

	delete mShip;
	mShip = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;

	for (auto b : mBullets) {
		delete b;
	}
}

void Player::Visible(bool visible) {
	mVisible = visible;
}

bool Player::IsAnimating() {
	return mAnimating;
}

int Player::Score() {
	return mScore;
}

int Player::Lives() {
	return mLives;
}

void Player::AddScore(int change) {
	mScore += change;
}

void Player::SetLives(int lives) {
	mLives = lives;
}

void Player::Hit() {
	mLives -= 1;
	mAnimating = true;
	hit = true;
	mDeathAnimation->ResetAnimation();
	//mAudio->PlaySFX("SFX/PlayerExplosion.wav");
}

bool Player::WasHit() {
	return hit;
}

void Player::Update(int grid[30][30]) {

	if (mAnimating) {
		mDeathAnimation->Update();
		mAnimating = mDeathAnimation->IsAnimating();
	}
	else {
		if (Active()) {
			HandleMovement(grid);
			HandleFiring();
		}
	}

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Update();
	}
}

void Player::Render() {
	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Render();
	}
	
	if (mVisible) {
		if (mAnimating) {
			mDeathAnimation->Scale(Graphics::windowScale);
			mDeathAnimation->Render();
		}
		else {
			mShip->Scale(Graphics::windowScale);
			mShip->Render();
		}
	}
}

Bullet* Player::GetBullet() {
	return mBullets[0];
}