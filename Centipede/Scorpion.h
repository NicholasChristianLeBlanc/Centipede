#pragma once

#include "Enemy.h"

class Scorpion : public Enemy {

private:
	enum scorpionStates {
		wander,
		dead,
		offScreen
	};

	scorpionStates mCurrentState;

	AnimatedTexture* mScorpion;
	AnimatedTexture* mLeftScorpion;
	AnimatedTexture* mDeathAnimation;
	Texture* mScore;

	int spawnTimer;
	int spawnTimeMax;
	int spawnHeight;

	int scoreTimer;
	int scoreTimerMax;

	bool leftSpawn;
	bool active;
	bool levelClear;

	Vector2 mMovement;
	Vector2 mHorizontalMoveBounds;

public:
	Scorpion(int stage);
	~Scorpion();

	void SpawnScorpion();
	void Shot();
	void LevelClear();

	Vector2 pos();
	bool Active();

	void Update(int stage, int score, int grid[30][30], bool fleaActive);
	void Render();

public:
	static int SCORPION_WIDTH;
	static int SCORPION_HEIGHT;
};