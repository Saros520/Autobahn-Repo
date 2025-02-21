#ifndef __ENEMYPOLICE_H
#define __ENEMYPOLICE_H

#include "Enemy.h"
#include "Player.h"
#include <vector>

class EnemyPolice : public Enemy {
private:
	Timer* mTimer;
	Player* mPlayer;
	float mChaseDuration;
	bool mChasing;
	float mBaseSpeed;
	bool mAvoiding;
	std::vector<Enemy*> mOtherEnemies;
	static EnemyPolice* sActivePoliceCar;

public:
	EnemyPolice(Player* player, const std::vector<Enemy*>& otherEnemies);
	~EnemyPolice();

	void Update() override;
	void Render() override;
	void StartChase();
	void StopChase();
	void Destroy();
	static void SpawnNewPoliceCar(Player* player, const std::vector<Enemy*>& otherEnemies);
	static EnemyPolice* GetActivePoliceCar() { return sActivePoliceCar; }

};

#endif
