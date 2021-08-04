#pragma once

#include "Texture.h"
#include "Random.h"

using namespace SDLFramework;

class Mushroom : public Texture {

private:

	int MushroomState;

	Random* mRand;

public:
	Mushroom(int layer);
	~Mushroom();

	void Update();
	void Render();
};