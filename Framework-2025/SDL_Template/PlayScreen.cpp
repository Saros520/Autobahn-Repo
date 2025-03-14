#include "PlayScreen.h"
#include "ScreenManager.h"
#include "InputManager.h"
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <sstream>

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
    mHighScoreNumber = new Scoreboard();

    mTopBar->Parent(this);
    mPlayerScore->Parent(mTopBar);
    mPlayerScoreNumber->Parent(mTopBar);
    mHighScoreNumber->Parent(mTopBar);

    mPlayerScore->Position(Graphics::SCREEN_WIDTH * -0.12f, -63.0f);
    mPlayerScoreNumber->Position(Graphics::SCREEN_WIDTH * 0.35f, -63.0f);
    mHighScoreNumber->Position(Graphics::SCREEN_WIDTH * 0.2f, -63.0f);

    mPlayer = new Player();
    mPlayer->Parent(this);
    mPlayer->Position(Graphics::SCREEN_WIDTH * 0.642f, Graphics::SCREEN_HEIGHT * 0.9f); // offset to fit lanes centered

    mEnemySpawner = new EnemySpawner(4.0f, vehicleIndex); // Spawn an enemy every 4 seconds

    mEnemyPolice = nullptr;
    mPoliceChaseActive = false;
    mPoliceChaseTimer = 0.0f;

    mTopPoliceCar = nullptr;
    mTopPoliceChaseActive = false;
    mTopPoliceChaseTimer = 0.0f;

    mDualPoliceChaseActive = false;
    mDualPoliceChaseTimer = 0.0f;

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

    mLivesScoreboard = new Scoreboard();
    mLivesScoreboard->Parent(mBottomBar);
    mLivesScoreboard->Position(Graphics::SCREEN_WIDTH * -0.72f, 86.0f);

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

    if (mEnemyPolice) {
        delete mEnemyPolice;
        mEnemyPolice = nullptr;
    }

    if (mTopPoliceCar) {
        delete mTopPoliceCar;
        mTopPoliceCar = nullptr;
    }

    for (auto policeCar : mPoliceCars) {
        delete policeCar;
    }
    mPoliceCars.clear();

    delete mPlayerScore;
    mPlayerScore = nullptr;

    delete mPlayerScoreNumber;
    mPlayerScoreNumber = nullptr;

    delete mHighScoreNumber;
    mHighScoreNumber = nullptr;

    delete mTopBar;
    mTopBar = nullptr;

    delete mSpeedLabel;
    mSpeedLabel = nullptr;

    delete mSpeedScoreboard;
    mSpeedScoreboard = nullptr;

    delete mLivesLabel;
    mLivesLabel = nullptr;

    delete mLivesScoreboard;
    mLivesScoreboard = nullptr;

    delete mBottomBar;
    mBottomBar = nullptr;

    // Clean up spike strips
    for (auto& spikeStrip : mSpikeStrips) {
        delete spikeStrip;
    }
    mSpikeStrips.clear();
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

void PlayScreen::SpawnPoliceCar(bool isTopPoliceCar) {
    // First police chase (only bottom police car)
    if (!isTopPoliceCar) {
        delete mEnemyPolice;
        mEnemyPolice = nullptr;
        mEnemyPolice = new EnemyPolice(mPlayer, mEnemySpawner, false);
        mEnemyPolice->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 1.05f);
        std::cout << "Spawned bottom police car." << std::endl;
        return;
    }

    // Second police chase (only top police car)
    if (isTopPoliceCar) {
        delete mTopPoliceCar;
        mTopPoliceCar = nullptr;
        mTopPoliceCar = new EnemyPolice(mPlayer, mEnemySpawner, true);
        std::cout << "Spawned top police car." << std::endl;
        return;
    }

    // Third chase (both police cars active)
    if (mLevelTime >= 420.0f) {
        EnemyPolice* newPoliceCar = new EnemyPolice(mPlayer, mEnemySpawner, isTopPoliceCar);
        mPoliceCars.push_back(newPoliceCar);
        std::cout << "Spawned dual police car. Top: " << isTopPoliceCar << std::endl;
    }
}

void PlayScreen::HandlePoliceCarSpawning(bool isTopPoliceCar) {
    if (isTopPoliceCar && mTopPoliceChaseActive) {
        SpawnPoliceCar(true);
    }
    else if (!isTopPoliceCar && mPoliceChaseActive) {
        SpawnPoliceCar(false);
    }
}

void PlayScreen::SpawnPoliceCarAtMidpoint(bool isTopPoliceCar) {
    float spawnX = Graphics::SCREEN_WIDTH * 0.5f;
    float spawnY = isTopPoliceCar ? -50.0f : Graphics::SCREEN_HEIGHT + 50.0f;

    EnemyPolice* policeCar = new EnemyPolice(mPlayer, mEnemySpawner, isTopPoliceCar);
    policeCar->Parent(this);
    policeCar->Position(Vector2(spawnX, spawnY));

    if (isTopPoliceCar) {
        mTopPoliceCar = policeCar;
        mTopPoliceChaseActive = true;
        mTopPoliceChaseTimer = 0.0f;
    }
    else {
        mEnemyPolice = policeCar;
        mPoliceChaseActive = true;
        mPoliceChaseTimer = 0.0f;
    }
    std::cout << "Police car spawned at midpoint. Top police car: " << isTopPoliceCar << std::endl; // Debug log
}

void PlayScreen::DestroyPoliceCar() {
    if (mTopPoliceChaseActive && mTopPoliceCar) {
        mTopPoliceCar->Destroy();
        delete mTopPoliceCar;
        mTopPoliceCar = nullptr;
        mTopPoliceChaseActive = false;
    }

    if (mPoliceChaseActive && mEnemyPolice) {
        mEnemyPolice->Destroy();
        delete mEnemyPolice;
        mEnemyPolice = nullptr;
        mPoliceChaseActive = false;
    }
    std::cout << "Police car destroyed." << std::endl; // Debug log
}

void PlayScreen::AddSpikeStrip(SpikeStrip* spikeStrip) {
    if (spikeStrip != nullptr) {
        mSpikeStrips.push_back(spikeStrip);
    }
}

void PlayScreen::RemoveOffScreenSpikeStrips() {
    for (int i = mSpikeStrips.size() - 1; i >= 0; i--) {
        if (mSpikeStrips[i]->Position().y > mScreenBoundsY) {
            std::vector<SpikeStrip*>::iterator iter = std::find(mSpikeStrips.begin(), mSpikeStrips.end(), mSpikeStrips[i]);
            mSpikeStrips.erase(iter);
        }

    }
}

void PlayScreen::StartPoliceChase() {
    if (!mPoliceChaseActive && mEnemyPolice == nullptr) {
        SpawnPoliceCar(false);
        mPoliceChaseActive = true;
        mPoliceChaseTimer = 0.0f;
    }
}

void PlayScreen::EndPoliceChase(bool isTopPoliceCar) {
    if (isTopPoliceCar && mTopPoliceCar) {
        mTopPoliceCar->Destroy();
        delete mTopPoliceCar;
        mTopPoliceCar = nullptr;
        mTopPoliceChaseActive = false;
        mTopPoliceChaseTimer = 0.0f;
        std::cout << "Ended top police chase." << std::endl;
    }
    else if (!isTopPoliceCar && mEnemyPolice) {
        mEnemyPolice->Destroy();
        delete mEnemyPolice;
        mEnemyPolice = nullptr;
        mPoliceChaseActive = false;
        mPoliceChaseTimer = 0.0f;
        std::cout << "Ended bottom police chase." << std::endl;
    }
}

void PlayScreen::StartTopPoliceChase() {
    if (!mTopPoliceChaseActive && mTopPoliceCar == nullptr) {
        SpawnPoliceCar(true);
        mTopPoliceChaseActive = true;
        mTopPoliceChaseTimer = 0.0f;
    }
}

void PlayScreen::UpdateTopPoliceCar() {
    if (mTopPoliceChaseActive) {
        mTopPoliceChaseTimer += mTimer->DeltaTime();
        if (mTopPoliceCar != nullptr) {
            mTopPoliceCar->Update();
        }

        if (mTopPoliceChaseTimer >= 60.0f) {
            EndTopPoliceChase();
        }

    }
}

void PlayScreen::EndTopPoliceChase() {
    if (mTopPoliceChaseActive) {
        if (mTopPoliceCar != nullptr) {
            mTopPoliceCar->Destroy();
            delete mTopPoliceCar;
            mTopPoliceCar = nullptr;
        }
        mTopPoliceChaseActive = false;
    }
}

void PlayScreen::StartDualPoliceChase() {
    if (!mDualPoliceChaseActive) {
        SpawnPoliceCar(false); // Bottom police car
        SpawnPoliceCar(true);  // Top police car
        mDualPoliceChaseActive = true;
        mDualPoliceChaseTimer = 0.0f;
    }
}

void PlayScreen::EndDualPoliceChase() {
    for (auto it = mPoliceCars.begin(); it != mPoliceCars.end(); ) {
        (*it)->Destroy();
        delete* it;
        it = mPoliceCars.erase(it);
    }
    mDualPoliceChaseActive = false;
    mDualPoliceChaseTimer = 0.0f; // Reset timer
    std::cout << "Ended dual police chase." << std::endl;
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

    mLivesScoreboard->Score(mPlayer->Lives()); 
    mLivesScoreboard->Update();

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

        // First police chase (120s start, lasts 60s)
        if (mLevelTime >= 120.0f && mEnemyPolice == nullptr) {
            StartPoliceChase();
        }
        if (mEnemyPolice) {
            mPoliceChaseTimer += mTimer->DeltaTime();
            mEnemyPolice->Update();
            if (mPoliceChaseTimer >= 60.0f) {
                EndPoliceChase(false);
                if (mEnemyPolice) {
                    delete mEnemyPolice;
                    mEnemyPolice = nullptr;
                }
            }
        }

        // Second police chase (240s start, lasts 60s)
        if (mLevelTime >= 240.0f && mTopPoliceCar == nullptr) {
            StartTopPoliceChase();
        }
        if (mTopPoliceCar) {
            mTopPoliceChaseTimer += mTimer->DeltaTime();
            mTopPoliceCar->Update();
            if (mTopPoliceChaseTimer >= 60.0f) {
                EndPoliceChase(true);
                if (mTopPoliceCar) {
                    delete mTopPoliceCar;
                    mTopPoliceCar = nullptr;
                }
            }
        }
        
        // Third police chase (420s start, lasts 10,000s)
        if (mLevelTime >= 420.0f && !mDualPoliceChaseActive) {
            StartDualPoliceChase();
            mDualPoliceChaseActive = true;
            mDualPoliceChaseTimer = 0.0f; // Reset timer
        }
        if (mDualPoliceChaseActive) {
            mDualPoliceChaseTimer += mTimer->DeltaTime();

            // Update both police cars in the third chase
            for (auto it = mPoliceCars.begin(); it != mPoliceCars.end();) {
                if ((*it)->IsDestroyed()) {
                    delete* it;
                    it = mPoliceCars.erase(it);
                }
                else {
                    (*it)->Update();
                    ++it;
                }
            }

            if (mDualPoliceChaseTimer >= 10000.0f) {
                EndDualPoliceChase();
            }
        }

        // Check for game over conditions
        if (mPlayer->IsOutOfLives() || mPlayer->WasHitByPolice() || mPlayer->WasHit()) {
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

        mPlayerScoreNumber->Score(mPlayer->Score());
        mPlayerScoreNumber->Update();

        int playerLives = mPlayer->Lives();
        mSpeedScoreboard->Score(playerLives); // Display remaining lives
        mSpeedScoreboard->Update();

        // Display distance traveled in kilometers to the hundredths place
        mPlayerScoreNumber->Distance(mPlayer->DistanceTraveled());
        mPlayerScoreNumber->Update();

        // Update high score if the current distance is greater
        float currentDistance = mPlayer->DistanceTraveled();
        float highScore = mHighScoreNumber->GetHighScore();
        
        if (currentDistance > highScore) {
            mHighScoreNumber->SetHighScore(currentDistance);
            ScreenManager::Instance()->UpdateHighScore(currentDistance);  // Store globally
        }
        
        mHighScoreNumber->Distance(ScreenManager::Instance()->GetHighScore());
        mHighScoreNumber->Update();

        float speed = mPlayer->GetSpeed();
        mSpeedScoreboard->Score(static_cast<int>(speed));
        mSpeedScoreboard->Update();

        // Update the spike strip
        for (auto it : mSpikeStrips) {
            it->Update();
        }

        // Check for out-of-bounds spike strips and remove them
        RemoveOffScreenSpikeStrips();
        
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
    mLivesScoreboard->Render();
	mPlayer->Render();
	mEnemySpawner->Render();
	
    if (mPoliceChaseActive && mEnemyPolice != nullptr) {
        std::cout << "Rendering bottom police car at: " << mEnemyPolice->Position().x << ", " << mEnemyPolice->Position().y << std::endl;
        mEnemyPolice->Render();
    }

    if (mTopPoliceChaseActive && mTopPoliceCar != nullptr) {
        mTopPoliceCar->Render();
    }

    for (auto policeCar : mPoliceCars) {
        if (policeCar) {
            policeCar->Render();
        }
    }

    for (auto& spikeStrip : mSpikeStrips) {
        spikeStrip->Render();
    }

    mPlayerScore->Render();
    mPlayerScoreNumber->Render();
    mHighScoreNumber->Render();
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
    ScreenManager::Instance()->SetGameOverBackground(
        currentNorthRoadType + std::to_string(currentNorthRoadIndex) + ".png",
        currentSouthRoadType + std::to_string(currentSouthRoadIndex) + ".png"
    );
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
    mTopPoliceChaseActive = false;
    mTopPoliceChaseTimer = 0.0f;
    mLevelTime = 0.0f;

    mPlayerScoreNumber->Score(0);  // Reset score
    mSpeedScoreboard->Score(0);  // Reset speed
}
