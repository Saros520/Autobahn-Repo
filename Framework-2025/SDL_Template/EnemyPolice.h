#ifndef __ENEMYPOLICE_H
#define __ENEMYPOLICE_H

#include "Enemy.h"
#include "Player.h"
#include "EnemySpawner.h"
#include <vector>

class EnemyPolice : public Enemy {
private:
    static EnemyPolice* sActivePoliceCar;
    static float sChaseDuration;

    Timer* mTimer;
    Player* mPlayer;
    EnemySpawner* mEnemySpawner;
    bool mAvoiding;
    float mBaseSpeed;
    bool mChasing;
    bool mDestroyed;

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
    bool IsDestroyed() const { return mDestroyed; }

};

#endif
