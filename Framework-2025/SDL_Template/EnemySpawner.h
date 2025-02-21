#ifndef __ENEMYSPAWNER_H
#define __ENEMYSPAWNER_H

#include "Timer.h"
#include "Enemy.h"
#include <vector>

class EnemySpawner {
private:
	Timer* mTimer;
	float mSpawnInterval;
	float mTimeSinceLastSpawn;
	std::vector<Enemy*> mEnemies;
	int mVehicleIndex; 

public:
	EnemySpawner(float spawnInteval, int vehicleIndex);
	~EnemySpawner();

	void SpawnEnemy();
	bool IsPositionOccupied(Vector2 position, float buffer);

	void Update();
	void Render();

	const std::vector<Enemy*>& GetEnemies() const;

};

#endif
