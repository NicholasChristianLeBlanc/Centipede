#pragma once

#include "Enemy.h"
#include "Texture.h"
#include "Scoreboard.h"
#include "Centipede.h"
#include "Player.h"
#include "Spider.h"
#include "Scorpion.h"
#include "Flea.h"
#include "Random.h"

#include "InputManager.h"

class Level : public GameEntity {

public:
	enum LevelStates {
		Attract,
		Running,
		GameOver
	};

private:
	// Row, Column
	int level[30][30];

	Scoreboard* mScoreLabel;

	int mStage;

	int playerScore;

	AudioManager* mAudio;

	int audioRand;

	Random* mRand;

	Timer* mTimer;

	Player* mPlayer;

	Bullet* mBullet;

	Centipede* cen1;
	Centipede* cen2;
	Centipede* cen3;
	Centipede* cen4;
	Centipede* cen5;
	Centipede* cen6;
	Centipede* cen7;
	Centipede* cen8;
	Centipede* cen9;
	Centipede* cen10;
	Centipede* cen11;
	Centipede* cen12;
	Centipede* cen13;

	Spider* mSpider;
	Flea* mFlea;
	Scorpion* mScorpion;
	
	const int SCORPION_SCORE = 900;
	const int FLEA_SCORE = 300;

	static const int MAX_PLAYER_DISPLAYED = 3;
	GameEntity* mShips;
	Texture* playerLifeDisplay[MAX_PLAYER_DISPLAYED];

	Texture* mMushroom1;
	Texture* mMushroom2;
	Texture* mMushroom3;
	Texture* mMushroom4;

	Texture* mPoisonMushroom1;
	Texture* mPoisonMushroom2;
	Texture* mPoisonMushroom3;
	Texture* mPoisonMushroom4;

	const int MUSHROOM_WIDTH = 8;
	const int MUSHROOM_HEIGHT = 8;

	bool mPlayerHit;
	float mRespawnDelay;
	float mRespawnTimer;
	float mRespawnLabelOnScreen;

	Texture* mGameOverLabel;
	float mGameOverDelay;
	float mGameOverTimer;
	float mGameOverLabelOnScreen;

	LevelStates mCurrentState;

	int nextStageCounter;
	const int NEXT_STAGE_MAX = 2 * 60;

	int resetStageCounter;
	const int RESET_STAGE_MAX = 2 * 60;

public:
	Level(int stage);
	~Level();

	void Update();
	void Render();

private:
	//void HandlePlayerDeath();

	void NextStage();
	void ResetStage();
};