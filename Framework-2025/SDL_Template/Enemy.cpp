#include "Enemy.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"
#include <cstdlib>
#include <ctime>

std::vector<float> Enemy::mLeftLanes;
std::vector<float> Enemy::mRightLanes;

Enemy::Enemy(bool moveDownward, int vehicleIndex) {
	mTimer = Timer::Instance();

	// Set movement direction based on spawn position
	mMoveDirection = moveDownward ? Vector2(0.0f, 1.0f) : Vector2(0.0f, -1.0f);

	// Seed random number generator
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Randomly select one of the enemy car sprites
	int carIndex = std::rand() % 17 + 1;
	std::string fileName = "EnemyCar" + std::to_string(carIndex) + ".png";
	mTexture = new Texture(fileName);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mDeathAnimation = new AnimatedTexture(fileName, 0, 0, 128, 128, 4, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

	/*mTexture = new Texture("Deer.png", 61, 0, 60, 64);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mDeathAnimation = new AnimatedTexture("Deer.png", 0, 0, 128, 128, 4, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);*/

	mAudio = AudioManager::Instance();

	mAnimating = false;

	SetColliderForVehicle(vehicleIndex);
	SetScaleForVehicle(vehicleIndex);
	SetSpeedForVehicle(vehicleIndex);

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
	float laneWidth = 70.0f;
	float leftStartX = 200.0f;
	float rightStartX = 500.0f;
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
	Vector2 moveAmount = mMoveDirection * mMoveSpeed * (1000.0f / 3600.0f) * mTimer->DeltaTime(); // Convert km/h to m/s
	Translate(moveAmount, World);

	// Check if enemy is off the screen
	if (Position().y > Graphics::SCREEN_HEIGHT || Position().y < -mTexture->ScaledDimensions().y) {
		// Reset enemy position
		Position(Vector2(Position().x, mMoveDirection.y > 0 ? -mTexture->ScaledDimensions().y : Graphics::SCREEN_HEIGHT + mTexture->ScaledDimensions().y));
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

void Enemy::SetColliderForVehicle(int vehicleIndex) {
	// Clear existing colliders
	for (auto collider : mColliders) {
		delete collider;
	}
	mColliders.clear();

	// Set collider size and position based on what vehicle spawns
	Vector2 colliderSize;
	Vector2 colliderOffset;

	switch (vehicleIndex) {
		case 1:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 2:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 3:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 4:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 5:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 6:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 7:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 8:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 9:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 10:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 11:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 12:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 13:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 14:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 15:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 16:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		case 17:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;
		default:
			colliderSize = Vector2(48.0f, 63.0f);
			colliderOffset = Vector2(-12.0f, 20.0f);
			break;

	}

	AddCollider(new BoxCollider(colliderSize), colliderOffset);
}

void Enemy::SetScaleForVehicle(int vehicleIndex) {
	// Set scale based on what vehicle spawns
	Vector2 scale;

	switch (vehicleIndex) {
		case 1:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 2:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 3:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 4:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 5:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 6:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 7:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 8:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 9:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 10:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 11:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 12:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 13:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 14:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 15:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 16:
			scale = Vector2(1.0f, 1.0f);
			break;
		case 17:
			scale = Vector2(1.0f, 1.0f);
			break;
		default:
			scale = Vector2(1.0f, 1.0f);
			break;
	}

	mTexture->Scale(scale);
}


void Enemy::SetSpeedForVehicle(int vehicleIndex) {
	// Set the speed based on what vehicle spawns

	switch (vehicleIndex) {
		case 1:
			mMoveSpeed = 1200.0f;
			break;
		case 2:
			mMoveSpeed = 1000.0f;
			break;
		case 3:
			mMoveSpeed = 1000.0f;
			break;
		case 4:
			mMoveSpeed = 1000.0f;
			break;
		case 5:
			mMoveSpeed = 1200.0f;
			break;
		case 6:
			mMoveSpeed = 1200.0f;
			break;
		case 7:
			mMoveSpeed = 800.0f; // slower velocity rate for transport trucks
			break;
		case 8:
			mMoveSpeed = 800.0f;
			break;
		case 9:
			mMoveSpeed = 950.0f;
			break;
		case 10:
			mMoveSpeed = 1200.0f;
			break;
		case 11:
			mMoveSpeed = 950.0f;
			break;
		case 12:
			mMoveSpeed = 1200.0f;
			break;
		case 13:
			mMoveSpeed = 800.0f;
			break;
		case 14:
			mMoveSpeed = 800.0f;
			break;
		case 15:
			mMoveSpeed = 1200.0f;
			break;
		case 16:
			mMoveSpeed = 1200.0f;
			break;
		case 17:
			mMoveSpeed = 1100.0f;
			break;
		default:
			mMoveSpeed = 1200.0f;
			break;
	}
}
