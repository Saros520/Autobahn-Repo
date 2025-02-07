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

	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.9f);

	//int selectedCar = ScreenManager::Instance()->GetSelectedCar();
	//mPlayer->SetCarTexture(selectedCar);

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
}

void PlayScreen::Update() {
	mLevelTime += mTimer->DeltaTime();
	mPlayer->Update();
	mEnemy->Update();
	mLevelTimeText->Score(mLevelTime);
}

void PlayScreen::Render() {
	mNorthRoadCity->Render();
	mSouthRoadCity->Render();

	mPlayer->Render();
	mEnemy->Render();
	mLevelTimeText->Render();
}
