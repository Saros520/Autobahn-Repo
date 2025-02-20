#include "PlayScreen.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include <cstdlib>
#include <ctime>

const int vehicleIndex = 0;

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();
	mInput = InputManager::Instance();

	mHighWaySpeed = 550.0f; // Speed of the highway movement
	mCurrentEnvironment = 2;
	mNextEnvironment = 1;
	mEnvironmentChangeTimer = 0.0f;
	mTransitioning = false;
	mTransitionAlpha = 0.0f;
	mTransitionDuration = 1.0f; // Duration of the transition effect in seconds

	mSpeedBox = new Texture("SpeedBox.png");
	mSpeedBox->Parent(this);
	mSpeedBox->Position(Graphics::SCREEN_WIDTH * 1.21f, Graphics::SCREEN_HEIGHT * 0.53f);
	mSpeedBox->Scale(Vector2(1.0f, 2.0f));

	mLivesBox = new Texture("LivesBox.png");
	mLivesBox->Parent(this);
	mLivesBox->Position(Graphics::SCREEN_WIDTH * 0.35f, Graphics::SCREEN_HEIGHT * 0.53f);
	mLivesBox->Scale(Vector2(1.0f, 2.0f));

	// Initialize random seed for the highway backgrounds
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Load highway sprites for different environments
	for (int i = 0; i < NUM_ROAD_SPRITES; ++i) {
		mNorthRoadSprites[0].push_back(new Texture("NorthRoadCity" + std::to_string(i + 1) + ".png"));
		mSouthRoadSprites[0].push_back(new Texture("SouthRoadCity" + std::to_string(i + 1) + ".png"));

		mNorthRoadSprites[1].push_back(new Texture("NorthRoadBridge" + std::to_string(i + 1) + ".png"));
		mSouthRoadSprites[1].push_back(new Texture("SouthRoadBridge" + std::to_string(i + 1) + ".png"));

		mNorthRoadSprites[2].push_back(new Texture("NorthRoadForest" + std::to_string(i + 1) + ".png"));
		mSouthRoadSprites[2].push_back(new Texture("SouthRoadForest" + std::to_string(i + 1) + ".png"));

		mNorthRoadSprites[3].push_back(new Texture("NorthRoadPlain" + std::to_string(i + 1) + ".png"));
		mSouthRoadSprites[3].push_back(new Texture("SouthRoadPlain" + std::to_string(i + 1) + ".png"));
	}

	// Calculate the scale factor to cover the whole play screen
	float scaleX = static_cast<float>(Graphics::SCREEN_WIDTH) / mNorthRoadSprites[0][0]->ScaledDimensions().x;
	float scaleY = static_cast<float>(Graphics::SCREEN_HEIGHT) / mNorthRoadSprites[0][0]->ScaledDimensions().y;
	Vector2 scaleFactor = Vector2(3.36f, 3.45f);

	// Define separate scale factors for NorthRoadCity and SouthRoadCity
	Vector2 cityScaleFactor = Vector2(1.5f, 1.489f);

	for (int i = 0; i < NUM_ROAD_SPRITES; ++i) {
		mNorthRoadSprites[mCurrentEnvironment][i]->Parent(this);
		mSouthRoadSprites[mCurrentEnvironment][i]->Parent(this);

		mNorthRoadSprites[mCurrentEnvironment][i]->Position(Graphics::SCREEN_WIDTH * 0.782f, Graphics::SCREEN_HEIGHT * 0.5f + i * Graphics::SCREEN_HEIGHT);
		mSouthRoadSprites[mCurrentEnvironment][i]->Position(Graphics::SCREEN_WIDTH * 0.219f, Graphics::SCREEN_HEIGHT * 0.5f + i * Graphics::SCREEN_HEIGHT);

		if (mCurrentEnvironment == 0) {
			mNorthRoadSprites[mCurrentEnvironment][i]->Scale(cityScaleFactor);
			mSouthRoadSprites[mCurrentEnvironment][i]->Scale(cityScaleFactor);
		}
		else {
			mNorthRoadSprites[mCurrentEnvironment][i]->Scale(scaleFactor);
			mSouthRoadSprites[mCurrentEnvironment][i]->Scale(scaleFactor);
		}

		mHighwayPosY[i] = mNorthRoadSprites[mCurrentEnvironment][i]->Position().y;
	}

	// top bar entities
	mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, 80.0f);
	mPlayerScore = new Texture("High-Score", "emulogic.ttf", 32, { 200, 0, 0 });
	mPlayerScoreNumber = new Scoreboard();

	mTopBar->Parent(this);
	mPlayerScore->Parent(mTopBar);
	mPlayerScoreNumber->Parent(mTopBar);

	mPlayerScore->Position(Graphics::SCREEN_WIDTH * -0.12f, -63.0f);
	mPlayerScoreNumber->Position(Graphics::SCREEN_WIDTH * 0.35f, -63.0f);

	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Position(Graphics::SCREEN_WIDTH * 0.642f, Graphics::SCREEN_HEIGHT * 0.9f); // offset to fit lanes centered

	mEnemySpawner = new EnemySpawner(4.0f, vehicleIndex); // Spawn an enemy every 4 seconds

	/*mLevelTime = 0.0f;
	mLevelTimeText = new Scoreboard();
	mLevelTimeText->Parent(this);
	mLevelTimeText->Position(Graphics::SCREEN_WIDTH * 0.5f, 32.0f);*/

	/*mLevelTimeText->Score(mLevelTime);*/

	mPauseGame = new PauseGame();
	mPauseGame->Parent(this);
	mIsPaused = false;

	// bottom bar entities
	mBottomBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.83f, Graphics::SCREEN_HEIGHT - 163.0f);
	mSpeedLabel = new Texture("Speed", "emulogic.ttf", 20, { 255, 0, 0 });
	mSpeedScoreboard = new Scoreboard();

	mLivesLabel = new Texture("Lives", "emulogic.ttf", 20, { 255, 0, 0 });

	mBottomBar->Parent(this);
	mSpeedLabel->Parent(mBottomBar);
	mLivesLabel->Parent(mBottomBar);
	mSpeedScoreboard->Parent(mBottomBar);

	mSpeedLabel->Position(Graphics::SCREEN_WIDTH * 0.10f, 30.0f);
	mSpeedScoreboard->Position(Graphics::SCREEN_WIDTH * 0.17f, 86.5f);
	mLivesLabel->Position(Graphics::SCREEN_WIDTH * -0.7579f, 30.0f);

}

Player* PlayScreen::GetPlayer() {
	return mPlayer;
}

void PlayScreen::ResetPauseState() {
	mIsPaused = false;
}

PlayScreen::~PlayScreen() {
	mTimer = nullptr;
	mAudio = nullptr;

	for (int env = 0; env < NUM_ENVIRONMENTS; ++env) {
		for (int i = 0; i < NUM_ROAD_SPRITES; ++i) {
			delete mNorthRoadSprites[env][i];
			delete mSouthRoadSprites[env][i];
		}
	}

	delete mSpeedBox;
	mSpeedBox = nullptr;

	delete mLivesBox;
	mLivesBox = nullptr;

	delete mPlayer;
	mPlayer = nullptr;

	delete mEnemySpawner;
	mEnemySpawner = nullptr;

	/*delete mLevelTimeText;
	mLevelTimeText = nullptr;*/

	delete mPlayerScore;
	mPlayerScore = nullptr;

	delete mPlayerScoreNumber;
	mPlayerScoreNumber = nullptr;

	delete mTopBar;
	mTopBar = nullptr;

	delete mSpeedLabel;
	mSpeedLabel = nullptr;

	delete mSpeedScoreboard;
	mSpeedScoreboard = nullptr;

	delete mLivesLabel;
	mLivesLabel = nullptr;

	delete mBottomBar;
	mBottomBar = nullptr;

}

void PlayScreen::UpdateHighway() {
	for (int i = 0; i < NUM_ROAD_SPRITES; ++i) {
		mHighwayPosY[i] += mHighWaySpeed * mTimer->DeltaTime();
		if (mHighwayPosY[i] >= Graphics::SCREEN_HEIGHT * 1.5f) {
			mHighwayPosY[i] -= Graphics::SCREEN_HEIGHT * NUM_ROAD_SPRITES;
		}
		mNorthRoadSprites[mCurrentEnvironment][i]->Position(Graphics::SCREEN_WIDTH * 0.782f, mHighwayPosY[i]);
		mSouthRoadSprites[mCurrentEnvironment][i]->Position(Graphics::SCREEN_WIDTH * 0.219f, mHighwayPosY[i]);
	}
}

void PlayScreen::StartEnvironmentTransition() {
	mNextEnvironment = std::rand() % NUM_ENVIRONMENTS;
	mTransitioning = true;
	mTransitionAlpha = 0.0f;

	// Calculate the scale factor to cover the whole play screen
	float scaleX = static_cast<float>(Graphics::SCREEN_WIDTH) / mNorthRoadSprites[mNextEnvironment][0]->ScaledDimensions().x;
	float scaleY = static_cast<float>(Graphics::SCREEN_HEIGHT) / mNorthRoadSprites[mNextEnvironment][0]->ScaledDimensions().y;
	Vector2 scaleFactor = Vector2(3.36f, 3.45f);

	// Define separate scale factors for NorthRoadCity and SouthRoadCity
	Vector2 cityScaleFactor = Vector2(1.5f, 1.489f);

	for (int i = 0; i < NUM_ROAD_SPRITES; ++i) {
		mNorthRoadSprites[mNextEnvironment][i]->Parent(this);
		mSouthRoadSprites[mNextEnvironment][i]->Parent(this);

		mNorthRoadSprites[mNextEnvironment][i]->Position(Graphics::SCREEN_WIDTH * 0.782f, mHighwayPosY[i]);
		mSouthRoadSprites[mNextEnvironment][i]->Position(Graphics::SCREEN_WIDTH * 0.219f, mHighwayPosY[i]);

		if (mNextEnvironment == 0) {
			mNorthRoadSprites[mNextEnvironment][i]->Scale(cityScaleFactor);
			mSouthRoadSprites[mNextEnvironment][i]->Scale(cityScaleFactor);
		}
		else {
			mNorthRoadSprites[mNextEnvironment][i]->Scale(scaleFactor);
			mSouthRoadSprites[mNextEnvironment][i]->Scale(scaleFactor);
		}
	}
}

void PlayScreen::UpdateEnvironmentTransition() {
	mTransitionAlpha += mTimer->DeltaTime() / mTransitionDuration;
	if (mTransitionAlpha >= 1.0f) {
		mTransitionAlpha = 1.0f;
		mCurrentEnvironment = mNextEnvironment;
		mTransitioning = false;
	}

	for (int i = 0; i < NUM_ROAD_SPRITES; ++i) {
		mNorthRoadSprites[mCurrentEnvironment][i]->Alpha(255 * (1.0f - mTransitionAlpha));
		mSouthRoadSprites[mCurrentEnvironment][i]->Alpha(255 * (1.0f - mTransitionAlpha));
		mNorthRoadSprites[mNextEnvironment][i]->Alpha(255 * mTransitionAlpha);
		mSouthRoadSprites[mNextEnvironment][i]->Alpha(255 * mTransitionAlpha);
	}
}


void PlayScreen::Update() {

	if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
		mIsPaused = !mIsPaused;
	}

	if (mIsPaused) {
		mPauseGame->Update();

		// Check if player selected an option
		if (mPauseGame->SelectedOption() == 0 && mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
			// Resume game
			mIsPaused = false;
		}
		else if (mPauseGame->SelectedOption() == 1 && mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
			// Exit to main menu
			ScreenManager::Instance()->SetScreen(ScreenManager::Start);
		}
	}
	else {
		mLevelTime += mTimer->DeltaTime();
		mEnvironmentChangeTimer += mTimer->DeltaTime();

		if (mEnvironmentChangeTimer >= 40.0f && !mTransitioning) {
			StartEnvironmentTransition();
			mEnvironmentChangeTimer = 0.0f;
		}

		if (mTransitioning) {
			UpdateEnvironmentTransition();
		}

		UpdateHighway();
		mPlayer->Update();
		mEnemySpawner->Update();

		// Switch music based on key press (1-9)
		for (int i = 0; i < 9; ++i) {
			if (mInput->KeyPressed(static_cast<SDL_Scancode>(SDL_SCANCODE_1 + i))) {
				if (mCurrentSongIndex != i) {
					mCurrentSongIndex = i;
					void PauseMusic();  // Stop the current music
					mAudio->PlayMusic(mSongs[mCurrentSongIndex].c_str(), -1);  // Play the new song
				}
				break;  // Only process the first matching key
			}
		}

		// Pause or resume music when the 0 key is pressed
		if (mInput->KeyPressed(SDL_SCANCODE_0)) {
			static bool musicPaused = false;
			if (musicPaused) {
				mAudio->ResumeMusic();  // Resume the music
			}
			else {
				mAudio->PauseMusic();  // Pause the music
			}
			musicPaused = !musicPaused;  // Toggle the pause state
		}

		/*mLevelTimeText->Score(mLevelTime);
		mLevelTimeText->Update();*/
		mPlayerScoreNumber->Score(mPlayer->Score());
		mPlayerScoreNumber->Update();

		int playerLives = mPlayer->Lives();
		mSpeedScoreboard->Score(playerLives); // Display remaining lives
		mSpeedScoreboard->Update();

		mPlayerScoreNumber->Distance(mPlayer->DistanceTraveled());
		mPlayerScoreNumber->Update();

		float speed = mPlayer->GetSpeed(); 
		mSpeedScoreboard->Score(static_cast<int>(speed));
		mSpeedScoreboard->Update();
	}
}


void PlayScreen::Render() {
	for (int i = 0; i < NUM_ROAD_SPRITES; ++i) {
		mNorthRoadSprites[mCurrentEnvironment][i]->Render();
		mSouthRoadSprites[mCurrentEnvironment][i]->Render();
		if (mTransitioning) {
			mNorthRoadSprites[mNextEnvironment][i]->Render();
			mSouthRoadSprites[mNextEnvironment][i]->Render();
		}
	}
	mSpeedBox->Render();
	mLivesBox->Render();
	mPlayer->Render();
	mEnemySpawner->Render();
	/*mLevelTimeText->Render();*/
	mPlayerScore->Render();
	mPlayerScoreNumber->Render();
	mSpeedLabel->Render();
	mSpeedScoreboard->Render();
	mLivesLabel->Render();
	
	if (mIsPaused) {
		mPauseGame->Render();
	}
}
