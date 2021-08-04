#include "Mushroom.h"

Mushroom::Mushroom(int layer) : Texture(, 0, 0, 4, 4) {
	mRand = Random::Instance();
}

Mushroom::~Mushroom() {
	mRand = nullptr;
}

void Mushroom::Render() {
	Texture::Render();
}