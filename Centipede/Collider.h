#pragma once

#include "Texture.h"

using namespace SDLFramework;

class Collider : public GameEntity {
public:    
	enum class ColliderType { Box, Circle }; // type safety

protected:    
	ColliderType mType;

	static const bool DEBUG_COLLIDERS = true;    
	Texture* mDebugTexture;    
	void SetDebugTexture(Texture* tex);

public:    
	Collider(ColliderType type);    
	virtual ~Collider(); 

	virtual Vector2 GetFurthestPoint() = 0;    
	ColliderType GetType();    
	virtual void Render(SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE) override;
};