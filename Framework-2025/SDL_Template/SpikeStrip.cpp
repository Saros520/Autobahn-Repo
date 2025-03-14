#include "SpikeStrip.h"
#include "PhysicsManager.h"
#include "BoxCollider.h" 
#include "MathHelper.h" 

SpikeStrip::SpikeStrip() {
	mTimer = Timer::Instance();
	mTexture = new Texture("SpikeStrip.png", 18, 25, 56, 6);
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero); 

	// Adjust the size of the box collider and scale the texture as needed
	Vector2 colliderSize = Vector2(85.0f, 20.0f);  
	mTexture->Scale(Vector2(1.5f, 1.5f));  // Scale the texture if needed
	AddCollider(new BoxCollider(colliderSize, Vec2_Zero));
	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::HostileProjectiles);

	Tag("SpikeStrip");
}

SpikeStrip::~SpikeStrip() {
	delete mTexture;
	mTexture = nullptr;

	PhysicsManager::Instance()->UnregisterEntity(mId);

}

void SpikeStrip::Update() {
	// Move the spike strip down the screen
	Translate(Vector2(0.0f, 560.0f * mTimer->DeltaTime()), World);

	// Update the position of the collider to follow the spike strip
	for (auto collider : mColliders) {
		BoxCollider* boxCollider = static_cast<BoxCollider*>(collider);
		boxCollider->SetOffset(mColliderOffset);
	}
}

void SpikeStrip::Render() {
	mTexture->Render();
	//PhysEntity::Render();

}

bool SpikeStrip::IsOutOfBounds() {
	return Position().y > Graphics::SCREEN_HEIGHT;
}

void SpikeStrip::Destroy() {
	PhysicsManager::Instance()->UnregisterEntity(mId);
	delete this;
}
