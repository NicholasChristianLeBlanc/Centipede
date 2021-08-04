#pragma once

#include "AnimatedTexture.h"
#include "BezierPath.h"

class Enemy : public GameEntity {
protected:
	static std::vector<std::vector<Vector2>> sPaths;

	Timer* mTimer;
	Texture* mTexture;

	unsigned mCurrentPath;
	unsigned mCurrentWaypoint;

	float mSpeed;

public:
	static void CreatePaths();

	Enemy();
	virtual ~Enemy();

	void Update();
	void Render();
};