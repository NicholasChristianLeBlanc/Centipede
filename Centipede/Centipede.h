#pragma once

#include "AnimatedTexture.h"
#include "BezierPath.h"
#include "Enemy.h"
#include "InputManager.h"
#include "Player.h"

class Centipede : public Enemy {
protected:
	enum centipedeStates {
		spawned,
		wander,
		poisoned,
		destroy,
		dead
	};

	centipedeStates mCurrentState;

	AnimatedTexture* mHead;
	AnimatedTexture* mHeadDiving;
	AnimatedTexture* mBody;
	AnimatedTexture* mBodyDiving;
	AnimatedTexture* mDeathAnimation;
	Texture* mCentipede;

	int delay;
	int spawnCounter;
	int destroyCounter;

	bool isHead;
	bool isActive;
	bool isLinked;

	bool mMiddleSpawn;
	bool floorHit;
	bool playerHit;

	Timer* mTimer;

	float mMovementSpeed;
	float mCurrentMoveSpeed;

	Vector2 mHorizontalMoveBounds;
	Vector2 mVerticalMoveBounds;

public:
	Centipede(int stage, bool middleSpawn = true, int spawnDelay = 1, bool linked = false, bool spawn = true);
	virtual ~Centipede();

	void Spawn(int delay);

	void Update(int grid[30][30], Player* player, Centipede* nextPart, Centipede* lastPart);
	void Render();

	void CheckCollision(Player* player, int grid[30][30]);
	void Shot(int grid[30][30]);

	Vector2 GetPosition();

	bool Active();

public:
	static int CENTIPEDE_WIDTH;
	static int CENTIPEDE_HEIGHT;
};