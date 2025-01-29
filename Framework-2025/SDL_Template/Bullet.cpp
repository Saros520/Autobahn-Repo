#include "Bullet.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

bool Bullet::IgnoreCollisions() {
	return !Active();
}

Bullet::Bullet(bool friendly) {
	mTimer = Timer::Instance();

	mTexture = new GLTexture("Bullet.png");
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mSpeed = 500;

	Reload();

	AddCollider(new BoxCollider(mTexture->ScaledDimensions()));

	if (friendly) {
		mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::FriendlyProjectiles);
	}
	else {
		mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::HostileProjectiles);
	}
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

void Bullet::Hit(PhysEntity * other) {
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

void Bullet::Render() {
	if (Active()) {
		mTexture->Render();
		PhysEntity::Render();
	}
}
