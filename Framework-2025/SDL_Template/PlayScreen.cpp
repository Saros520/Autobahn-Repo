#include "PlayScreen.h"
#include "ScreenManager.h"

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mNorthRoadCity = new Texture("NorthRoadCity.png");
	mSouthRoadCity = new Texture("SouthRoadCity.png");

	mNorthRoadCity->Parent(this);
	mSouthRoadCity->Parent(this);

	mNorthRoadCity->Position(Graphics::SCREEN_WIDTH * 0.782f, Graphics::SCREEN_HEIGHT * 0.5f);
	mSouthRoadCity->Position(Graphics::SCREEN_WIDTH * 0.219f, Graphics::SCREEN_HEIGHT * 0.5f);

	mNorthRoadCity->Scale(Vector2(1.5f, 1.489f));
	mSouthRoadCity->Scale(Vector2(1.5f, 1.489f));

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
	delete mNorthRoadCity;
	delete mSouthRoadCity;
	mNorthRoadCity = nullptr;
	mSouthRoadCity = nullptr;
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

void PlayScreen::Update() {
	mLevelTime += mTimer->DeltaTime();
	mPlayer->Update();
	mEnemy->Update();
	mLevelTimeText->Score(mLevelTime);
	mLevelTimeText->Update();
	mPlayerScoreNumber->Score(mPlayer->Score());
	mPlayerScoreNumber->Update();
}

void PlayScreen::Render() {
	mNorthRoadCity->Render();
	mSouthRoadCity->Render();

	mPlayer->Render();
	mEnemy->Render();
	mLevelTimeText->Render();
	mPlayerScore->Render();
	mPlayerScoreNumber->Render();
	
}
