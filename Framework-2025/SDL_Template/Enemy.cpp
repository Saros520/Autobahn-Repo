#include "Enemy.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"

Enemy::Enemy() {
	mAudio = AudioManager::Instance();

	mAnimating = false;

	mTexture = new GLTexture("Exit.png", 61, 0, 60, 64);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	mDeathAnimation = new AnimatedGLTexture("Exit.png", 0, 0, 128, 128, 4, 1.0f, Animation::Layouts::Horizontal);
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
<<<<<<< Updated upstream
	mAnimating = true;
	mDeathAnimation->ResetAnimation();
	mAudio->PlaySFX("SFX/PlayerExplosion.wav");
=======
    // Check if the other entity is an enemy
    Enemy* otherEnemy = dynamic_cast<Enemy*>(other);
    if (otherEnemy) {
        // Destroy both the current enemy and the other enemy
        Destroy();
        otherEnemy->Destroy();
    }
    else {
        // Handle other types of collisions if necessary
        mAnimating = true;
        mDeathAnimation->ResetAnimation();
        mAudio->PlaySFX("SFX/PlayerExplosion.wav");
    }
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
Vector2 Enemy::GetTextureDimensions() const {
    return mTexture->ScaledDimensions();
}

void Enemy::SetMoveDirection(const Vector2& direction) {
    mMoveDirection = direction;
}

void Enemy::StartTurning(float targetRotation, const Vector2& targetDirection, float turnSpeedFactor) {
    mTargetRotation = targetRotation;
    mTargetDirection = targetDirection;
    mTurnSpeedFactor = turnSpeedFactor;
    mTurning = true;
}

void Enemy::SetSpeedMultiplier(float multiplier) {
    mSpeedMultiplier = multiplier;
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

    AddCollider(new BoxCollider(mTexture->ScaledDimensions()));
}

void Enemy::SetScaleForVehicle(int vehicleIndex) {
    // Set scale based on what vehicle spawns
    Vector2 scale;

    switch (vehicleIndex) {
    case 1:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 2:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 3:
        scale = Vector2(3.0f, 3.0f);
        break;
    case 4:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 5:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 6:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 7:
        scale = Vector2(4.0f, 3.8f);
        break;
    case 8:
        scale = Vector2(4.0, 3.8f);
        break;
    case 9:
        scale = Vector2(3.0f, 3.0f);
        break;
    case 10:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 11:
        scale = Vector2(3.0f, 3.0f);
        break;
    case 12:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 13:
        scale = Vector2(4.0f, 3.8f);
        break;
    case 14:
        scale = Vector2(4.0f, 3.8f);
        break;
    case 15:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 16:
        scale = Vector2(2.0f, 2.0f);
        break;
    case 17:
        scale = Vector2(3.0f, 3.0f);
        break;
    default:
        scale = Vector2(2.0f, 2.0f);
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

void Enemy::Destroy() {
    // Properly delete the enemy
    PhysicsManager::Instance()->UnregisterEntity(mId);
    Active(false);
}
>>>>>>> Stashed changes
