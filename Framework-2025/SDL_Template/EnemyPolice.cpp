#include "EnemyPolice.h"
#include "Graphics.h"
#include "ScreenManager.h"
#include "PhysicsManager.h"

EnemyPolice* EnemyPolice::sActivePoliceCar = nullptr;

EnemyPolice::EnemyPolice(Player* player, const std::vector<Enemy*>& otherEnemies)
	: Enemy(true, 18), mOtherEnemies(otherEnemies), mAvoiding(false) {
	mTimer = Timer::Instance();
	mPlayer = player;
	mChaseDuration = 0.0f;
	mChasing = false;
	mBaseSpeed = 210.0f;

	PhysicsManager::Instance()->UnregisterEntity(mId);
	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Police);
}

EnemyPolice::~EnemyPolice() {
	mTimer = nullptr;
	mPlayer = nullptr;
}

void EnemyPolice::Update() {
	std::cout << "Number of Enemies in EnemyPolice: " << mOtherEnemies.size() << std::endl;

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
			if (enemy != this && CheckCollision(enemy)) {
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

		// Limit this police car's vertical position to being next to the player car
		if (Position().y + moveAmount.y > playerPos.y) {
			moveAmount.y = playerPos.y - Position().y;
		}

		Translate(moveAmount, World);

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

void EnemyPolice::SpawnNewPoliceCar(Player* player, const std::vector<Enemy*>& otherEnemies) {
	// Spawn a new police car below the bottom of the play screen if there isn't already an active one
	if (sActivePoliceCar == nullptr) {
		EnemyPolice* newPoliceCar = new EnemyPolice(player, otherEnemies);
		newPoliceCar->StartChase();
	}
}
