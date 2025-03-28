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

    mStartPosition = Vector2(Graphics::SCREEN_WIDTH * 0.503f, Graphics::SCREEN_HEIGHT * 0.9f);
    Position(mStartPosition);  // Set the initial position of the car

    mCollisionCooldownTime = 0.0f;
    mIsCooldownActive = false;
    mDeathAnimation = new AnimatedTexture("CarExplosion.png", 0, 0, 128, 128, 5, 1.0f, Animation::Layouts::Horizontal);
    mDeathAnimation->Parent(this);
    mDeathAnimation->Position(Vec2_Zero);
    mDeathAnimation->SetWrapMode(Animation::WrapModes::Once);

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

    PhysicsManager::Instance()->UnregisterEntity(mId);
}

void Player::HandleMovement() {
    const float Gravity = 100.0f;
    Vector2 moveDir = Vec2_Zero;

    if (mInput->KeyDown(SDL_SCANCODE_W) || mInput->KeyDown(SDL_SCANCODE_UP)) {
        moveDir.y -= 1;
        mAudio->SetVolume(0.1f);
        mAudio->PlaySFX("CarRev.MP3");
    }
    else if (!(mInput->KeyDown(SDL_SCANCODE_W) || mInput->KeyDown(SDL_SCANCODE_UP))) {
        mAudio->StopSFX("CarRev.MP3"); // Stop sound when the key is not pressed
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
        mMoveDirection = moveDir; // Update mMoveDirection
        if (moveDir.y > 0) {
            mCurrentSpeed -= mDeceleration * mTimer->DeltaTime();
            if (mCurrentSpeed < 150.0f) {
                mCurrentSpeed = 150.0f;
            }
        }
        else if (moveDir.y < 0) {
            mCurrentSpeed += mAcceleration * mTimer->DeltaTime();
            if (mCurrentSpeed > mMoveSpeed) {
                mCurrentSpeed = mMoveSpeed;
            }
        }
    }
    else {
        if (mCurrentSpeed > 186.0f) {
            mCurrentSpeed -= mDeceleration * mTimer->DeltaTime();
            if (mCurrentSpeed < 186.0f) {
                mCurrentSpeed = 186.0f;
            }
        }
        else if (mCurrentSpeed < 186.0f) {
            mCurrentSpeed += mAcceleration * mTimer->DeltaTime();
            if (mCurrentSpeed > 186.0f) {
                mCurrentSpeed = 186.0f;
            }
        }
    }

    Vector2 moveAmount = moveDir * mCurrentSpeed * mTimer->DeltaTime();
    if (moveDir.x != 0) {
        moveAmount.x = moveDir.x * mHorizontalMoveSpeed * mTimer->DeltaTime();
    }
    Translate(moveAmount, World);

    if (!(mInput->KeyDown(SDL_SCANCODE_W) || mInput->KeyDown(SDL_SCANCODE_UP))) {
        Vector2 pos = Position();
        pos.y += Gravity * mTimer->DeltaTime();
        Position(pos);
    }

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

    if (IsOffHighway()) {
        mAnimating = true;
        mDeathAnimation->ResetAnimation();
        mAudio->SetVolume(0.2f);
        mWasHit = true;
    }
}

bool Player::IsOffHighway() {
    Vector2 pos = Position(Local);
    return (pos.x < mMoveBoundsX.x || pos.x > mMoveBoundsX.y);
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

void Player::AddScore(int change) {
    mScore += change;
}

float Player::DistanceTraveled() const {
    return mDistanceTraveled;
}

Vector2 Player::GetVelocity() const {
    return mMoveDirection * mCurrentSpeed;
}

float Player::GetSpeed() {
    float speed = 50.0f;
    if (mInput->KeyDown(SDL_SCANCODE_W) || mInput->KeyDown(SDL_SCANCODE_UP)) {
        speed = 62.0f;
    }
    else if (mInput->KeyDown(SDL_SCANCODE_S) || mInput->KeyDown(SDL_SCANCODE_DOWN)) {
        speed = 42.0f;
    }
    return mCurrentSpeed;
}

bool Player::IgnoreCollisions() {
    return !mVisible || mAnimating;
}

bool Player::IsOutOfLives() {
    return mLives <= 0;
}

void Player::Hit(PhysEntity* other) {
    if (mIsCooldownActive) {
        // If the cooldown is active, ignore collisions
        return;
    }
    
    if (other->Tag() == "PoliceCar" || other->Tag() == "SpikeStrip") {
       
        mAudio->PlaySFX("SFX/CrashSound.wav");
        mAnimating = true;
        mDeathAnimation->ResetAnimation();
        mAudio->PlaySFX("SFX/CrashSound.wav");
        mWasHit = true;
        mWasHitByPolice = true;
        //Position(mStartPosition);
    }
    else if (mLives <= 0) {
        // Normal collision behavior (only applies if NOT PoliceCar or SpikeStrip)
        mWasHit = true;
        mAnimating = true;
        mAudio->PlaySFX("SFX/CrashSound.wav");
       
    }
    else {
        mAnimating = true;
        mDeathAnimation->ResetAnimation();
        mAudio->PlaySFX("SFX/CrashSound.wav");
        mWasHit = true;
		//Position(mStartPosition);
    }

    mLives -= 1;

    // Play the crash sound only during a collision
    mAudio->SetVolume(0.1f);
    mAudio->PlaySFX("CrashSound.wav");

    // Start the cooldown timer
    mIsCooldownActive = true;
    mCollisionCooldownTime = 1.0f;  // 1 second cooldown

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

        mDeathAnimation->Update();
        mAnimating = mDeathAnimation->IsAnimating();
        if (!mAnimating) {
            Position(mStartPosition);
        }
    }
    else {
        if (Active()) {
            HandleMovement();
        }

        // Update distance traveled based on movement conditions
        float speed = mCurrentSpeed * (1000.0f / 3600.0f); // Convert km/h to m/s
        mDistanceTraveled += speed * mTimer->DeltaTime() / 1000.0f; // Convert to kilometers
    }
    // Cooldown logic: decrement the cooldown time if active
    if (mIsCooldownActive) {
        mCollisionCooldownTime -= mTimer->DeltaTime();
        if (mCollisionCooldownTime <= 0.0f) {
            mIsCooldownActive = false;  // Cooldown is over
        }
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

    //PhysEntity::Render();
}

int Player::Lives() {
    return mLives;
}

void Player::ResetLives() {
    mLives = 3;
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
