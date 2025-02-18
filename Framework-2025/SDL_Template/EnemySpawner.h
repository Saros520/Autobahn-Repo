#ifndef __ENEMYSPAWNER_H
#define __ENEMYSPAWNER_H

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

	void Update();
	void Render();

};

#endif
