#ifndef __PLAYER_H
#define __PLAYER_H
#include "AnimatedTexture.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "Bullet.h"

using namespace SDLFramework;

class Player : public PhysEntity {
private:
	Timer * mTimer;
	InputManager * mInput;
	AudioManager * mAudio;

	bool mVisible;
	bool mAnimating;
	bool mWasHit;

	int mScore;
	int mLives;

	Texture * mTexture;
	AnimatedTexture * mDeathAnimation;

	float mMoveSpeed;
	float mHorizontalMoveSpeed;
	float mAcceleration;
	float mDeceleration;
	float mCurrentSpeed;
	Vector2 mMoveBoundsX;
	Vector2 mMoveBoundsY;

	float mDistanceTraveled;

	static const int MAX_BULLETS = 2;
	Bullet * mBullets[MAX_BULLETS];

	void HandleMovement();
	void HandleFiring();

	bool IsOffHighway();

public:
	Player();
	~Player();

	void SetCarTexture(int carIndex);

	void Visible(bool visible);
	bool IsAnimating();

	int Score();
	int Lives();

	void AddScore(int change);

	float DistanceTraveled();
	float GetSpeed();

	// Inherited from PhysEntity
	bool IgnoreCollisions() override;
	void Hit(PhysEntity * other) override;
	
	bool WasHit();

	void SetMoveBounds(float top, float bottom, float left, float right);

	void Update() override;
	void Render() override;
};
#endif