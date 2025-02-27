#include "PlayScreen.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <iostream>

bool FileExists(const std::string& filePath) {
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}

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
    mSpeedBox->Position(Graphics::SCREEN_WIDTH * 1.21f, Graphics::SCREEN_HEIGHT * 0.52f);
    mSpeedBox->Scale(Vector2(1.0f, 2.0f));

    mLivesBox = new Texture("LivesBox.png");
    mLivesBox->Parent(this);
    mLivesBox->Position(Graphics::SCREEN_WIDTH * 0.35f, Graphics::SCREEN_HEIGHT * 0.52f);
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

    mEnemyPolice = nullptr;
    mPoliceChaseActive = false;
    mPoliceChaseTimer = 0.0f;

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
    mSpeedScoreboard->Position(Graphics::SCREEN_WIDTH * 0.169f, 86.0f);
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

    if (EnemyPolice::GetActivePoliceCar() != nullptr) {
        delete EnemyPolice::GetActivePoliceCar();
    }

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

    // Set the current background state
    SetCurrentBackground(
        mNextEnvironment == 0 ? "City" : mNextEnvironment == 1 ? "Bridge" : mNextEnvironment == 2 ? "Forest" : "Plain",
        1,
        mNextEnvironment == 0 ? "City" : mNextEnvironment == 1 ? "Bridge" : mNextEnvironment == 2 ? "Forest" : "Plain",
        1
    );

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

void PlayScreen::StartPoliceChase() {
    if (!mPoliceChaseActive && EnemyPolice::GetActivePoliceCar() == nullptr) {
        EnemyPolice::SpawnNewPoliceCar(mPlayer, mEnemySpawner);
        mPoliceChaseActive = true;
        mPoliceChaseTimer = 0.0f;
    }
}

void PlayScreen::EndPoliceChase() {
    if (mPoliceChaseActive) {
        if (EnemyPolice::GetActivePoliceCar() != nullptr) {
            EnemyPolice::GetActivePoliceCar()->Destroy();
            mEnemyPolice = nullptr; // Set the pointer to nullptr after deletion
        }
        mPoliceChaseActive = false;
    }
}

void PlayScreen::SetCurrentBackground(std::string northRoadType, int northRoadIndex, std::string southRoadType, int southRoadIndex) {
    mCurrentNorthRoadType = northRoadType;
    mCurrentNorthRoadIndex = northRoadIndex;
    mCurrentSouthRoadType = southRoadType;
    mCurrentSouthRoadIndex = southRoadIndex;
}

std::string PlayScreen::GetCurrentNorthRoadType() const {
    return mCurrentNorthRoadType;
}

int PlayScreen::GetCurrentNorthRoadIndex() const {
    return mCurrentNorthRoadIndex;
}

std::string PlayScreen::GetCurrentSouthRoadType() const {
    return mCurrentSouthRoadType;
}

int PlayScreen::GetCurrentSouthRoadIndex() const {
    return mCurrentSouthRoadIndex;
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

        // Start the police chase after about 2 minutes
        if (mLevelTime >= 2.0f && !mPoliceChaseActive) {
            StartPoliceChase();
        }

        // Update police chase 
        if (mPoliceChaseActive) {
            mPoliceChaseTimer += mTimer->DeltaTime();
            if (EnemyPolice::GetActivePoliceCar() != nullptr) {
                EnemyPolice::GetActivePoliceCar()->Update();
            }

            // End the police chase/player gets away for now after 1 minute
            if (mPoliceChaseTimer >= 60.0f) {
                EndPoliceChase();
            }
        }

        // Check for game over conditions
        if (mPlayer->IsOutOfLives() || mPlayer->WasHitByPolice()) {
            std::string northRoadSprite, southRoadSprite;
            switch (mCurrentEnvironment) {
            case 0:
                northRoadSprite = "NorthRoadCity1.png";
                southRoadSprite = "SouthRoadCity1.png";
                break;
            case 1:
                northRoadSprite = "NorthRoadBridge1.png";
                southRoadSprite = "SouthRoadBridge1.png";
                break;
            case 2:
                northRoadSprite = "NorthRoadForest1.png";
                southRoadSprite = "SouthRoadForest1.png";
                break;
            case 3:
                northRoadSprite = "NorthRoadPlain1.png";
                southRoadSprite = "SouthRoadPlain1.png";
                break;
            default:
                northRoadSprite = "NorthRoadCity1.png";
                southRoadSprite = "SouthRoadCity1.png";
                break;
            }
            ScreenManager::Instance()->SetScreen(ScreenManager::GameOver);
            ScreenManager::Instance()->SetGameOverBackground(northRoadSprite, southRoadSprite);
            OnGameOver();
            return; // Exit the update loop to prevent further updates
        }

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
	if (mPoliceChaseActive) {
		if (EnemyPolice::GetActivePoliceCar() != nullptr) {
			EnemyPolice::GetActivePoliceCar()->Render();
		}
	}

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

void PlayScreen::OnGameOver() {
	std::string currentNorthRoadType = GetCurrentNorthRoadType();
	int currentNorthRoadIndex = GetCurrentNorthRoadIndex();
	std::string currentSouthRoadType = GetCurrentSouthRoadType();
	int currentSouthRoadIndex = GetCurrentSouthRoadIndex();

	ScreenManager::Instance()->SetScreen(ScreenManager::GameOver);
}

void PlayScreen::Reset() {
	mHighWaySpeed = 550.0f; // Reset highway speed
	mCurrentEnvironment = 2;
	mNextEnvironment = 1;
	mEnvironmentChangeTimer = 0.0f;
	mTransitioning = false;
	mTransitionAlpha = 0.0f;

	for (int i = 0; i < NUM_ROAD_SPRITES; ++i) {
		mHighwayPosY[i] = mNorthRoadSprites[mCurrentEnvironment][i]->Position().y;
	}

	mPlayer->ResetLives();  // Reset player lives
	mPlayer->Reset();  // Reset player state
	mEnemySpawner->Reset();  // Reset enemy spawner
	mPoliceChaseActive = false;
	mPoliceChaseTimer = 0.0f;
	mLevelTime = 0.0f;

	mPlayerScoreNumber->Score(0);  // Reset score
	mSpeedScoreboard->Score(0);  // Reset speed
}
