#include "Enemy.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"
#include <cstdlib>
#include <ctime>

std::vector<float> Enemy::mLeftLanes;
std::vector<float> Enemy::mRightLanes;

Enemy::Enemy() {
	mTimer = Timer::Instance();
	mTexture = new Texture("EnemyCar.png");
	mTexture->Parent(this);
	mTexture ->Position(Vec2_Zero);

	mMoveSpeed = 150.0f;

	// Seed random number generator
	std::srand(std::time(nullptr));

	mTexture = new Texture("Deer.png", 61, 0, 60, 64);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mDeathAnimation = new AnimatedTexture("Deer.png", 0, 0, 128, 128, 4, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

	mAudio = AudioManager::Instance();

	mAnimating = false;

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

void Enemy::InitializeLanes() {
	//Initialize lanes
	float laneWidth = 100.0f;
	float leftStartX = 100.0f;
	float rightStartX = 600.0f;
	for (int i = 0; i < 4; ++i) {
		mLeftLanes.push_back(leftStartX + i * laneWidth);
		mRightLanes.push_back(rightStartX + i * laneWidth);
	}
}

const std::vector<float>& Enemy::GetLeftLanes() {
	return mLeftLanes;
}

const std::vector<float>& Enemy::GetRightLanes() {
	return mRightLanes;
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
	// Move enemy vehicle
	Vector2 moveDir = Vec2_Zero;
	moveDir.y += 1; // Move down

	Vector2 moveAmount = moveDir * mMoveSpeed * (1000.0f / 3600.0f) * mTimer->DeltaTime(); 
	Translate(moveAmount, World);

	// Check if enemy is off the screen
	if (Position().y > Graphics::SCREEN_HEIGHT) {
		// Reset enemy position
		Position(Vector2(Position().x, -mTexture->ScaledDimensions().y));
	}
	
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

Vector2 Enemy::GetTextureDimensions() const {
	return mTexture->ScaledDimensions();
}
