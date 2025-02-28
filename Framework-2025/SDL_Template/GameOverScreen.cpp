#include "GameOverScreen.h"
#include "ScreenManager.h"
#include "AssetManager.h"
#include "Graphics.h"
#include <fstream>
#include <iostream>

GameOverScreen::GameOverScreen(std::string northRoadSprite, std::string southRoadSprite, float currentScore) {
    mInput = InputManager::Instance();
    mBustedText = new Texture("Busted", "emulogic.ttf", 72, { 255, 0, 0 });

    // Set the backgrounds for the GameOver screen
    SetBackground(northRoadSprite, southRoadSprite);

    mCurrentScore = currentScore;
    mPlayerScore = new Scoreboard({ 255, 0, 0 });
    mPlayerScore->Distance(mCurrentScore);
    mPlayerScore->Parent(this);
    mPlayerScore->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.4f);

    mHighScoreBoard = new Scoreboard({ 255, 0, 0 }); // Red color for high score
    mHighScoreBoard->LoadHighScore();
    mHighScore = mHighScoreBoard->GetHighScore();

    if (mCurrentScore > mHighScore) {
        mHighScore = mCurrentScore;
        mHighScoreBoard->SetHighScore(mHighScore);
    }

    mHighScoreBoard->Distance(mHighScore);
    mHighScoreBoard->Parent(this);
    mHighScoreBoard->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);
}

GameOverScreen::~GameOverScreen() {
    delete mPlayerScore;
    delete mHighScoreBoard;
    delete mNorthRoadBackground;
    delete mSouthRoadBackground;
    delete mBustedText;
}

void GameOverScreen::SetBackground(std::string NorthRoadSprite, std::string SouthRoadSprite) {
    if (mNorthRoadBackground != nullptr) {
        delete mNorthRoadBackground;
    }
    if (mSouthRoadBackground != nullptr) {
        delete mSouthRoadBackground;
    }
    mNorthRoadBackground = new Texture(NorthRoadSprite);
    if (!mNorthRoadBackground->IsValid()) {
        std::cerr << "Failed to load NorthRoad background texture: " << NorthRoadSprite << std::endl;
    }
    mSouthRoadBackground = new Texture(SouthRoadSprite);
    if (!mSouthRoadBackground->IsValid()) {
        std::cerr << "Failed to load SouthRoad background texture: " << SouthRoadSprite << std::endl;
    }

    // Calculate the scale factor to cover the whole screen
    float scaleX = static_cast<float>(Graphics::SCREEN_WIDTH) / mNorthRoadBackground->ScaledDimensions().x;
    float scaleY = static_cast<float>(Graphics::SCREEN_HEIGHT) / mNorthRoadBackground->ScaledDimensions().y;
    mBackgroundScaleFactor = Vector2(3.36f, 3.45f);

    mNorthRoadBackground->Scale(mBackgroundScaleFactor);
    mSouthRoadBackground->Scale(mBackgroundScaleFactor);

    // Center the backgrounds on the screen
    mNorthRoadBackground->Position(Graphics::SCREEN_WIDTH * 0.78f, Graphics::SCREEN_HEIGHT * 0.5f);
    mSouthRoadBackground->Position(Graphics::SCREEN_WIDTH * 0.22f, Graphics::SCREEN_HEIGHT * 0.5f);
}

void GameOverScreen::Update() {
    if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        ScreenManager::Instance()->SetScreen(ScreenManager::Start);
    }
}

void GameOverScreen::Render() {
    if (mNorthRoadBackground != nullptr && mSouthRoadBackground != nullptr) {
        mNorthRoadBackground->Render();
        mSouthRoadBackground->Render();
    }
    else {
        std::cerr << "Background texture is null" << std::endl;
    }

    mPlayerScore->Render();
    mHighScoreBoard->Render();

    if (mBustedText != nullptr) {
        int screenWidth = Graphics::SCREEN_WIDTH;
        int screenHeight = Graphics::SCREEN_HEIGHT;

        int textWidth = mBustedText->GetWidth();
        int textHeight = mBustedText->GetHeight();

        int x = (screenWidth) / 2;
        int y = (screenHeight) / 2;

        mBustedText->Position(x, y); // Set position for "Busted" text
        mBustedText->Render(); // Render "Busted" text
    }
}
