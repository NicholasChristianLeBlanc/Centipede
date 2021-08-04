#include "PhysicsEntity.h"
#include "PhysicsHelper.h"
#include "PhysicsManager.h"

void PhysicsEntity::AddCollider(Collider * collider, Vector2 localPos) {    
	collider->Parent(this);    
	collider->Position(localPos);    
	mColliders.push_back(collider);    
	
	if (mColliders.size() > 1 || mColliders[0]->GetType() != Collider::ColliderType::Circle) {        
		float furthestDistance = mColliders[0]->GetFurthestPoint().Magnitude();        
		float dist = 0.0f;        
		
		for (int i = 1; i < mColliders.size(); i++) {            
			dist = mColliders[i]->GetFurthestPoint().Magnitude();            
			
			if (dist > furthestDistance) {                
				furthestDistance = dist;            
			}        
		}        
		// create the collider for mBroadPhaseCollider!    
	};
}

bool PhysicsEntity::IgnoreCollisions() { 
	return false; 
}

bool PhysicsEntity::CheckCollision(PhysicsEntity* other) {
	if (IgnoreCollisions() || other->IgnoreCollisions()) { 
		return false; 
	}    
	// need to check for collision!    
	// returning true just to avoid compiler error!    
	return true;
}

PhysicsEntity::PhysicsEntity() { 
	mBroadPhaseCollider = nullptr;    
	mId = 0; 
}

PhysicsEntity::~PhysicsEntity() { 
	for (auto c : mColliders) { 
		delete c; 
	}    
	mColliders.clear();    
	delete mBroadPhaseCollider; 
}
	
unsigned long PhysicsEntity::GetId() { 
	return mId; 
}

void PhysicsEntity::Render(SDL_RendererFlip flip) {
	for (auto c : mColliders) { 
		c->Render(flip); 
	}    
	
	if (mBroadPhaseCollider) { 
		mBroadPhaseCollider->Render(flip); 
	} 
}