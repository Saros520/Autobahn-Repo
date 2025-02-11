#ifndef __PLAYSCREEN_H
#define __PLAYSCREEN_H

#include "GameEntity.h"
#include "Player.h"
#include "Scoreboard.h"
#include "Enemy.h"
#include "PauseGame.h"

class PlayScreen : public GameEntity {
private:
	Timer * mTimer;
	AudioManager * mAudio;
	InputManager* mInput;

	static const int NUM_ROAD_CITY = 5;
	Texture* mNorthRoadCity[NUM_ROAD_CITY];
	Texture* mSouthRoadCity[NUM_ROAD_CITY];
	float mHighWaySpeed;
	float mHighwayPosY[NUM_ROAD_CITY];

	// Top Bar Entities
	GameEntity* mTopBar;
	Texture* mPlayerScore;
	Scoreboard* mPlayerScoreNumber;

	Player * mPlayer;
	Enemy* mEnemy;

	float mLevelTime;
	Scoreboard* mLevelTimeText;

	void UpdateHighway();
	void UpdatePlayer();
	void UpdateEnemy();

	PauseGame* mPauseGame;
	bool mIsPaused;

public:
	PlayScreen();
	~PlayScreen();

	void Update() override;
	void Render() override;

	Player* GetPlayer();
	
	void ResetPauseState();
};
#endif