#pragma once

#include "Enemy.h"

class Spider : public Enemy {

private:
	enum spiderStates {
		spawned,
		wander,
		poisoned,
		dead,
		offScreen
	};

	spiderStates mCurrentState;

	AnimatedTexture* mSpider;

	AnimatedTexture* mDeathAnimation;

	Texture* farScore;
	Texture* midScore;
	Texture* closeScore;

	int spawnHeight;

	int spawnTimer;
	int spawnTimeMax;
	
	int wanderTimer;
	int wanderTimeMax;

	int scoreTimer;
	int scoreTimerMax;

	int mSpeedX;
	int mSpeedY;

	Vector2 mMovement;
	Vector2 mVerticalMoveBounds;
	Vector2 mHorizontalMoveBounds;

	bool leftSpawn;
	bool centipedeCollision;
	bool active;

public:
	Spider(int stage);
	~Spider();

	void SpawnSpider(int score);
	void Shot(int shotScore);
	void PlayerCollide();
	void LevelClear();

	Vector2 pos();
	bool Active();

	void Update(int stage, int score, int grid[30][30]);
	void Render();
	
public:
	static int SPIDER_WIDTH;
	static int SPIDER_HEIGHT;
};