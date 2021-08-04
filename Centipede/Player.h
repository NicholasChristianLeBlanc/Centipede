#pragma once

#include "AnimatedTexture.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Bullet.h"

using namespace SDLFramework;

class Player : public GameEntity {
private:
	Timer* mTimer;
	InputManager* mInput;
	AudioManager* mAudio;

	bool mVisible;
	bool mAnimating;

	bool hit;

	int mScore;
	int mLives;

	int mLastStage;

	Texture* mShip;
	AnimatedTexture* mDeathAnimation;

	Vector2 mHorizontalMoveBounds;
	Vector2 mVerticalMoveBounds;

	Vector2 mFiringPosition;

	static const int MAX_BULLETS = 1;
	Bullet* mBullets[MAX_BULLETS];

public:
	Player(int stage);
	~Player();

	void Visible(bool visible);
	bool IsAnimating();

	int Score();
	int Lives();

	void AddScore(int change);
	void SetLives(int lives);

	void Hit();
	bool WasHit();

	Bullet* GetBullet();

	Vector2 FiringPosition();

	void Update(int grid[30][30]);
	void Render();

private:
	void HandleMovement(int grid[30][30]);
	void HandleFiring();

public:
	static int SHIP_WIDTH;
	static int SHIP_HEIGHT;
};