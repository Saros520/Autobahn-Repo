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
	mLives = 2;

	mTexture = nullptr;

	SetCarTexture(0);

	mMoveSpeed = 400.0f;
	mMoveBoundsX = Vector2(20.0f + mTexture->ScaledDimensions().x / 2, Graphics::SCREEN_WIDTH - mTexture->ScaledDimensions().x / 2);
	mMoveBoundsY = Vector2(110.0f, Graphics::SCREEN_HEIGHT); // Assuming the player score texture is positioned at y = 80

	Position(Vector2(Graphics::SCREEN_WIDTH * 0.75f, Graphics::SCREEN_HEIGHT * 0.9f));

	mDeathAnimation = new AnimatedTexture("EnemyExplosion.png", 0, 0, 128, 128, 5, 1.0f, Animation::Layouts::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i] = new Bullet(true);
	}

	/*AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));
	AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2( 18.0f, 10.0f));
	AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(-18.0f, 10.0f));*/

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);
}

void Player::SetCarTexture(int carIndex) {
	std::string fileName = "PlayerCar" + std::to_string(carIndex + 1) + ".png";
	delete mTexture;
	mTexture = new Texture(fileName);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero);
}

Player::~Player() {
	mTimer = nullptr;
	mInput = nullptr;
	mAudio = nullptr;

	delete mTexture;
	mTexture = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;

	for (auto b : mBullets) {
		delete b;
	}
}

void Player::HandleMovement() {
	const float Gravity = 150.0f;
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
		moveDir = moveDir.Normalized() * mMoveSpeed * mTimer->DeltaTime();
		Translate(moveDir, World);
	}

	if (moveDir.MagnitudeSqr() > 0.0f) {
		moveDir = moveDir.Normalized() * mMoveSpeed * mTimer->DeltaTime();
		Translate(moveDir, World);
	}

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

void Player::HandleFiring() {
	if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
		for (int i = 0; i < MAX_BULLETS; ++i) {
			if (!mBullets[i]->Active()) {
				mBullets[i]->Fire(Position());
				mAudio->PlaySFX("SFX/Fire.wav");
				break;
			}
		}
	}
}

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
			HandleFiring();
		}
	}

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Update();
	}
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

	for (int i = 0; i < MAX_BULLETS; ++i) {
		mBullets[i]->Render();
	}

	PhysEntity::Render();
}
