#include "SpikeStrip.h"
#include "PhysicsManager.h"
#include "BoxCollider.h" 
#include "MathHelper.h" 

SpikeStrip::SpikeStrip() {
	mTimer = Timer::Instance();
	mTexture = new Texture("SpikeStrip.png");
	mTexture->Parent(this);
	mTexture->Position(Vec2_Zero); 

	AddCollider(new BoxCollider(mTexture->ScaledDimensions()));
	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::HostileProjectiles);

	Tag("SpikeStrip");
}

SpikeStrip::~SpikeStrip() {
	delete mTexture;
	mTexture = nullptr;

	PhysicsManager::Instance()->UnregisterEntity(mId);

}

void SpikeStrip::Update() {

}

void SpikeStrip::Render() {
	mTexture->Render();
	PhysEntity::Render();

}

bool SpikeStrip::IsOutOfBounds() {
	return Position().y > Graphics::SCREEN_HEIGHT;
}
