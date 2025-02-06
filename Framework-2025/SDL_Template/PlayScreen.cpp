#include "PlayScreen.h"
#include "ScreenManager.h"

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.9f);

	int selectedCar = ScreenManager::Instance()->GetSelectedCar();
	mPlayer->SetCarTexture(selectedCar);

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
	mPlayer->Render();
	mEnemy->Render();
	mLevelTimeText->Render();
}
