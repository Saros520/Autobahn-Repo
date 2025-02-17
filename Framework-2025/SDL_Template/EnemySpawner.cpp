
#include "EnemySpawner.h"
#include "Graphics.h"
#include <cstdlib>
#include <ctime>

EnemySpawner::EnemySpawner(float spawnInterval) {
    mTimer = Timer::Instance();
    mSpawnInterval = spawnInterval;
    mTimeSinceLastSpawn = 0.0f;

    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initialize lanes for enemy spawning
    if (Enemy::GetLeftLanes().empty() || Enemy::GetRightLanes().empty()) {
        Enemy::InitializeLanes();
    }
}

EnemySpawner::~EnemySpawner() {
    mTimer = nullptr;

    // Clean up active enemies
    for (auto enemy : mEnemies) {
        delete enemy;
    }
    mEnemies.clear();
}

void EnemySpawner::Update() {
    mTimeSinceLastSpawn += mTimer->DeltaTime();

    if (mTimeSinceLastSpawn >= mSpawnInterval) {
        SpawnEnemy();
        mTimeSinceLastSpawn = 0.0f;
    }

    // Update active enemies
    for (auto enemy : mEnemies) {
        enemy->Update();
    }

    // Remove enemies that are off the screen
    mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(), [](Enemy* enemy) {
        Vector2 pos = enemy->Position();
        return (pos.y > Graphics::SCREEN_HEIGHT || pos.y < -enemy->GetTextureDimensions().y);
        }), mEnemies.end());
}

void EnemySpawner::SpawnEnemy() {
    // Get lanes from enemy class
    const std::vector<float>& leftLanes = Enemy::GetLeftLanes();
    const std::vector<float>& rightLanes = Enemy::GetRightLanes();

    // Spawn enemy vehicles at random intervals
    if (std::rand() % 2 == 0) { // 50% chance to spawn an enemy on the left side
        // Choose a random lane
        int laneIndex = std::rand() % leftLanes.size();
        float laneX = leftLanes[laneIndex];

        // Create a new enemy vehicle
        Enemy* enemy = new Enemy();
        enemy->Position(Vector2(laneX, -enemy->GetTextureDimensions().y)); // Spawn just above the top of the play screen
        mEnemies.push_back(enemy);
    }
    else { // 50% chance to spawn an enemy on the right side
        // Choose a random lane
        int laneIndex = std::rand() % rightLanes.size();
        float laneX = rightLanes[laneIndex];

        // Create a new enemy vehicle
        Enemy* enemy = new Enemy();
        enemy->Position(Vector2(laneX, Graphics::SCREEN_HEIGHT + enemy->GetTextureDimensions().y)); // Spawn just below the bottom of the play screen
        mEnemies.push_back(enemy);
    }
}

void EnemySpawner::Render() {
    // Render active enemies
    for (auto enemy : mEnemies) {
        enemy->Render();
    }
}
