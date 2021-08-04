#include "Level.h"
#include <random>
#include <string>
#include <iostream>
#include <stdlib.h>

Level::Level(int stage) {
	
	mScoreLabel = new Scoreboard();
	mScoreLabel->Parent(this);
	mScoreLabel->Position(6 * 8 * Graphics::windowScale, Graphics::SCREEN_HEIGHT * Graphics::windowScale - 10 * Graphics::windowScale);
	mScoreLabel->SetColor({ 255, 0, 0 });
	
	nextStageCounter = 0;
	resetStageCounter = 0;

	mShips = new GameEntity();
	mShips->Parent(this);
	mShips->Position(7 * 8 * Graphics::windowScale + 2 * Graphics::windowScale, Graphics::SCREEN_HEIGHT * Graphics::windowScale - 10 * Graphics::windowScale);

	for (int i = 0; i < MAX_PLAYER_DISPLAYED; ++i) {
		playerLifeDisplay[i] = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 21, 9, 7, 8);
		playerLifeDisplay[i]->Parent(mShips);
		playerLifeDisplay[i]->Position(8 * i * Graphics::windowScale, 0);
	}

	srand(time(NULL));

	mStage = stage;

	mPlayer = new Player(stage);
	SDL_ShowCursor(SDL_DISABLE);

	playerScore = 0;

	mAudio = AudioManager::Instance();

	audioRand = rand() % 2;

	if (audioRand == 0) {
		mAudio->PlayMusic("MUSIC/Centipede.mp3", 0);
	}
	else if (audioRand == 1) {
		mAudio->PlayMusic("MUSIC/Cent1pede.mp3", 0);
	}

	cen1  = new Centipede(stage, true, 1 , true);
	cen2  = new Centipede(stage, true, 2 , true);
	cen3  = new Centipede(stage, true, 3 , true);
	cen4  = new Centipede(stage, true, 4 , true);
	cen5  = new Centipede(stage, true, 5 , true);
	cen6  = new Centipede(stage, true, 6 , true);
	cen7  = new Centipede(stage, true, 7 , true);
	cen8  = new Centipede(stage, true, 8 , true);
	cen9  = new Centipede(stage, true, 9 , true);
	cen10 = new Centipede(stage, true, 10, true);
	cen11 = new Centipede(stage, true, 11, true);
	cen12 = new Centipede(stage, true, 12, true);
	cen13 = new Centipede(stage, true, 12, true, false);

	mSpider = new Spider(stage);
	mFlea = new Flea(stage);
	mScorpion = new Scorpion(stage);
	
	mMushroom1 = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 95, 72, 8, 8);
	mMushroom1->Scale(Graphics::windowScale);

	mMushroom2 = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 86, 72, 8, 8);
	mMushroom2->Scale(Graphics::windowScale);

	mMushroom3 = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 77, 72, 8, 8);
	mMushroom3->Scale(Graphics::windowScale);

	mMushroom4 = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 68, 72, 8, 8);
	mMushroom4->Scale(Graphics::windowScale);

	// Poison Mushrooms
	mPoisonMushroom1 = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 95, 81, 8, 8);
	mPoisonMushroom1->Scale(Graphics::windowScale);

	mPoisonMushroom2 = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 86, 81, 8, 8);
	mPoisonMushroom2->Scale(Graphics::windowScale);

	mPoisonMushroom3 = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 77, 81, 8, 8);
	mPoisonMushroom3->Scale(Graphics::windowScale);

	mPoisonMushroom4 = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 68, 81, 8, 8);
	mPoisonMushroom4->Scale(Graphics::windowScale);

	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			level[i][j] = 0;
		}
	}

	for (int i = 0; i <= 40; i++) {
		int y = rand() % 29;
		int x = rand() % 31;

		if (level[y][x] == 0) {
			level[y][x] = 4;
		}
		else {
			i--;
		}
	}
}

Level::~Level() {
	delete mScoreLabel;
	mScoreLabel = nullptr;
	
	delete cen1;
	cen1 = nullptr;

	delete cen2;
	cen2 = nullptr;

	delete cen3;
	cen3 = nullptr;

	delete cen4;
	cen4 = nullptr;

	delete cen5;
	cen5 = nullptr;

	delete cen6;
	cen6 = nullptr;

	delete cen7;
	cen7 = nullptr;

	delete cen8;
	cen8 = nullptr;

	delete cen9;
	cen9 = nullptr;

	delete cen10;
	cen10 = nullptr;
	
	delete cen11;
	cen11 = nullptr;

	delete cen12;
	cen12 = nullptr;
	
	delete mSpider;
	mSpider = nullptr;

	delete mScorpion;
	mScorpion = nullptr;

	delete mFlea;
	mFlea = nullptr;

	delete mPlayer;
	mPlayer = nullptr;

	delete mMushroom1;
	mMushroom1 = nullptr;

	delete mMushroom2;
	mMushroom2 = nullptr;

	delete mMushroom3;
	mMushroom3 = nullptr;

	delete mMushroom4;
	mMushroom4 = nullptr;

	delete mPoisonMushroom1;
	mPoisonMushroom1 = nullptr;

	delete mPoisonMushroom2;
	mPoisonMushroom2 = nullptr;

	delete mPoisonMushroom3;
	mPoisonMushroom3 = nullptr;

	delete mPoisonMushroom4;
	mPoisonMushroom4 = nullptr;

	delete mShips;
	mShips = nullptr;

	for (auto t : playerLifeDisplay) {
		delete t;
	}
}

void Level::Update() {
	if (mPlayer->Lives() > 0) {
		mPlayer->Update(level);

		cen1->Update(level, mPlayer, cen13, cen2);
		cen2->Update(level, mPlayer, cen1, cen3);
		cen3->Update(level, mPlayer, cen2, cen4);
		cen4->Update(level, mPlayer, cen3, cen5);
		cen5->Update(level, mPlayer, cen4, cen6);
		cen6->Update(level, mPlayer, cen5, cen7);
		cen7->Update(level, mPlayer, cen6, cen8);
		cen8->Update(level, mPlayer, cen7, cen9);
		cen9->Update(level, mPlayer, cen8, cen10);
		cen10->Update(level, mPlayer, cen9, cen11);
		cen11->Update(level, mPlayer, cen10, cen12);
		cen12->Update(level, mPlayer, cen11, cen13);

		if (mPlayer->WasHit()) {
			resetStageCounter++;

			if (mSpider->Active()) {
				mSpider->LevelClear();
			}

			if (mScorpion->Active()) {
				mScorpion->LevelClear();
			}

			if (mFlea->Active()) {
				mFlea->LevelClear();
			}

			if (resetStageCounter >= RESET_STAGE_MAX) {
				ResetStage();
			}
		}
		else {
			mSpider->Update(mStage, mPlayer->Score(), level);
			mScorpion->Update(mStage, mPlayer->Score(), level, mFlea->Active());
			mFlea->Update(mStage, mPlayer->Score(), level, mScorpion->Active());

			int bulletX = mPlayer->GetBullet()->Position().x / 32;
			int bulletY = mPlayer->GetBullet()->Position().y / 32;

			if (mPlayer->GetBullet()->Active() && bulletX >= 0 && bulletX <= 30 && bulletY >= 0 && bulletY <= 30) {
				if (level[bulletY][bulletX] != 0 && level[bulletY][bulletX] != 5) {
					level[bulletY][bulletX]--;
					mPlayer->GetBullet()->Reload();
				}
				else if (level[bulletY][bulletX] == 5) {
					level[bulletY][bulletX] = 0;
					mPlayer->GetBullet()->Reload();
				}

				if (mSpider->Active()
					&& mPlayer->GetBullet()->Position().x >= mSpider->pos().x - (Spider::SPIDER_WIDTH / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().x <= mSpider->pos().x + (Spider::SPIDER_WIDTH / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().y <= mSpider->pos().y + (Spider::SPIDER_HEIGHT / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().y >= mSpider->pos().y - (Spider::SPIDER_HEIGHT / 2) * Graphics::windowScale) {

					int spiderScore = 300;

					if (mPlayer->FiringPosition().y - mSpider->pos().y <= (Spider::SPIDER_HEIGHT * 3) * Graphics::windowScale) {
						spiderScore = 900;
					}
					else if (mPlayer->FiringPosition().y - mSpider->pos().y <= (Spider::SPIDER_HEIGHT * 6) * Graphics::windowScale) {
						spiderScore = 600;
					}
					else {
						spiderScore = 300;
					}

					mSpider->Shot(spiderScore);
					mPlayer->AddScore(spiderScore);
					mPlayer->GetBullet()->Reload();
				}

				if (mScorpion->Active()
					&& mPlayer->GetBullet()->Position().x >= mScorpion->pos().x - (Scorpion::SCORPION_WIDTH / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().x <= mScorpion->pos().x + (Scorpion::SCORPION_WIDTH / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().y <= mScorpion->pos().y + (Scorpion::SCORPION_HEIGHT / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().y >= mScorpion->pos().y - (Scorpion::SCORPION_HEIGHT / 2) * Graphics::windowScale) {

					mScorpion->Shot();
					mPlayer->AddScore(SCORPION_SCORE);
					mPlayer->GetBullet()->Reload();
				}

				if (mFlea->Active()
					&& mPlayer->GetBullet()->Position().x >= mFlea->pos().x - (Flea::FLEA_WIDTH / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().x <= mFlea->pos().x + (Flea::FLEA_WIDTH / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().y <= mFlea->pos().y + (Flea::FLEA_HEIGHT / 2) * Graphics::windowScale
					&& mPlayer->GetBullet()->Position().y >= mFlea->pos().y - (Flea::FLEA_HEIGHT / 2) * Graphics::windowScale) {

					mFlea->Shot();
					mPlayer->AddScore(FLEA_SCORE);
					mPlayer->GetBullet()->Reload();
				}
			}

			if (mSpider->Active()
				&& mPlayer->Position().x + (Player::SHIP_WIDTH / 2) * Graphics::windowScale >= mSpider->pos().x - (Spider::SPIDER_WIDTH / 2) * Graphics::windowScale
				&& mPlayer->Position().x - (Player::SHIP_WIDTH / 2) * Graphics::windowScale <= mSpider->pos().x + (Spider::SPIDER_WIDTH / 2) * Graphics::windowScale
				&& mPlayer->Position().y + (Player::SHIP_HEIGHT / 2) * Graphics::windowScale >= mSpider->pos().y - (Spider::SPIDER_HEIGHT / 2) * Graphics::windowScale
				&& mPlayer->Position().y - (Player::SHIP_HEIGHT / 2) * Graphics::windowScale <= mSpider->pos().y + (Spider::SPIDER_HEIGHT / 2) * Graphics::windowScale) {

				mSpider->PlayerCollide();
				mPlayer->Hit();
			}

			if (mFlea->Active()
				&& mPlayer->Position().x + (Player::SHIP_WIDTH / 2) * Graphics::windowScale >= mFlea->pos().x - (Spider::SPIDER_WIDTH / 2) * Graphics::windowScale
				&& mPlayer->Position().x - (Player::SHIP_WIDTH / 2) * Graphics::windowScale <= mFlea->pos().x + (Spider::SPIDER_WIDTH / 2) * Graphics::windowScale
				&& mPlayer->Position().y + (Player::SHIP_HEIGHT / 2) * Graphics::windowScale >= mFlea->pos().y - (Spider::SPIDER_HEIGHT / 2) * Graphics::windowScale
				&& mPlayer->Position().y - (Player::SHIP_HEIGHT / 2) * Graphics::windowScale <= mFlea->pos().y + (Spider::SPIDER_HEIGHT / 2) * Graphics::windowScale) {

				mFlea->PlayerCollide();
				mPlayer->Hit();
			}

			if (!cen1->Active() && !cen2->Active() && !cen3->Active() && !cen4->Active() && !cen5->Active() && !cen6->Active() && !cen7->Active() && !cen8->Active() && !cen9->Active() && !cen10->Active() && !cen11->Active() && !cen12->Active()) {
				nextStageCounter++;

				if (mSpider->Active()) {
					mSpider->LevelClear();
				}

				if (mScorpion->Active()) {
					mScorpion->LevelClear();
				}

				if (mFlea->Active()) {
					mFlea->LevelClear();
				}

				if (nextStageCounter >= NEXT_STAGE_MAX) {
					NextStage();
				}
			}
		}
	}
	else {
		
	}

	mScoreLabel->Score(mPlayer->Score());
}

void Level::Render() {
	mSpider->Render();
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 30; x++) {
			
			if (level[y][x] != 0) {
				switch (level[y][x]) {
				case 1:
					mMushroom1->Position(((x * MUSHROOM_WIDTH) + MUSHROOM_WIDTH / 2) * Graphics::windowScale, ((y * MUSHROOM_HEIGHT) + MUSHROOM_HEIGHT / 2) * Graphics::windowScale);
					mMushroom1->Render();
					break;
				case 2:
					mMushroom2->Position(((x * MUSHROOM_WIDTH) + MUSHROOM_WIDTH / 2) * Graphics::windowScale, ((y * MUSHROOM_HEIGHT) + MUSHROOM_HEIGHT / 2) * Graphics::windowScale);
					mMushroom2->Render();
					break;
				case 3:
					mMushroom3->Position(((x * MUSHROOM_WIDTH) + MUSHROOM_WIDTH / 2) * Graphics::windowScale, ((y * MUSHROOM_HEIGHT) + MUSHROOM_HEIGHT / 2) * Graphics::windowScale);
					mMushroom3->Render();
					break;
				case 4:
					mMushroom4->Position(((x * MUSHROOM_WIDTH) + MUSHROOM_WIDTH / 2) * Graphics::windowScale , ((y * MUSHROOM_HEIGHT) + MUSHROOM_HEIGHT / 2)  * Graphics::windowScale);
					mMushroom4->Render();
					break;
				case 5:
					mPoisonMushroom1->Position(((x * MUSHROOM_WIDTH) + MUSHROOM_WIDTH / 2) * Graphics::windowScale, ((y * MUSHROOM_HEIGHT) + MUSHROOM_HEIGHT / 2) * Graphics::windowScale);
					mPoisonMushroom1->Render();
					break;
				case 6:
					mPoisonMushroom2->Position(((x * MUSHROOM_WIDTH) + MUSHROOM_WIDTH / 2) * Graphics::windowScale, ((y * MUSHROOM_HEIGHT) + MUSHROOM_HEIGHT / 2) * Graphics::windowScale);
					mPoisonMushroom2->Render();
					break;
				case 7:
					mPoisonMushroom3->Position(((x * MUSHROOM_WIDTH) + MUSHROOM_WIDTH / 2) * Graphics::windowScale, ((y * MUSHROOM_HEIGHT) + MUSHROOM_HEIGHT / 2) * Graphics::windowScale);
					mPoisonMushroom3->Render();
					break;
				case 8:
					mPoisonMushroom4->Position(((x * MUSHROOM_WIDTH) + MUSHROOM_WIDTH / 2) * Graphics::windowScale, ((y * MUSHROOM_HEIGHT) + MUSHROOM_HEIGHT / 2) * Graphics::windowScale);
					mPoisonMushroom4->Render();
					break;
				default:
					break;
				}
			}
		}
	}

	mScorpion->Render();
	mFlea->Render();

	if (mPlayerHit) {
		if (mGameOverTimer >= mGameOverLabelOnScreen) {
			mGameOverLabel->Render();
		}
	}

	cen1->Render();
	cen2->Render();
	cen3->Render();
	cen4->Render();
	cen5->Render();
	cen6->Render();
	cen7->Render();
	cen8->Render();
	cen9->Render();
	cen10->Render();
	cen11->Render();
	cen12->Render();

	mPlayer->Render();

	mScoreLabel->Render();

	for (int i = 0; i < MAX_PLAYER_DISPLAYED && i < mPlayer->Lives(); ++i) {
		playerLifeDisplay[i]->Scale(Graphics::windowScale);
		playerLifeDisplay[i]->Render();
	}
}

void Level::NextStage() {
	mStage += 1;

	nextStageCounter = 0;

	int playerScore = mPlayer->Score();
	int playerLives = mPlayer->Lives();

	delete cen1;
	delete cen2;
	delete cen3;
	delete cen4;
	delete cen5;
	delete cen6;
	delete cen7;
	delete cen8;
	delete cen9;
	delete cen10;
	delete cen11;
	delete cen12;

	delete mSpider;
	delete mScorpion;
	delete mFlea;
	delete mPlayer;
	delete mMushroom1;
	delete mMushroom2;
	delete mMushroom3;
	delete mMushroom4;
	delete mPoisonMushroom1;
	delete mPoisonMushroom2;
	delete mPoisonMushroom3;
	delete mPoisonMushroom4;

	delete mShips;
	mShips = nullptr;

	for (auto t : playerLifeDisplay) {
		delete t;
	}

	int stageReduced;	// stageReduced is for if the stage level is over 14, if its over 14 it will be reduced to any number from 1 to 14
	if (mStage % 14 == 0) {
		stageReduced = 14;
	}
	else {
		stageReduced = mStage % 14;
	}

	if (mStage >= 13) {
		cen1 = new Centipede(stageReduced, true, 1, false);
	}
	else {
		cen1 = new Centipede(stageReduced, true, 1, true);
	}
	if (mStage >= 12) {
		cen2 = new Centipede(stageReduced, false, 2, false);
	}
	else {
		cen2 = new Centipede(stageReduced, true, 2, true);
	}
	if (mStage >= 11) {
		cen3 = new Centipede(stageReduced, false, 3, false);
	}
	else {
		cen3 = new Centipede(stageReduced, true, 3, true);
	}
	if (mStage >= 10) {
		cen4 = new Centipede(stageReduced, false, 4, false);
	}
	else {
		cen4 = new Centipede(stageReduced, true, 4, true);
	}
	if (mStage >= 9) {
		cen5 = new Centipede(stageReduced, false, 5, false);
	}
	else {
		cen5 = new Centipede(stageReduced, true, 5, true);
	}
	if (mStage >= 8) {
		cen6 = new Centipede(stageReduced, false, 6, false);
	}
	else {
		cen6 = new Centipede(stageReduced, true, 6, true);
	}
	if (mStage >= 7) {
		cen7 = new Centipede(stageReduced, false, 7, false);
	}
	else {
		cen7 = new Centipede(stageReduced, true, 7, true);
	}
	if (mStage >= 6) {
		cen8 = new Centipede(stageReduced, false, 8, false);
	}
	else {
		cen8 = new Centipede(stageReduced, true, 8, true);
	}
	if (mStage >= 5) {
		cen9 = new Centipede(stageReduced, false, 9, false);
	}
	else {
		cen9 = new Centipede(stageReduced, true, 9, true);
	}
	if (mStage >= 4) {
		cen10 = new Centipede(stageReduced, false, 10, false);
	}
	else {
		cen10 = new Centipede(stageReduced, true, 10, true);
	}
	if (mStage >= 3) {
		cen11 = new Centipede(stageReduced, false, 11, false);
	}
	else {
		cen11 = new Centipede(stageReduced, true, 11, true);
	}
	if (mStage >= 2) {
		cen12 = new Centipede(stageReduced, false, 12, false);
	}
	else {
		cen12 = new Centipede(stageReduced, true, 12, true);
	}

	switch (stageReduced) {
	case 1: 
		mScoreLabel->SetColor({ 255, 0, 0 });
		break;
	case 2:
		mScoreLabel->SetColor({ 0, 255, 192 });
		break;
	case 3:
		mScoreLabel->SetColor({ 255, 255, 0 });
		break;
	case 4:
		mScoreLabel->SetColor({ 0, 0, 255 });
		break;
	case 5:
		mScoreLabel->SetColor({ 255, 0, 255 });
		break;
	case 6:
		mScoreLabel->SetColor({ 255, 192, 0 });
		break;
	case 7:
		mScoreLabel->SetColor({ 255, 0, 255 });
		break;
	case 8:
		mScoreLabel->SetColor({ 255, 0, 0 });
		break;
	case 9:
		mScoreLabel->SetColor({ 0, 255, 192 });
		break;
	case 10:
		mScoreLabel->SetColor({ 0, 0, 255 });
		break;
	case 11:
		mScoreLabel->SetColor({ 255, 0, 0 });
		break;
	case 12:
		mScoreLabel->SetColor({ 255, 0, 255 });
		break;
	case 13:
		mScoreLabel->SetColor({ 0, 0, 255 });
		break;
	case 14:
		mScoreLabel->SetColor({ 255, 0, 255 });
		break;
	default:
		mScoreLabel->SetColor({ 230, 230, 230 });
		break;
	}

	mPlayer = new Player(stageReduced);
	SDL_ShowCursor(SDL_DISABLE);
	mPlayer->AddScore(playerScore);

	mSpider = new Spider(stageReduced);
	mFlea = new Flea(stageReduced);
	mScorpion = new Scorpion(stageReduced);

	// Regular Mushrooms
	mMushroom1 = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 95, 72, 8, 8);
	mMushroom1->Scale(Graphics::windowScale);

	mMushroom2 = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 86, 72, 8, 8);
	mMushroom2->Scale(Graphics::windowScale);

	mMushroom3 = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 77, 72, 8, 8);
	mMushroom3->Scale(Graphics::windowScale);

	mMushroom4 = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 68, 72, 8, 8);
	mMushroom4->Scale(Graphics::windowScale);

	// Poison Mushrooms
	mPoisonMushroom1 = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 95, 81, 8, 8);
	mPoisonMushroom1->Scale(Graphics::windowScale);

	mPoisonMushroom2 = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 86, 81, 8, 8);
	mPoisonMushroom2->Scale(Graphics::windowScale);

	mPoisonMushroom3 = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 77, 81, 8, 8);
	mPoisonMushroom3->Scale(Graphics::windowScale);

	mPoisonMushroom4 = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 68, 81, 8, 8);
	mPoisonMushroom4->Scale(Graphics::windowScale);

	mShips = new GameEntity();
	mShips->Parent(this);
	mShips->Position(7 * 8 * Graphics::windowScale + 2 * Graphics::windowScale, Graphics::SCREEN_HEIGHT* Graphics::windowScale - 10 * Graphics::windowScale);

	for (int i = 0; i < MAX_PLAYER_DISPLAYED; ++i) {
		playerLifeDisplay[i] = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 21, 9, 7, 8);
		playerLifeDisplay[i]->Parent(mShips);
		playerLifeDisplay[i]->Position(8 * i * Graphics::windowScale, 0);
	}
}

void Level::ResetStage() {
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 30; x++) {
			if (level[y][x] != 0) {
				level[y][x] = 4;
			}
		}
	}

	int playerScore = mPlayer->Score();
	int playerLives = mPlayer->Lives();

	delete cen1;
	delete cen2;
	delete cen3;
	delete cen4;
	delete cen5;
	delete cen6;
	delete cen7;
	delete cen8;
	delete cen9;
	delete cen10;
	delete cen11;
	delete cen12;

	delete mSpider;
	delete mScorpion;
	delete mFlea;
	delete mPlayer;

	delete mShips;
	mShips = nullptr;

	for (auto t : playerLifeDisplay) {
		delete t;
	}

	int stageReduced;	// stageReduced is for if the stage level is over 14, if its over 14 it will be reduced to any number from 1 to 14
	if (mStage % 14 == 0) {
		stageReduced = 14;
	}
	else {
		stageReduced = mStage % 14;
	}

	if (mStage >= 13) {
		cen1 = new Centipede(stageReduced, true, 1, false);
	}
	else {
		cen1 = new Centipede(stageReduced, true, 1, true);
	}
	if (mStage >= 12) {
		cen2 = new Centipede(stageReduced, false, 2, false);
	}
	else {
		cen2 = new Centipede(stageReduced, true, 2, true);
	}
	if (mStage >= 11) {
		cen3 = new Centipede(stageReduced, false, 3, false);
	}
	else {
		cen3 = new Centipede(stageReduced, true, 3, true);
	}
	if (mStage >= 10) {
		cen4 = new Centipede(stageReduced, false, 4, false);
	}
	else {
		cen4 = new Centipede(stageReduced, true, 4, true);
	}
	if (mStage >= 9) {
		cen5 = new Centipede(stageReduced, false, 5, false);
	}
	else {
		cen5 = new Centipede(stageReduced, true, 5, true);
	}
	if (mStage >= 8) {
		cen6 = new Centipede(stageReduced, false, 6, false);
	}
	else {
		cen6 = new Centipede(stageReduced, true, 6, true);
	}
	if (mStage >= 7) {
		cen7 = new Centipede(stageReduced, false, 7, false);
	}
	else {
		cen7 = new Centipede(stageReduced, true, 7, true);
	}
	if (mStage >= 6) {
		cen8 = new Centipede(stageReduced, false, 8, false);
	}
	else {
		cen8 = new Centipede(stageReduced, true, 8, true);
	}
	if (mStage >= 5) {
		cen9 = new Centipede(stageReduced, false, 9, false);
	}
	else {
		cen9 = new Centipede(stageReduced, true, 9, true);
	}
	if (mStage >= 4) {
		cen10 = new Centipede(stageReduced, false, 10, false);
	}
	else {
		cen10 = new Centipede(stageReduced, true, 10, true);
	}
	if (mStage >= 3) {
		cen11 = new Centipede(stageReduced, false, 11, false);
	}
	else {
		cen11 = new Centipede(stageReduced, true, 11, true);
	}
	if (mStage >= 2) {
		cen12 = new Centipede(stageReduced, false, 12, false);
	}
	else {
		cen12 = new Centipede(stageReduced, true, 12, true);
	}

	mSpider = new Spider(stageReduced);
	mFlea = new Flea(stageReduced);
	mScorpion = new Scorpion(stageReduced);

	mPlayer = new Player(stageReduced);
	SDL_ShowCursor(SDL_DISABLE);
	mPlayer->AddScore(playerScore);
	mPlayer->SetLives(playerLives);

	mShips = new GameEntity();
	mShips->Parent(this);
	mShips->Position(7 * 8 * Graphics::windowScale + 2 * Graphics::windowScale, Graphics::SCREEN_HEIGHT * Graphics::windowScale - 10 * Graphics::windowScale);

	for (int i = 0; i < MAX_PLAYER_DISPLAYED; ++i) {
		playerLifeDisplay[i] = new Texture("SHEETS\\" + std::to_string(stageReduced) + ".png", 21, 9, 7, 8);
		playerLifeDisplay[i]->Parent(mShips);
		playerLifeDisplay[i]->Position(8 * i * Graphics::windowScale, 0);
	}

	resetStageCounter = 0;
}