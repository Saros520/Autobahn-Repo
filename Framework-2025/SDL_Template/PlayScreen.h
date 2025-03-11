#ifndef __PLAYSCREEN_H
#define __PLAYSCREEN_H

#include "GameEntity.h"
#include "Player.h"
#include "Scoreboard.h"
#include "EnemySpawner.h"
#include "EnemyPolice.h"
#include "PauseGame.h"
#include "Texture.h"
#include "SpikeStrip.h"
#include <vector>

class PlayScreen : public GameEntity {
private:

	Timer * mTimer;
	AudioManager * mAudio;
	InputManager* mInput;
	
	float mScreenBoundsY = 1100;

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

	const std::vector<std::string> mSongs = {
		"TokyoDrift.mp3", // Key 1
		"GoodOldBoys.mp3", // Key 2
		"EastBoundAndDown.mp3", // Key 3
		"KickstartMyHeart.mp3", // Key 4
		"Panama.mp3", // Key 5
		"LaGrange.mp3", // Key 6
		"Rollin.mp3", // Key 7
		"Ridin.mp3", // Key 8
		"GetLow.mp3"  // Key 9
	};
	int mCurrentSongIndex = -1;

	// Top Bar Entities
	GameEntity* mTopBar;
	Texture* mPlayerScore;
	Scoreboard* mPlayerScoreNumber;
	Scoreboard* mHighScoreNumber;

	Player * mPlayer;
	EnemySpawner* mEnemySpawner;
	EnemyPolice* mEnemyPolice;
	bool mPoliceChaseActive;
	float mPoliceChaseTimer;

	EnemyPolice* mTopPoliceCar;
	bool mTopPoliceChaseActive;
	float mTopPoliceChaseTimer;

	std::vector<SpikeStrip*> mSpikeStrips;

	PauseGame* mPauseGame;

	float mLevelTime;
	/*Scoreboard* mLevelTimeText;*/

	// Bottom Bar Entities
	GameEntity* mBottomBar;
	Texture* mSpeedLabel;
	Scoreboard* mSpeedScoreboard;
	Texture* mLivesLabel;
	Texture* mSpeedBox;
	Texture* mLivesBox;
	Scoreboard* mLivesScoreboard;

	void UpdateHighway();
	void StartEnvironmentTransition();
    void UpdateEnvironmentTransition();
	void StartPoliceChase();
	void EndPoliceChase();
	void StartTopPoliceChase();
	void EndTopPoliceChase();
	void UpdateTopPoliceCar();
	void UpdatePlayer();
	void UpdateEnemy();

	bool mIsPaused;

	// Variables to track the current background state 
	std::string mCurrentNorthRoadType;
	int mCurrentNorthRoadIndex;
	std::string mCurrentSouthRoadType;
	int mCurrentSouthRoadIndex;

public:
	PlayScreen();
	~PlayScreen();

	void Update() override;
	void Render() override;

	Player* GetPlayer();
	
	void ResetPauseState();

	void Reset();

	// Methods to set and get the current background state
	void SetCurrentBackground(std::string northRoadType, int northRoadIndex, std::string southRoadType, int southRoadIndex);
	std::string GetCurrentNorthRoadType() const;
	int GetCurrentNorthRoadIndex() const;
	std::string GetCurrentSouthRoadType() const;
	int GetCurrentSouthRoadIndex() const;

	void AddSpikeStrip(SpikeStrip* spikeStrip);

	// Method to handle game over event
	void OnGameOver();

	// Method to remove off-screen spike strips
	void RemoveOffScreenSpikeStrips();

};
#endif
