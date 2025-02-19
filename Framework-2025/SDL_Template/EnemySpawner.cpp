#include "EnemySpawner.h"
#include "Graphics.h"
#include <cstdlib>
#include <ctime>


EnemySpawner::EnemySpawner(float spawnInterval, int vehicleIndex) {
    mTimer = Timer::Instance();
    mSpawnInterval = 1.8f; // Spawns a vehicle every 1.8 seconds
    mTimeSinceLastSpawn = 0.0f;
    mVehicleIndex = vehicleIndex;

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
        bool offScreen = (pos.y > Graphics::SCREEN_HEIGHT + 140.0f || pos.y < -enemy->GetTextureDimensions().y);
        if (offScreen) {
            delete enemy;
        }
        return offScreen;
        }), mEnemies.end());
}

bool EnemySpawner::IsPositionOccupied(Vector2 position, float buffer) {
    for (auto enemy : mEnemies) {
        Vector2 enemyPos = enemy->Position();
        if (abs(enemyPos.y - position.y) < buffer) {
            return true;
        }
    }
    return false;
}

void EnemySpawner::SpawnEnemy() {
    // Get lanes from enemy class
    const std::vector<float>& leftLanes = Enemy::GetLeftLanes();
    const std::vector<float>& rightLanes = Enemy::GetRightLanes();

    // Define the vehicle indices for transport trucks
    std::vector<int> transportTrucks = { 7, 8, 13, 14 };

    // Weighted random selection to reduce the probability of transport trucks
    int vehicleIndex;
    if (std::rand() % 10 < 2) { // 20% chance to select a transport truck
        vehicleIndex = transportTrucks[std::rand() % transportTrucks.size()];
    }
    else { // 80% chance to select a regular vehicle
        do {
            vehicleIndex = std::rand() % 17 + 1;
        } while (std::find(transportTrucks.begin(), transportTrucks.end(), vehicleIndex) != transportTrucks.end());
    }

    Vector2 spawnPosition;
    bool positionOccupied;

    if (std::rand() % 2 == 0) { // 50% chance to spawn an enemy on the left side
        // Choose a random lane
        int laneIndex = std::rand() % leftLanes.size();
        float laneX = leftLanes[laneIndex];

        std::cout << "Spawning vehicle on the left." << std::endl;
        // Set spawn position
        spawnPosition = Vector2(laneX, -100.0f); // Spawn just above the top of the play screen

        // Check if the position is occupied
        positionOccupied = IsPositionOccupied(spawnPosition, 100.0f);
    }
    else { // 50% chance to spawn an enemy on the right side
        // Choose a random lane
        int laneIndex = std::rand() % rightLanes.size();
        float laneX = rightLanes[laneIndex];

        std::cout << "Spawning vehicle on the right." << std::endl;
        // Set spawn position
        spawnPosition = Vector2(laneX, Graphics::SCREEN_HEIGHT + 100.0f); // Spawn just below the bottom of the play screen

        // Check if the position is occupied
        positionOccupied = IsPositionOccupied(spawnPosition, 100.0f);
    }

    // If the position is not occupied, spawn the enemy
    if (!positionOccupied) {
        bool moveDownward = spawnPosition.y < Graphics::SCREEN_HEIGHT / 2;
        Enemy* enemy = new Enemy(moveDownward, vehicleIndex); // Move downward if spawned above the screen, otherwise move upward
        enemy->Position(spawnPosition);
        mEnemies.push_back(enemy);
    }
}

void EnemySpawner::Render() {
    // Render active enemies
    for (auto enemy : mEnemies) {
        enemy->Render();
    }
}
