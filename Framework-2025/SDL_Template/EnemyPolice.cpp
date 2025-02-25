#include "EnemyPolice.h"
#include "Graphics.h"
#include "ScreenManager.h"
#include "PhysicsManager.h"
#include <iostream>

EnemyPolice* EnemyPolice::sActivePoliceCar = nullptr;

EnemyPolice::EnemyPolice(Player* player, EnemySpawner* enemySpawner)
	: Enemy(true, 18), mEnemySpawner(enemySpawner), mAvoiding(false) {
	mTimer = Timer::Instance();
	mPlayer = player;
	mChaseDuration = 0.0f;
	mChasing = false;
	mBaseSpeed = 50.0f;

	PhysicsManager::Instance()->UnregisterEntity(mId);
	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Police);

	sActivePoliceCar = this; // Set the active police car
}

EnemyPolice::~EnemyPolice() {
	mTimer = nullptr;
	mPlayer = nullptr;
	mEnemySpawner = nullptr;
	sActivePoliceCar = nullptr; // clearing the active police car
}

void EnemyPolice::Update() {
	//std::cout << "Number of Enemies in EnemyPolice: " << mOtherEnemies.size() << std::endl;

	if (mChasing) {
		mChaseDuration += mTimer->DeltaTime();

		// Move towards the player car
		Vector2 playerPos = mPlayer->Position();
		Vector2 direction = (playerPos - Position()).Normalized();
		float speed = mBaseSpeed;
		Vector2 moveAmount = direction * speed * mTimer->DeltaTime();

		mAvoiding = false;

		// Sense this police car is after you it will avoid the other sprites
		for (Enemy* enemy : mOtherEnemies) {
			if (enemy != this && enemy != nullptr) {
				if (CheckCollision(enemy)) {
					Vector2 enemyPos = enemy->Position();
					Vector2 avoidDirection = (Position() - enemyPos).Normalized();
					moveAmount += avoidDirection * speed * mTimer->DeltaTime();
					speed = mBaseSpeed * 0.75f;
					mAvoiding = true;
				}
				
		for (Enemy* enemy : mEnemySpawner->GetEnemies()) {
			if (enemy != this && enemy != nullptr && CheckCollision(enemy)) {
				Vector2 enemyPos = enemy->Position();
				Vector2 avoidDirection = (Position() - enemyPos).Normalized();
				moveAmount += avoidDirection * speed * mTimer->DeltaTime();
				speed = mBaseSpeed * 0.75f;
				mAvoiding = true;
			}
		}

		// If not avoiding other vehicles, resume base speed
		if (!mAvoiding) {
			speed = mBaseSpeed;
		}

		// Ensure the police car starts moving only after it has been spawned below the play screen
		if (Position().y < Graphics::SCREEN_HEIGHT) {
			Translate(moveAmount, World);
		}

		Translate(moveAmount, World);

		// Debug output to verify the position
		Vector2 pos = Position();
		std::cout << "Police car position: (" << pos.x << ", " << pos.y << ")" << std::endl;

		// Check if the police car has been chasing for 1 minute
		if (mChaseDuration >= 60.0f) {
			StopChase();
		}

		// Check for collision with the player car
		if (CheckCollision(mPlayer)) {
			ScreenManager::Instance()->SetScreen(ScreenManager::Start);
			// Display "Busted" message
		}
	}

	Enemy::Update();
}

void EnemyPolice::Render() {
	Enemy::Render();
}

void EnemyPolice::StartChase() {
	mChasing = true;
	mChaseDuration = 0.0f;
}

void EnemyPolice::StopChase() {
	mChasing = false;
	Active(false);
}

void EnemyPolice::Destroy() {
	// Properly delete the police car
	sActivePoliceCar = nullptr;
	delete this;
}

void EnemyPolice::SpawnNewPoliceCar(Player* player, EnemySpawner* enemySpawner) {
	// Spawn a new police car below the bottom of the play screen if there isn't already an active one
	if (sActivePoliceCar == nullptr) {
		EnemyPolice* newPoliceCar = new EnemyPolice(player, enemySpawner);

		// Set the initial position to being just below the playscreen 
		float spawnY = Graphics::SCREEN_HEIGHT + 50.0f;
		newPoliceCar->Position(Vector2(Graphics::SCREEN_HEIGHT * 0.5f, spawnY));

		// Debug output to verify the position
		Vector2 pos = newPoliceCar->Position();
		std::cout << "Police car spawned at position: (" << pos.x << ", " << pos.y << ")" << std::endl;

		newPoliceCar->StartChase();
	}
}
