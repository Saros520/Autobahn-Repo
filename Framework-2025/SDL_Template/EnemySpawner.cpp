#include "EnemySpawner.h"
#include "Graphics.h"
#include <cstdlib>
#include <ctime>
//#include <iostream> For debug output


EnemySpawner::EnemySpawner(float spawnInterval, int vehicleIndex) {
    mTimer = Timer::Instance();
    mSpawnInterval = 1.0f; // Spawns a vehicle every 1 seconds
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
    // std::cout << "Number of Enemies in EnemySpawner: " << mEnemies.size() << std::endl;

    mTimeSinceLastSpawn += mTimer->DeltaTime();

    if (mTimeSinceLastSpawn >= mSpawnInterval) {
        SpawnEnemy(); // Spawn enemies in both lanes
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
            //std::cout << "Removing enemy at position: " << pos.x << ", " << pos.y << std::endl; // Debug output
            delete enemy;
        }
        return offScreen;
        }), mEnemies.end());
}

const std::vector<Enemy*>& EnemySpawner::GetEnemies() const {
    return mEnemies; // Return the list of currently spawned enemies
}

bool EnemySpawner::IsPositionOccupied(Vector2 position, float buffer) {
    for (auto enemy : mEnemies) {
        Vector2 enemyPos = enemy->Position();
        if (abs(enemyPos.y - position.y) < buffer) {
            //std::cout << "Position occupied by enemy at: " << enemyPos.x << ", " << enemyPos.y << std::endl; // Debug output
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

    // Function to spawn an enemy in a given lane
    auto spawnInLane = [&](const std::vector<float>& lanes, bool moveDownward, bool flipTexture, float speedMultiplier) {
        
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

        // Choose a random lane
        int laneIndex = std::rand() % lanes.size();
        float laneX = lanes[laneIndex];

        // Set spawn position above the screen
        Vector2 spawnPosition = Vector2(laneX, -100.0f);

        // Check if the position is occupied
        bool positionOccupied = IsPositionOccupied(spawnPosition, 0.0f); // Adjust buffer as needed

        // If the position is not occupied, spawn the enemy
        if (!positionOccupied) {
            Enemy* enemy = new Enemy(moveDownward, vehicleIndex); // Move downward
            enemy->Position(spawnPosition);
            enemy->SetSpeedMultiplier(speedMultiplier); // Set speed multiplier

            if (flipTexture) {
                enemy->Rotation(180.0f); // Flip vehicle textures on the left 180 degrees
            }

            mEnemies.push_back(enemy);
            //std::cout << "Spawned enemy at laneX: " << laneX << " with speedMultiplier: " << speedMultiplier << std::endl; // Debug output
        }
        else {
            //std::cout << "Position occupied at laneX: " << laneX << std::endl; // Debug output
        }
    };

    // Spawn enemies in both left and right lanes
    spawnInLane(leftLanes, true, true, 2.3f);  // Spawn enemy in the left lane with faster speed
    spawnInLane(rightLanes, true, false, 1.0f); // Spawn enemy in the right lane with normal speed
}

void EnemySpawner::Render() {
    // Render active enemies
    for (auto enemy : mEnemies) {
        enemy->Render();
    }
}

void EnemySpawner::Reset() {
    for (Enemy* enemy : mEnemies) {
        delete enemy;
    }
    mEnemies.clear();
    mSpawnTimer = 0.0f;
}
