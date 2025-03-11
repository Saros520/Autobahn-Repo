#ifndef __BOXCOLLIDER_H
#define __BOXCOLLIDER_H

#include "Collider.h"

class BoxCollider : public Collider {
private:
	static const int MAX_VERTS = 4;

	GameEntity * mVerts[MAX_VERTS];
	Vector2 mSize;
	Vector2 mOffset;

	void AddVert(int index, Vector2 pos);

public:
	BoxCollider(Vector2 size, Vector2 offset = Vec2_Zero);
	~BoxCollider();

	Vector2 GetSize() const { return mSize; }
	void SetOffset(Vector2 offset) { mOffset = offset; }  
	Vector2 GetOffset() const { return mOffset; }  

	Vector2 GetFurthestPoint() override;
	Vector2 GetVertexPos(int index);
};
#endif