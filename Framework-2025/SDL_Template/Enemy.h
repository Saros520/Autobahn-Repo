#ifndef __ENEMY_H
#define __ENEMY_H

#include "Texture.h"
#include "AnimatedGLTexture.h"
#include "AudioManager.h"
#include "PhysEntity.h"
#include "Timer.h"
#include <vector>

using namespace SDLFramework;

class Enemy : public PhysEntity {
private:
    Timer* mTimer;
    AnimatedTexture* mDeathAnimation;
    AudioManager* mAudio;
    bool mAnimating;
    float mMoveSpeed;
    Vector2 mMoveDirection;

    static std::vector<float> mLeftLanes;
    static std::vector<float> mRightLanes;

    void SetColliderForVehicle(int vehicleIndex);
    void SetScaleForVehicle(int vehicleIndex);
    void SetSpeedForVehicle(int vehicleIndex);

public:
    Enemy(bool moveDownward, int vehicleIndex);
    ~Enemy();

    Texture* mTexture;

    static void InitializeLanes();
    static const std::vector<float>& GetLeftLanes();
    static const std::vector<float>& GetRightLanes();

    bool IsAnimating();
    bool IgnoreCollisions() override;
    void Hit(PhysEntity* other) override;
    void Update() override;
    void Render() override;

    Vector2 GetTextureDimensions() const;

};
#endif
