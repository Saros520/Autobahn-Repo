#ifndef __PHYSENTITY_H
#define __PHYSENTITY_H

#include <vector>
#include "Collider.h"

class PhysEntity : public GameEntity {
protected:
	unsigned long mId;
	std::string mTag;

	std::vector<Collider*> mColliders;

	Collider * mBroadPhaseCollider;

	void AddCollider(Collider * collider, Vector2 localPos = Vec2_Zero);

	virtual bool IgnoreCollisions();

public:
	PhysEntity();
	virtual ~PhysEntity();

	unsigned long GetId();

	std::string Tag();
	void Tag(std::string value);

	bool CheckCollision(PhysEntity * other);

	virtual void Hit(PhysEntity * other) { }

	virtual void Render() override;
};
#endif