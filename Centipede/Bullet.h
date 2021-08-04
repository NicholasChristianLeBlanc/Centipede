#pragma once

#include "PhysicsEntity.h"
#include "Timer.h"

using namespace SDLFramework;

class Bullet : public PhysicsEntity {
private:
	static const int OFFSCREEN_BUFFER = 10;

	Timer* mTimer;

	Texture* mTexture;

	float mSpeed;

private:
	//Inherited from PhysEntity
	bool IgnoreCollisions() override;

public:
	Bullet(int stage);
	~Bullet();

	void Fire(Vector2 pos);
	void Reload();

	// Inherited from PhysEntity
	void Hit(PhysicsEntity* other) override;

	void Update() override;
	void Render(SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE) override;
};