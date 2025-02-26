#include "EnemyPolice.h"
#include "Graphics.h"
#include "ScreenManager.h"
#include "PhysicsManager.h"
#include <iostream>

EnemyPolice* EnemyPolice::sActivePoliceCar = nullptr;
float EnemyPolice::sChaseDuration = 0.0f; // Initialize static chase duration

EnemyPolice::EnemyPolice(Player* player, EnemySpawner* enemySpawner)
    : Enemy(true, 18), mEnemySpawner(enemySpawner), mAvoiding(false) {
    mTimer = Timer::Instance();
    mPlayer = player;
    mChasing = false;
    mBaseSpeed = 60.0f;

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
    if (mChasing) {
        sChaseDuration += mTimer->DeltaTime();

        // Move towards the player car
        Vector2 playerPos = mPlayer->Position();
        Vector2 direction = (playerPos - Position()).Normalized();
        float speed = mBaseSpeed;
        Vector2 moveAmount = direction * speed * mTimer->DeltaTime();

        mAvoiding = false;
        Vector2 separationForce(0.0f, 0.0f);

        // Advanced obstacle detection using raycasting
        for (Enemy* enemy : mEnemySpawner->GetEnemies()) {
            if (enemy != this && enemy != nullptr) {
                Vector2 enemyPos = enemy->Position();
                Vector2 toEnemy = enemyPos - Position();
                float distanceToEnemy = toEnemy.Magnitude();
                float verticalDistance = std::abs(enemyPos.y - Position().y);
                float horizontalDistance = std::abs(enemyPos.x - Position().x);

                // Check if the enemy is within a certain distance to avoid
                if (verticalDistance < 360.0f && horizontalDistance < 200.0f) { // Adjust the distance thresholds as needed
                    Vector2 avoidDirection = (Position() - enemyPos).Normalized();
                    separationForce += Vector2(avoidDirection.x / distanceToEnemy, avoidDirection.y / distanceToEnemy); // Stronger force for closer enemies
                    mAvoiding = true;
                }

                // Raycasting to detect obstacles
                Vector2 rayDirection = (enemyPos - Position()).Normalized();
                float rayLength = distanceToEnemy;
                Vector2 rayEnd = Position() + rayDirection * rayLength;

                // Check if the ray intersects with the enemy vehicle
                if (CheckRayIntersection(Position(), rayEnd, enemyPos, enemy->GetTextureDimensions())) {
                    mAvoiding = true;
                }
            }
        }

        // Apply separation force
        if (mAvoiding) {
            moveAmount += separationForce * speed * mTimer->DeltaTime();
            speed = mBaseSpeed * 0.75f;
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
        if (sChaseDuration >= 60.0f) {
            StopChase();
        }

        // Check for collision with the player car
        if (CheckCollision(mPlayer)) {
            ScreenManager::Instance()->SetScreen(ScreenManager::GameOver);
        }

        // Check for collision with other enemy vehicles
        for (Enemy* enemy : mEnemySpawner->GetEnemies()) {
            if (enemy != this && enemy != nullptr && CheckCollision(enemy)) {
                enemy->Destroy();
                Destroy();
                if (sChaseDuration < 60.0f) {
                    SpawnNewPoliceCar(mPlayer, mEnemySpawner);
                }
                return;
            }
        }

        Enemy::Update();
    }
}

bool EnemyPolice::CheckRayIntersection(const Vector2& rayStart, const Vector2& rayEnd, const Vector2& enemyPos, const Vector2& enemySize) {
    // Check if the ray intersects with the enemy vehicle's bounding box
    float minX = enemyPos.x - enemySize.x / 2.0f;
    float maxX = enemyPos.x + enemySize.x / 2.0f;
    float minY = enemyPos.y - enemySize.y / 2.0f;
    float maxY = enemyPos.y + enemySize.y / 2.0f;

    // Check if the ray intersects with the bounding box
    if (rayStart.x < minX && rayEnd.x < minX) return false;
    if (rayStart.x > maxX && rayEnd.x > maxX) return false;
    if (rayStart.y < minY && rayEnd.y < minY) return false;
    if (rayStart.y > maxY && rayEnd.y > maxY) return false;

    return true;
}

void EnemyPolice::Render() {
    Enemy::Render();
}

void EnemyPolice::StartChase() {
    mChasing = true;
}

void EnemyPolice::StopChase() {
    mChasing = false;

    if (Active()) {
        // Move down the play screen avoiding other enemy vehicles
        Vector2 direction = Vector2(0.0f, 1.0f); // Move downward
        float speed = mBaseSpeed;
        Vector2 moveAmount = direction * speed * mTimer->DeltaTime();

        mAvoiding = false;
        Vector2 separationForce(0.0f, 0.0f);

        for (Enemy* enemy : mEnemySpawner->GetEnemies()) {
            if (enemy != this && enemy != nullptr) {
                Vector2 enemyPos = enemy->Position();
                Vector2 toEnemy = enemyPos - Position();
                float distanceToEnemy = toEnemy.Magnitude();
                float verticalDistance = std::abs(enemyPos.y - Position().y);
                float horizontalDistance = std::abs(enemyPos.x - Position().x);

                // Check if the enemy is within a certain distance to avoid
                if (verticalDistance < 360.0f && horizontalDistance < 200.0f) { // Adjust the distance thresholds as needed
                    Vector2 avoidDirection = (Position() - enemyPos).Normalized();
                    separationForce += Vector2(avoidDirection.x / distanceToEnemy, avoidDirection.y / distanceToEnemy); // Stronger force for closer enemies
                    mAvoiding = true;
                }
            }
        }

        // Apply separation force
        if (mAvoiding) {
            moveAmount += separationForce * speed * mTimer->DeltaTime();
            speed = mBaseSpeed * 0.93f;
        }

        if (!mAvoiding) {
            speed = mBaseSpeed;
        }

        Translate(moveAmount, World);

        // Check if the police car is below the play screen
        if (Position().y > Graphics::SCREEN_HEIGHT + 50.0f) {
            Destroy();
        }
    }
    else {
        // Properly delete the police car if it is not active
        Destroy();
    }
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
