#ifndef __PLAYSCREEN_H
#define __PLAYSCREEN_H
#include "Player.h"
#include "Scoreboard.h"
#include "Enemy.h"

class PlayScreen : public GameEntity {
private:
	Timer * mTimer;
	AudioManager * mAudio;

	Player * mPlayer;
	Enemy* mEnemy;

	float mLevelTime;
	Scoreboard * mLevelTimeText;

public:
	PlayScreen();
	~PlayScreen();

	void Update() override;
	void Render() override;
};
#endif