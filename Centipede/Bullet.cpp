#include "Bullet.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

bool Bullet::IgnoreCollisions() {
	return !Active();
}

Bullet::Bullet(int stage) {
	mTimer = Timer::Instance();

	mTexture = new Texture("SHEETS\\" + std::to_string(stage) + ".png", 23, 1, 3, 7);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mSpeed = 1800;

	Reload();
}

Bullet::~Bullet() {
	mTimer = nullptr;

	delete mTexture;
	mTexture = nullptr;
}

void Bullet::Fire(Vector2 pos) {
	Position(pos);
	Active(true);
}

void Bullet::Reload() {
	Active(false);
}

void Bullet::Hit(PhysicsEntity* other) {
	Reload();
}

void Bullet::Update() {
	if (Active()) {
		Translate(-Vec2_Up * mSpeed * mTimer->DeltaTime());

		Vector2 pos = Position();
		if (pos.y < -OFFSCREEN_BUFFER) {
			Reload();
		}
	}
}

void Bullet::Render(SDL_RendererFlip flip) {
	if (Active()) {
		mTexture->Scale(Graphics::windowScale);
		mTexture->Render(flip);
		PhysicsEntity::Render(flip);
	}
}