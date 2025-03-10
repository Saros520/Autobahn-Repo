#include "EnemyPolice.h"
#include "Player.h"
#include "Graphics.h"
#include "ScreenManager.h"
#include "PhysicsManager.h"
#include "PlayScreen.h"
#include <iostream>
#include <cmath>

EnemyPolice* EnemyPolice::sActivePoliceCar = nullptr;
float EnemyPolice::sChaseDuration = 0.0f; // Initialize static chase duration

EnemyPolice::EnemyPolice(Player* player, EnemySpawner* enemySpawner, bool isTopPoliceCar)
    : Enemy(isTopPoliceCar ? false : true, 18), mEnemySpawner(enemySpawner), mAvoiding(false), mDestroyed(false), mIsTopPoliceCar(isTopPoliceCar) {
    mTimer = Timer::Instance();
    mPlayer = player;
    mChasing = false;
    mBaseSpeed = 60.0f;
    mSpikeStripTimer = 0.0f;
    mSpikeStripInterval = 5.0f;

    PhysicsManager::Instance()->UnregisterEntity(mId);
    mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Police);

    sActivePoliceCar = this; // Set the active police car

    Tag("PoliceCar");
}

EnemyPolice::~EnemyPolice() {
    mTimer = nullptr;
    mPlayer = nullptr;
    mEnemySpawner = nullptr;
    sActivePoliceCar = nullptr; // clearing the active police car

    PhysicsManager::Instance()->UnregisterEntity(mId);
}

void EnemyPolice::Update() {
    if (mDestroyed) {
        return; // Do not update if the object is destroyed
    }

    // Call the base class update method first
    Enemy::Update();

    if (mChasing) {
        sChaseDuration += mTimer->DeltaTime();
        mSpikeStripTimer += mTimer->DeltaTime();

        // Predict the player car's future position
        Vector2 playerPos = mPlayer->Position();
        Vector2 playerVelocity = mPlayer->GetVelocity();
        Vector2 predictedPlayerPos = playerPos + playerVelocity * 0.75f; // Predict 0.75 seconds ahead

        // Calculate the direction towards the predicted player position
        Vector2 direction = (predictedPlayerPos - Position()).Normalized();
        float speed = mBaseSpeed;
        Vector2 moveAmount = direction * speed * mTimer->DeltaTime();

        // For the top police car
        if (mIsTopPoliceCar) {
            // Move horizontally to track the player car
            if (predictedPlayerPos.x < Position().x) {
                moveAmount = Vector2(-speed * mTimer->DeltaTime(), 0.0f);
            }
            else if (predictedPlayerPos.x > Position().x) {
                moveAmount = Vector2(speed * mTimer->DeltaTime(), 0.0f);
            }

            // Ensure the top police car stays near the top of the screen
            if (Position().y > 50.0f) {
                moveAmount.y = -speed * mTimer->DeltaTime();
            }
            else {
                moveAmount.y = 0.0f;
            }
        }
        else {
            // For the bottom police car
            if (predictedPlayerPos.y > Position().y) {
                direction = (playerPos - Position()).Normalized();
                moveAmount = direction * speed * mTimer->DeltaTime();
            }
        }

        mAvoiding = false;
        bool pathBlocked = false;
        Vector2 separationForce(0.0f, 0.0f);

        // Check if there is an enemy vehicle between the police car and the player car
        for (Enemy* enemy : mEnemySpawner->GetEnemies()) {
            if (enemy != this && enemy != nullptr) {
                Vector2 enemyPos = enemy->Position();
                Vector2 toEnemy = enemyPos - Position();
                Vector2 toPlayer = playerPos - Position();
                float distanceToEnemy = toEnemy.Magnitude();
                float verticalDistance = std::abs(enemyPos.y - Position().y);
                float horizontalDistance = std::abs(enemyPos.x - Position().x);

                if (verticalDistance < 300.0f && horizontalDistance < 150.0f) {
                    Vector2 avoidDirection = (Position() - enemyPos).Normalized();
                    separationForce += Vector2(avoidDirection.x / distanceToEnemy, avoidDirection.y / distanceToEnemy) * 1.0f;
                    mAvoiding = true;
                }

                if ((mIsTopPoliceCar && enemyPos.y > playerPos.y && enemyPos.y < Position().y && std::abs(enemyPos.x - Position().x) < 0.4f) ||
                    (!mIsTopPoliceCar && enemyPos.y < playerPos.y && enemyPos.y > Position().y && std::abs(enemyPos.x - Position().x) < 0.4f) ||
                    (std::abs(toEnemy.Normalized().x - toPlayer.Normalized().x) < 0.2f && distanceToEnemy < toPlayer.Magnitude())) {
                    pathBlocked = true;
                }
            }
        }

        // Apply separation force
        if (mAvoiding) {
            moveAmount += separationForce * speed * mTimer->DeltaTime();
            speed = mBaseSpeed * 0.8f;
        }

        // If the path is blocked, switch lanes to avoid the enemy vehicle
        if (pathBlocked) {
            if (Position().x < Graphics::SCREEN_WIDTH * 0.5f) {
                moveAmount = Vector2(speed * mTimer->DeltaTime(), mIsTopPoliceCar ? -0.9f : 0.9f);
            }
            else {
                moveAmount = Vector2(-speed * mTimer->DeltaTime(), mIsTopPoliceCar ? -0.9f : 0.9f);
            }
        }
        else if (!mAvoiding) {
            speed = mBaseSpeed;
        }

        // Logic for spawning police cars
        if ((mIsTopPoliceCar && Position().y > 0) || (!mIsTopPoliceCar && Position().y < Graphics::SCREEN_HEIGHT)) {
            Translate(moveAmount, World);
        }

        Translate(moveAmount, World);

        if (mIsTopPoliceCar && mSpikeStripTimer >= mSpikeStripInterval) {
            LaySpikeStrip();
            mSpikeStripTimer = 0.0f;
        }

        // Debug output to verify the position
        Vector2 pos = Position();
        std::cout << "Police car position: (" << pos.x << ", " << pos.y << ")" << std::endl;

        // Check if the police car has been chasing for 1 minute
        if (sChaseDuration >= 60.0f) {
            StopChase();
        }

        // Check for collision with other enemy vehicles
        for (Enemy* enemy : mEnemySpawner->GetEnemies()) {
            if (enemy != this && enemy != nullptr && CheckCollision(enemy)) {
                enemy->Destroy();
                Destroy();
                if (sChaseDuration < 60.0f) {
                    SpawnNewPoliceCar(mPlayer, mEnemySpawner, mIsTopPoliceCar);
                }
                return;
            }
        }
    }
}

void EnemyPolice::Render() {
    if (mDestroyed) {
        return; // Do not render if the object is destroyed
    }

    Enemy::Render();
}

void EnemyPolice::StartChase() {
    mChasing = true;
}

void EnemyPolice::StopChase() {
    mChasing = false;

    if (Active()) {
        Vector2 direction = mIsTopPoliceCar ? Vector2(0.0f, -1.0f) : Vector2(0.0f, 1.0f);
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
                if (verticalDistance < 300.0f && horizontalDistance < 150.0f) { // Adjust the distance thresholds as needed
                    Vector2 avoidDirection = (Position() - enemyPos).Normalized();
                    separationForce += Vector2(avoidDirection.x / distanceToEnemy, avoidDirection.y / distanceToEnemy) * 1.0f; // Stronger force for closer enemies
                    mAvoiding = true;
                }
            }
        }

        // Apply separation force
        if (mAvoiding) {
            moveAmount += separationForce * speed * mTimer->DeltaTime();
            speed = mBaseSpeed * 0.8f;
        }

        if (!mAvoiding) {
            speed = mBaseSpeed;
        }

        Translate(moveAmount, World);

        if ((mIsTopPoliceCar && Position().y < -50.0f) || (!mIsTopPoliceCar && Position().y > Graphics::SCREEN_HEIGHT + 50.0f)) {
            Destroy();
        }
    }
    else {
        Destroy();
    }
}

void EnemyPolice::Destroy() {
    // Properly delete the police car
    sActivePoliceCar = nullptr;
    mDestroyed = true; // Set the destroyed flag
    PhysicsManager::Instance()->UnregisterEntity(mId);
}

void EnemyPolice::LaySpikeStrip() {
    SpikeStrip* spikeStrip = new SpikeStrip();
    spikeStrip->Position(Position() + Vector2(0.0f, 50.0f));
    mPlayScreen->AddSpikeStrip(spikeStrip);
}

void EnemyPolice::SpawnNewPoliceCar(Player* player, EnemySpawner* enemySpawner, bool isTopPoliceCar) {
    // Spawn a new police car if there isn't already an active one
    if (sActivePoliceCar == nullptr) {
        EnemyPolice* newPoliceCar = new EnemyPolice(player, enemySpawner, isTopPoliceCar);

        // Set the initial positions of both police cars 
        float spawnY = isTopPoliceCar ? -50.0f : Graphics::SCREEN_HEIGHT + 50.0f;
        newPoliceCar->Position(Vector2(Graphics::SCREEN_WIDTH * 0.5f, spawnY));

        // Debug output to verify the position
        Vector2 pos = newPoliceCar->Position();
        std::cout << "Police car spawned at position: (" << pos.x << ", " << pos.y << ")" << std::endl;

        newPoliceCar->StartChase();
    }
}
