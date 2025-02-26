#ifndef __ENEMY_H
#define __ENEMY_H
#include "AnimatedGLTexture.h"
#include "AudioManager.h"
#include "PhysEntity.h"

using namespace SDLFramework;

class Enemy : public PhysEntity {
private:

	AudioManager* mAudio;

	bool mAnimating;

	Texture* mTexture;
	AnimatedGLTexture* mDeathAnimation;

public:
	Enemy();
	~Enemy();

	bool IsAnimating();

<<<<<<< Updated upstream
	// Inherited from PhysEntity
	bool IgnoreCollisions() override;
	void Hit(PhysEntity* other) override;
=======
    static void InitializeLanes();
    static const std::vector<float>& GetLeftLanes();
    static const std::vector<float>& GetRightLanes();

    bool IsAnimating();
    bool IgnoreCollisions() override;
    void Hit(PhysEntity* other) override;
    void Update() override;
    void Render() override;

    Vector2 GetTextureDimensions() const;
    void SetMoveDirection(const Vector2& direction);
    void StartTurning(float targetRotation, const Vector2& targetDirection, float turnSpeedFactor);
    void SetSpeedMultiplier(float multiplier);
    void Destroy();
>>>>>>> Stashed changes

	void Update() override;
	void Render() override;
};
#endif