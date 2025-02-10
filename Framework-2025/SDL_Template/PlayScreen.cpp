#include "PlayScreen.h"
#include "ScreenManager.h"

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mHighWaySpeed = 600.0f; // Speed of the highway movement

	for (int i = 0; i < NUM_ROAD_CITY; ++i) {
		mNorthRoadCity[i] = new Texture("NorthRoadCity" + std::to_string(i + 1) + ".png");
		mSouthRoadCity[i] = new Texture("SouthRoadCity" + std::to_string(i + 1) + ".png");

		mNorthRoadCity[i]->Parent(this);
		mSouthRoadCity[i]->Parent(this);

		mNorthRoadCity[i]->Position(Graphics::SCREEN_WIDTH * 0.782f, Graphics::SCREEN_HEIGHT * 0.5f + i * Graphics::SCREEN_HEIGHT);
		mSouthRoadCity[i]->Position(Graphics::SCREEN_WIDTH * 0.219f, Graphics::SCREEN_HEIGHT * 0.5f + i * Graphics::SCREEN_HEIGHT);

		mNorthRoadCity[i]->Scale(Vector2(1.5f, 1.489f));
		mSouthRoadCity[i]->Scale(Vector2(1.5f, 1.489f));

		mHighwayPosY[i] = mNorthRoadCity[i]->Position().y;
	}

	// top bar entities
	mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, 80.0f);
	mPlayerScore = new Texture("High-Score", "emulogic.ttf", 32, { 200, 0, 0 });
	mPlayerScoreNumber = new Scoreboard();

	mTopBar->Parent(this);
	mPlayerScore->Parent(mTopBar);

	mPlayerScore->Position(Graphics::SCREEN_WIDTH * -0.12f, -10.0f);

	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Position(Graphics::SCREEN_WIDTH * 0.642f, Graphics::SCREEN_HEIGHT * 0.9f); // offset to fit lanes centered

	mEnemy = new Enemy();
	mEnemy->Parent(this);
	mEnemy->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mLevelTime = 0.0f;
	mLevelTimeText = new Scoreboard();
	mLevelTimeText->Parent(this);
	mLevelTimeText->Position(Graphics::SCREEN_WIDTH * 0.5f, 32.0f);

	mLevelTimeText->Score(mLevelTime);
}

Player* PlayScreen::GetPlayer() {
	return mPlayer;
}

PlayScreen::~PlayScreen() {
	mTimer = nullptr;
	mAudio = nullptr;

	for (int i = 0; i < NUM_ROAD_CITY; ++i) {
		delete mNorthRoadCity[i];
		delete mSouthRoadCity[i];
		mNorthRoadCity[i] = nullptr;
		mSouthRoadCity[i] = nullptr;
	}

	delete mPlayer;
	mPlayer = nullptr;

	delete mEnemy;
	mEnemy = nullptr;

	delete mLevelTimeText;
	mLevelTimeText = nullptr;

	delete mPlayerScore;
	mPlayerScore = nullptr;

	delete mPlayerScoreNumber;
	mPlayerScoreNumber = nullptr;

	delete mTopBar;
	mTopBar = nullptr;
}

void PlayScreen::UpdateHighway() {
	for (int i = 0; i < NUM_ROAD_CITY; ++i) {
		mHighwayPosY[i] += mHighWaySpeed * mTimer->DeltaTime();
		if (mHighwayPosY[i] >= Graphics::SCREEN_HEIGHT * 1.5f) {
			mHighwayPosY[i] -= Graphics::SCREEN_HEIGHT * NUM_ROAD_CITY;
		}
		mNorthRoadCity[i]->Position(Graphics::SCREEN_WIDTH * 0.782f, mHighwayPosY[i]);
		mSouthRoadCity[i]->Position(Graphics::SCREEN_WIDTH * 0.219f, mHighwayPosY[i]);
	}
}

void PlayScreen::Update() {
	mLevelTime += mTimer->DeltaTime();
	UpdateHighway();
	mPlayer->Update();
	mEnemy->Update();
	mLevelTimeText->Score(mLevelTime);
	mLevelTimeText->Update();
	mPlayerScoreNumber->Score(mPlayer->Score());
	mPlayerScoreNumber->Update();
}

void PlayScreen::Render() {
	for (int i = 0; i < NUM_ROAD_CITY; ++i) {
		mNorthRoadCity[i]->Render();
		mSouthRoadCity[i]->Render();
	}

	mPlayer->Render();
	mEnemy->Render();
	mLevelTimeText->Render();
	mPlayerScore->Render();
	mPlayerScoreNumber->Render();
	
}
