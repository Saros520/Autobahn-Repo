#ifndef __PLAYSCREEN_H
#define __PLAYSCREEN_H
#include "Player.h"
#include "Scoreboard.h"
#include "Enemy.h"

class PlayScreen : public GameEntity {
private:
	Timer * mTimer;
	AudioManager * mAudio;

	Texture* mNorthRoadCity;
	Texture* mSouthRoadCity;

	Player * mPlayer;
	Enemy* mEnemy;

	// Top Bar Entities
	GameEntity* mTopBar;
	float mLevelTime;
	Scoreboard * mLevelTimeText;

	Texture* mPlayerScore;
	Scoreboard* mPlayerScoreNumber;

public:
	PlayScreen();
	~PlayScreen();

	void Update() override;
	void Render() override;

	Player* GetPlayer();
};
#endif