#include "Player.h"
#include "BoxCollider.h"
#include "PhysicsManager.h"
#include "ScreenManager.h"

Player::Player() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();

	mVisible = true;
	mAnimating = false;
	mWasHit = false;

	mScore = 0;
	mLives = 3;

	mTexture = nullptr;

	SetCarTexture(0);

	mMoveSpeed = 223.0f;
	mHorizontalMoveSpeed = 186.0f;
	mAcceleration = 40.0f;
	mDeceleration = 30.0f;
	mCurrentSpeed = 186.0f;
	mMoveBoundsX = Vector2(20.0f + mTexture->ScaledDimensions().x / 2, Graphics::SCREEN_WIDTH - mTexture->ScaledDimensions().x / 2);
	mMoveBoundsY = Vector2(110.0f, Graphics::SCREEN_HEIGHT);

	mDistanceTraveled = 0.0f;

	Position(Vector2(Graphics::SCREEN_WIDTH * 0.75f, Graphics::SCREEN_HEIGHT * 0.9f));

	mDeathAnimation = new AnimatedTexture("EnemyExplosion.png", 0, 0, 128, 128, 5, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

	/*for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i] = new Bullet(true);
	}*/

	SetColliderForCar(0);

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);
}

void Player::SetCarTexture(int carIndex) {
	std::string fileName = "PlayerCar" + std::to_string(carIndex + 1) + ".png";
	delete mTexture;
	mTexture = new Texture(fileName);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);

	SetColliderForCar(carIndex);
}

void Player::SetColliderForCar(int carIndex) {
	int* collider = 0;
	// Clear existing colliders 
	for (auto c : mColliders) {
		delete collider;
	}
	mColliders.clear();

	// Set collider size and position based on what vehicle has been selected 
	Vector2 colliderSize;
	Vector2 colliderOffset;

	switch (carIndex) {
	case 0:
		colliderSize = Vector2(48.0f, 63.0f);
		colliderOffset = Vector2(-12.0f, 20.0f);
		break;
	case 1:
		colliderSize = Vector2(40.0f, 90.0f);
		colliderOffset = Vector2(-12.0f, 20.0f);
		break;
	case 2:
		colliderSize = Vector2(48.0f, 85.0f);
		colliderOffset = Vector2(-12.0f, 10.0f);
		break;
	case 3:
		colliderSize = Vector2(48.0f, 85.0f);
		colliderOffset = Vector2(-12.0f, 10.0f);
		break;
	case 4:
		colliderSize = Vector2(48.0f, 85.0f);
		colliderOffset = Vector2(-12.0f, 10.0f);
		break;
	case 5:
		colliderSize = Vector2(48.0f, 85.0f);
		colliderOffset = Vector2(-12.0f, 10.0f);
		break;
	default:
		colliderSize = Vector2(48.0f, 85.0f);
		colliderOffset = Vector2(-12.0f, 10.0f);
		break;
	}

	AddCollider(new BoxCollider(colliderSize), colliderOffset);
}

Player::~Player() {
	mTimer = nullptr;
	mInput = nullptr;
	mAudio = nullptr;

	delete mTexture;
	mTexture = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;

	/*for (auto b : mBullets) {
		delete b;
	}*/
}

void Player::HandleMovement() {
	const float Gravity = 100.0f;
	Vector2 moveDir = Vec2_Zero;

	if (mInput->KeyDown(SDL_SCANCODE_W) || mInput->KeyDown(SDL_SCANCODE_UP)) {
		moveDir.y -= 1;
	}
	if (mInput->KeyDown(SDL_SCANCODE_S) || mInput->KeyDown(SDL_SCANCODE_DOWN)) {
		moveDir.y += 1;
	}
	if (mInput->KeyDown(SDL_SCANCODE_A) || mInput->KeyDown(SDL_SCANCODE_LEFT)) {
		moveDir.x -= 1;
	}
	if (mInput->KeyDown(SDL_SCANCODE_D) || mInput->KeyDown(SDL_SCANCODE_RIGHT)) {
		moveDir.x += 1;
	}

	if (moveDir.MagnitudeSqr() > 0.0f) {
		moveDir = moveDir.Normalized();
		if (moveDir.y > 0) { // Decelerating
			mCurrentSpeed -= mDeceleration * mTimer->DeltaTime();
			if (mCurrentSpeed < 150.0f) { 
				mCurrentSpeed = 150.0f;
			}
		}
		else if (moveDir.y < 0) { // Accelerating
			mCurrentSpeed += mAcceleration * mTimer->DeltaTime();
			if (mCurrentSpeed > mMoveSpeed) {
				mCurrentSpeed = mMoveSpeed;
			}
		}
	}
	else {
		if (mCurrentSpeed > 186.0f) { // Decelerate to 186 km/h
			mCurrentSpeed -= mDeceleration * mTimer->DeltaTime();
			if (mCurrentSpeed < 186.0f) {
				mCurrentSpeed = 186.0f;
			}
		}
		else if (mCurrentSpeed < 186.0f) { // Accelerate to 186 km/h
			mCurrentSpeed += mAcceleration * mTimer->DeltaTime();
			if (mCurrentSpeed > 186.0f) {
				mCurrentSpeed = 186.0f;
			}
		}
	}

	Vector2 moveAmount = moveDir * mCurrentSpeed * mTimer->DeltaTime(); // Convert km/h to m/s
	if (moveDir.x != 0) {
		moveAmount.x = moveDir.x * mHorizontalMoveSpeed * mTimer->DeltaTime(); // Convert km/h to m/s
	}
	Translate(moveAmount, World);

	if (!(mInput->KeyDown(SDL_SCANCODE_W) || mInput->KeyDown(SDL_SCANCODE_UP))) {
		Vector2 pos = Position();
		pos.y += Gravity * mTimer->DeltaTime(); // Move down constantly
		Position(pos);
	}

	// Keep player within movement bounds
	Vector2 pos = Position(Local);
	if (pos.x < mMoveBoundsX.x) {
		pos.x = mMoveBoundsX.x;
	}
	else if (pos.x > mMoveBoundsX.y) {
		pos.x = mMoveBoundsX.y;
	}

	if (pos.y < mMoveBoundsY.x) {
		pos.y = mMoveBoundsY.x;
	}
	else if (pos.y > mMoveBoundsY.y) {
		pos.y = mMoveBoundsY.y;
	}

	Position(pos);

	// Check if the player is off the highway
	if (IsOffHighway()) {
		mAnimating = true;
		mDeathAnimation->ResetAnimation();
		mAudio->PlaySFX("SFX/PlayerExplosion.wav");
		mWasHit = true;
	}
}

bool Player::IsOffHighway() {
	Vector2 pos = Position(Local);
	return (pos.x < mMoveBoundsX.x || pos.x > mMoveBoundsX.y);
}

//void Player::HandleFiring() {
//	if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
//		for (int i = 0; i < MAX_BULLETS; ++i) {
//			if (!mBullets[i]->Active()) {
//				mBullets[i]->Fire(Position());
//				mAudio->PlaySFX("SFX/Fire.wav");
//				break;
//			}
//		}
//	}
//}

void Player::Visible(bool visible) {
	mVisible = visible;
}

bool Player::IsAnimating() {
	return mAnimating;
}

int Player::Score() {
	return mScore;
}

int Player::Lives() {
	return mLives;
}

void Player::AddScore(int change) {
	mScore += change;
}

float Player::DistanceTraveled() {
	return mDistanceTraveled;
}

float Player::GetSpeed() {
	
	// Calculate Speed based on movement conditions
	float speed = 50.0f; 
	if (mInput->KeyDown(SDL_SCANCODE_W) || mInput->KeyDown(SDL_SCANCODE_UP)) {
		speed = 62.0f; // Speed when moving up
	}
	else if (mInput->KeyDown(SDL_SCANCODE_S) || mInput->KeyDown(SDL_SCANCODE_DOWN)) {
		speed = 42.0f; // Speed when moving down
	}
	return mCurrentSpeed; 

}

bool Player::IgnoreCollisions()
{
	return !mVisible || mAnimating;
}

void Player::Hit(PhysEntity * other) {
	mLives -= 1;
	mAnimating = true;
	mDeathAnimation->ResetAnimation();
	mAudio->PlaySFX("SFX/PlayerExplosion.wav");
	mWasHit = true;
}

bool Player::WasHit() {
	return mWasHit;
}

void Player::Update() {
	if (mAnimating) {

		if (mWasHit) {
			mWasHit = false;
		}

		mDeathAnimation->Update();
		mAnimating = mDeathAnimation->IsAnimating();

		if (!mAnimating) {
			ScreenManager::Instance()->SetScreen(ScreenManager::Start);
		}
	}
	else {
		if (Active()) {
			HandleMovement();
	//		HandleFiring();
		}

		// Update distance traveled based on movement conditions
		float speed = mCurrentSpeed * (1000.0f / 3600.0f); // Convert km/h to m/s
		mDistanceTraveled += speed * mTimer->DeltaTime() / 1000.0f; // Convert to kilometers
	}

	/*for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Update();
	}*/
}

void Player::Render() {
	if (mVisible) {
		if (mAnimating) {
			mDeathAnimation->Render();
		}
		else {
			mTexture->Render();
		}
	}

	/*for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Render();
	}*/

	PhysEntity::Render();
}
