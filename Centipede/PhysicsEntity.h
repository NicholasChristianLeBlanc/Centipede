#pragma once

#include <vector>
#include "Collider.h"

class PhysicsEntity : public GameEntity {
protected:    
	unsigned long mId;    
	std::vector<Collider *> mColliders;    
	Collider * mBroadPhaseCollider;

	void AddCollider(Collider* collider, Vector2 localPos = Vec2_Zero);    
	virtual bool IgnoreCollisions();

public:    
	PhysicsEntity();    
	~PhysicsEntity();    
	
	unsigned long GetId();    
	bool CheckCollision(PhysicsEntity* other);

	virtual void Hit(PhysicsEntity* other) { }    
	
	virtual void Render(SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE) override;
};