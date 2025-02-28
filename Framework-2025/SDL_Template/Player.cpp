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
	mHorizontalMoveSpeed = 200.0f;
	mAcceleration = 40.0f;
	mDeceleration = 30.0f;
	mCurrentSpeed = 186.0f;
	mMoveBoundsX = Vector2(120.0f + mTexture->ScaledDimensions().x / 2, 780.0f - mTexture->ScaledDimensions().x / 2);
	mMoveBoundsY = Vector2(90.0f, Graphics::SCREEN_HEIGHT);

	mDistanceTraveled = 0.0f;

	Position(Vector2(Graphics::SCREEN_WIDTH * 0.503f, Graphics::SCREEN_HEIGHT * 0.9f));

	mDeathAnimation = new AnimatedTexture("CarExplosion.png", 0, 0, 128, 128, 5, 1.0f, Animation::Layouts::Horizontal);
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

	AddCollider(new BoxCollider(mTexture->ScaledDimensions()));
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
	PhysicsManager::Instance()->UnregisterEntity(mId);
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

void Player::AddScore(int change) {
	mScore += change;
}

float Player::DistanceTraveled() const {
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

bool Player::IsOutOfLives() {
	return mLives <= 0;
}

void Player::Hit(PhysEntity* other) {
	if (other->Tag() == "PoliceCar") {
		mWasHitByPolice = true;
	}

	mLives -= 1;

	if (mLives <= 0) {
		mAnimating = true;
		mAudio->PlaySFX("SFX/PlayerExplosion.wav");
	}
	else {
		mAnimating = true;
		mDeathAnimation->ResetAnimation();
		mAudio->PlaySFX("SFX/PlayerExplosion.wav");
		mWasHit = true;
	}
}

bool Player::WasHit() {
	return mWasHit;
}

bool Player::WasHitByPolice() const {
	return mWasHitByPolice;
}

void Player::Update() {
	if (mAnimating) {

		if (mWasHit) {
			mWasHit = false;
		}

		//mDeathAnimation->Update();
		//mAnimating = mDeathAnimation->IsAnimating();

		if (!mAnimating && mLives <= 0) {
			ScreenManager::Instance()->SetScreen(ScreenManager::Start);
		}
		mDeathAnimation->Update();
		mAnimating = mDeathAnimation->IsAnimating();
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

int Player::Lives() {
	return mLives;
}

void Player::ResetLives() {
	mLives = 3;  // Reset lives when starting a new game
}

void Player::Reset() {
	mScore = 0;
	mLives = 3;
	mCurrentSpeed = 186.0f;
	mDistanceTraveled = 0.0f;
	mAnimating = false;
	mWasHit = false;
	mWasHitByPolice = false;
	mVisible = true;
	Position(Vector2(Graphics::SCREEN_WIDTH * 0.503f, Graphics::SCREEN_HEIGHT * 0.9f));
}
