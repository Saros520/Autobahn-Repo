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
	AnimatedTexture* mDeathAnimation;

public:
	Enemy();
	~Enemy();

	bool IsAnimating();

	// Inherited from PhysEntity
	bool IgnoreCollisions() override;
	void Hit(PhysEntity* other) override;

	void Update() override;
	void Render() override;
};
#endif