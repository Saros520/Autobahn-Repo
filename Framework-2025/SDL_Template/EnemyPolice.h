#ifndef __ENEMYPOLICE_H
#define __ENEMYPOLICE_H

#include "Enemy.h"
#include "Player.h"
#include "EnemySpawner.h"
#include "SpikeStrip.h"
#include <vector>


class EnemyPolice : public Enemy {
private:
    static EnemyPolice* sActivePoliceCar;
    static float sChaseDuration;

    Timer* mTimer;
    Player* mPlayer;
    EnemySpawner* mEnemySpawner;
    PlayScreen* mPlayScreen;
    bool mAvoiding;
    float mBaseSpeed;
    bool mChasing;
    bool mDestroyed;
    bool mIsTopPoliceCar;
    float mSpikeStripTimer;
    float mSpikeStripInterval;

public:
    EnemyPolice(Player* player, EnemySpawner* enemySpawner, bool isTopPoliceCar = false);
    ~EnemyPolice();

    void Update() override;
    void Render() override;
    void StartChase();
    void StopChase();
    void Destroy();
    void LaySpikeStrip();
    static EnemyPolice* GetActivePoliceCar() { return sActivePoliceCar; }
    bool IsDestroyed() const { return mDestroyed; }

};

#endif
