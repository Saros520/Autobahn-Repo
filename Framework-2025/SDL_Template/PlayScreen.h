#ifndef __PLAYSCREEN_H
#define __PLAYSCREEN_H

#include "GameEntity.h"
#include "Player.h"
#include "Scoreboard.h"
#include "Enemy.h"
#include "PauseGame.h"
#include <vector>

class PlayScreen : public GameEntity {
private:
	Timer * mTimer;
	AudioManager * mAudio;
	InputManager* mInput;

	static const int NUM_ROAD_SPRITES = 5;
	static const int NUM_ENVIRONMENTS = 4;
	std::vector<Texture*> mNorthRoadSprites[NUM_ENVIRONMENTS];
	std::vector<Texture*> mSouthRoadSprites[NUM_ENVIRONMENTS];
	float mHighWaySpeed;
	float mHighwayPosY[NUM_ROAD_SPRITES];

	int mCurrentEnvironment;
	int mNextEnvironment;
	float mEnvironmentChangeTimer;
	bool mTransitioning;
	float mTransitionAlpha;
	float mTransitionDuration;

	// Top Bar Entities
	GameEntity* mTopBar;
	Texture* mPlayerScore;
	Scoreboard* mPlayerScoreNumber;

	Player * mPlayer;
	Enemy* mEnemy;

	float mLevelTime;
	Scoreboard* mLevelTimeText;

	void UpdateHighway();
	void StartEnvironmentTransition();
    void UpdateEnvironmentTransition();
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