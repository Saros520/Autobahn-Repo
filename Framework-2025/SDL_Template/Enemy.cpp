#include "Enemy.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

Enemy::Enemy() {
	mAudio = AudioManager::Instance();

	mAnimating = false;

	mTexture = new GLTexture("PlayerShips.png", 61, 0, 60, 64);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mDeathAnimation = new AnimatedGLTexture("PlayerExplosion.png", 0, 0, 128, 128, 4, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

	AddCollider(new BoxCollider(mTexture->ScaledDimensions()));

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Hostile);
}

Enemy::~Enemy() {

	mAudio = nullptr;

	delete mTexture;
	mTexture = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;

}

bool Enemy::IsAnimating() {
	return mAnimating;
}

bool Enemy::IgnoreCollisions()
{
	return mAnimating;
}

void Enemy::Hit(PhysEntity* other) {
	mAnimating = true;
	mDeathAnimation->ResetAnimation();
	mAudio->PlaySFX("SFX/PlayerExplosion.wav");
}

void Enemy::Update() {
	if (mAnimating) {

		mDeathAnimation->Update();
		mAnimating = mDeathAnimation->IsAnimating();
	}
}

void Enemy::Render() {
	
	if (mAnimating) {
		mDeathAnimation->Render();
	}
	else {
		mTexture->Render();
	}

	PhysEntity::Render();
}

