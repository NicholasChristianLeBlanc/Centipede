#pragma once

#include "Enemy.h"

class Flea : public Enemy {

private:
	enum fleaStates {
		diving,
		dead,
		offScreen
	};

	fleaStates mCurrentState;

	bool active;
	bool levelClear;

	AnimatedTexture* mFlea;
	AnimatedTexture* mDeathAnimation;

	Vector2 mMovement;
	Vector2 mVerticalMoveBounds;

	int spawnTimer;
	int spawnTimerMax;

public:
	Flea(int stage);
	~Flea();

	bool Active();
	void SpawnFlea();

	void Shot();
	void PlayerCollide();
	void LevelClear();
	Vector2 pos();

	void Update(int stage, int score, int grid[30][30], bool scorpionActive);
	void Render();

public:
	static int FLEA_WIDTH;
	static int FLEA_HEIGHT;
};