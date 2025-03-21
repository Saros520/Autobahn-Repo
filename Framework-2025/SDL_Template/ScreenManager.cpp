#include "ScreenManager.h"
#include <iostream>

ScreenManager* ScreenManager::sInstance = nullptr;

ScreenManager* ScreenManager::Instance() {
    if (sInstance == nullptr) {
        sInstance = new ScreenManager();
    }
    return sInstance;
}

void ScreenManager::Release() {
    delete sInstance;
    sInstance = nullptr;
}

ScreenManager::ScreenManager() {
    mInput = InputManager::Instance();

    mStartScreen = new StartScreen();
    mCarSelectScreen = new CarSelectScreen();
    mOptionsScreen = new OptionsScreen();
    mPlayScreen = new PlayScreen();
    mGameOverScreen = nullptr; 

    mCurrentScreen = Start;
    mSelectedCar = 0;
}

ScreenManager::~ScreenManager() {
    delete mStartScreen;
    delete mCarSelectScreen;
    delete mOptionsScreen;
    delete mPlayScreen;
    delete mGameOverScreen;
}

void ScreenManager::SetGameOverBackground(std::string northRoadSprite, std::string southRoadSprite) {
    mGameOverNorthRoadSprite = northRoadSprite;
    mGameOverSouthRoadSprite = southRoadSprite;
    std::cout << "SetGameOverBackground: " << mGameOverNorthRoadSprite << ", " << mGameOverSouthRoadSprite << std::endl;
}

void ScreenManager::SetScreen(Screens newScreen) {
    mCurrentScreen = newScreen;

    if (mCurrentScreen == Play) {
        mPlayScreen->ResetPauseState();  // Reset pause state when loading back into game
        mPlayScreen->GetPlayer()->ResetLives();  // Reset lives when loading new game
        mPlayScreen->Reset();
    }
    else if (mCurrentScreen == GameOver) {
        delete mGameOverScreen; // Delete the previous game over screen if it exists
        mGameOverScreen = new GameOverScreen(
            mGameOverNorthRoadSprite,
            mGameOverSouthRoadSprite,
            ScreenManager::Instance()->GetHighScore()
        );
    }
    else if (mCurrentScreen == Start) {
        mStartScreen->Update(); // Reload the high score when transitioning to the Start screen
    }
}

void ScreenManager::UpdateHighScore(float newHighScore) {
    if (newHighScore > mHighScore) {
        mHighScore = newHighScore;
    }
}

float ScreenManager::GetHighScore() {
    return mHighScore;
}

void ScreenManager::Update() {
    switch (mCurrentScreen) {
    case Start:
        mStartScreen->Update();
        if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
            if (mStartScreen->SelectedMode() == 0) {
                mCurrentScreen = CarSelect;
                mStartScreen->ResetAnimation();
            }
            else if (mStartScreen->SelectedMode() == 1) {
                mCurrentScreen = Options;
            }
            else if (mStartScreen->SelectedMode() == 2) {
                SDL_Quit();
                exit(0);
            }
        }
        break;
    case CarSelect:
        mCarSelectScreen->Update();
        if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
            mCurrentScreen = Play;
            mPlayScreen->GetPlayer()->SetCarTexture(mSelectedCar);
        }
        break;
    case Options:
        mOptionsScreen->Update();
        if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
            mCurrentScreen = Start;
        }
        break;
    case Play:
        mPlayScreen->Update();
        break;
    case GameOver:
        mGameOverScreen->Update();
        if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
            mCurrentScreen = Start;
        }
        break;
    }
}

void ScreenManager::Render() {
    switch (mCurrentScreen) {
    case Start:
        mStartScreen->Render();
        break;
    case CarSelect:
        mCarSelectScreen->Render();
        break;
    case Options:
        mOptionsScreen->Render();
        break;
    case Play:
        mPlayScreen->Render();
        break;
    case GameOver:
        mGameOverScreen->Render();
        break;
    }
}

void ScreenManager::SetSelectedCar(int car) {
    mSelectedCar = car;
}

int ScreenManager::GetSelectedCar() {
    return mSelectedCar;
}
