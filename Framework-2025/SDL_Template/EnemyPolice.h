#ifndef __ENEMYPOLICE_H
#define __ENEMYPOLICE_H

#include "Enemy.h"
#include "Player.h"

class EnemyPolice : public Enemy {
private:
	Timer* mTimer;
	Player* mPlayer;
	float mChaseDuration;
	bool mChasing;
	float mBaseSpeed;
	std::vector<Enemy*> mOtherEnemies;
	bool mAvoiding;

public:
	EnemyPolice(Player* player, const std::vector<Enemy*>& otherEnemies);
	~EnemyPolice();

	void Update() override;
	void Render() override;
	void StartChase();
	void StopChase();

};

#endif
