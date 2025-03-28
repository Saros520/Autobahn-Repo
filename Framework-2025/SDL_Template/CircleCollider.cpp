#include "CircleCollider.h"

CircleCollider::CircleCollider(float radius, bool broadPhase) 
: Collider(ColliderType::Circle) {
	mRadius = radius;

	if (DEBUG_COLLIDERS) {
		if (broadPhase) {
			SetDebugTexture(new Texture("Framework/BroadPhaseCollider.png"));
		}
		else {
			SetDebugTexture(new Texture("Framework/CircleCollider.png"));
		}
		// circle collider file size is 50px
		mDebugTexture->Scale(Vec2_One * (radius * 2 / 50.0f));
	}
}

CircleCollider::~CircleCollider() {
}

Vector2 CircleCollider::GetFurthestPoint()
{
	return Vec2_Right * (mRadius + Position(GameEntity::Local).Magnitude());
}

float CircleCollider::GetRadius()
{
	return mRadius;
}
