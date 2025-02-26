#ifndef __ENEMYPOLICE_H
#define __ENEMYPOLICE_H

#include "Enemy.h"
#include "Player.h"
#include "EnemySpawner.h"
#include <vector>

class EnemyPolice : public Enemy {
private:
	Timer* mTimer;
	Player* mPlayer;
	float mChaseDuration;
	bool mChasing;
	float mBaseSpeed;
	bool mAvoiding;
	//std::vector<Enemy*> mOtherEnemies;
	EnemySpawner* mEnemySpawner;
	static EnemyPolice* sActivePoliceCar;
	static float sChaseDuration;

public:
	EnemyPolice(Player* player, EnemySpawner* enemySpawner);
	~EnemyPolice();

	void Update() override;
	void Render() override;
	void StartChase();
	void StopChase();
	void Destroy();
	static void SpawnNewPoliceCar(Player* player, EnemySpawner* enemySpawner);
	static EnemyPolice* GetActivePoliceCar() { return sActivePoliceCar; }

};

#endif
