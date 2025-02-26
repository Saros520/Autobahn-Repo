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

    bool CheckRayIntersection(const Vector2& rayStart, const Vector2& rayEnd, const Vector2& enemyPos, const Vector2& enemySize);


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
