#include "ScreenManager.h"

ScreenManager * ScreenManager::sInstance = nullptr;

ScreenManager * ScreenManager::Instance() {
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
    mPlayScreen = new PlayScreen();

    mCurrentScreen = Start;
    mSelectedCar = 0;
}

ScreenManager::~ScreenManager() {
    delete mStartScreen;
    delete mCarSelectScreen;
    delete mPlayScreen;
}

void ScreenManager::SetScreen(Screens newScreen) {
	mCurrentScreen = newScreen;
}

void ScreenManager::Update() {
    switch (mCurrentScreen) {
    case Start:
        mStartScreen->Update();
        if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
            mCurrentScreen = CarSelect;
            mStartScreen->ResetAnimation();
        }
        break;
    case CarSelect:
        mCarSelectScreen->Update();
        if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
            mCurrentScreen = Play;
            mPlayScreen->GetPlayer()->SetCarTexture(mSelectedCar);
        }
        break;
    case Play:
        mPlayScreen->Update();
        if (mInput->KeyPressed(SDL_SCANCODE_ESCAPE)) {
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
    case Play:
        mPlayScreen->Render();
        break;
    }
}

void ScreenManager::SetSelectedCar(int car) {
    mSelectedCar = car;
}

int ScreenManager::GetSelectedCar() {
    return mSelectedCar;
}
