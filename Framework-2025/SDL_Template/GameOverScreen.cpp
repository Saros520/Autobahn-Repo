#include "GameOverScreen.h"
#include "ScreenManager.h"
#include "AssetManager.h"
#include "Graphics.h"
#include <fstream>
#include <iostream>

GameOverScreen::GameOverScreen(std::string northRoadSprite, std::string southRoadSprite) {
    mInput = InputManager::Instance();
    mPlayerScore = new Scoreboard();
    mHighScoreBoard = new Scoreboard({ 255, 0, 0 }); // Red color for high score
    mBustedText = new Texture("GAME OVER", "emulogic.ttf", 72, { 255, 255, 255 });
    // Top Bar entities
    mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.7f, Graphics::SCREEN_HEIGHT * 0.4f);
    mGameOver2 = new Texture("game over", "emulogic.ttf", 72, { 255, 0, 0 });
    // bottom bar entities
    mBottomBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.7f, Graphics::SCREEN_HEIGHT * 0.4f);
    mCreator = new Texture("creators", "namco__.ttf", 26, { 255, 0, 0 });
    mTopBorder = new Texture("------------------------", "emulogic.ttf", 26, { 0, 0, 0 });
    mCreatorName = new Texture("keigan", "namco__.ttf", 26, { 0, 0, 0 });
    mCreatorName2 = new Texture("andrew", "namco__.ttf", 26, { 0, 0, 0 });
    mCreatorName3 = new Texture("josh", "namco__.ttf", 26, { 0, 0, 0 });
    mBottomBorder = new Texture("------------------------", "emulogic.ttf", 26, { 0, 0, 0 });
    mMainMenu2 = new Texture("press enter to return", "namco__.ttf", 26, { 0, 0, 0 });
    mMainMenu3 = new Texture("to main menu", "namco__.ttf", 26, { 0, 0, 0 });

    mBottomBar->Parent(this);
    mCreator->Parent(mBottomBar);
    mTopBorder->Parent(mBottomBar);
    mCreatorName->Parent(mBottomBar);
    mCreatorName2->Parent(mBottomBar);
    mCreatorName3->Parent(mBottomBar);
    mBottomBorder->Parent(mBottomBar);
    mMainMenu2->Parent(mBottomBar);
    mMainMenu3->Parent(mBottomBar);

    // Set the backgrounds for the GameOver screen
    SetBackground(northRoadSprite, southRoadSprite);

    // Load high score from file
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> mHighScore;
    }
    else {
        mHighScore = 0;
    }
    file.close();
}

GameOverScreen::~GameOverScreen() {
    delete mPlayerScore;
    delete mHighScoreBoard;
    delete mNorthRoadBackground;
    delete mSouthRoadBackground;
    delete mBustedText;

    delete mTopBar;
    mTopBar = nullptr;
    delete mGameOver2;
    mGameOver2 = nullptr;

    delete mBottomBar;
    mBottomBar = nullptr;
    delete mCreator;
    mCreator = nullptr;
    delete mTopBorder;
    mTopBorder = nullptr;
    delete mCreatorName;
    mCreatorName = nullptr;
    delete mCreatorName2;
    mCreatorName2 = nullptr;
    delete mCreatorName3;
    mCreatorName3 = nullptr;
    delete mBottomBorder;
    mBottomBorder = nullptr;
    delete mMainMenu2;
    mMainMenu2 = nullptr;
    delete mMainMenu3;
    mMainMenu3 = nullptr;
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
    mCreator->Position(Graphics::SCREEN_WIDTH * -0.20f, Graphics::SCREEN_HEIGHT * -0.36f);
    mTopBorder->Position(Graphics::SCREEN_WIDTH * -0.20f, Graphics::SCREEN_HEIGHT * 0.055f);
    mCreatorName->Position(Graphics::SCREEN_WIDTH * -0.20f, Graphics::SCREEN_HEIGHT * -0.30f);
    mCreatorName2->Position(Graphics::SCREEN_WIDTH * -0.20f, Graphics::SCREEN_HEIGHT * -0.25f);
    mCreatorName3->Position(Graphics::SCREEN_WIDTH * -0.20f, Graphics::SCREEN_HEIGHT * -0.20f);
    mBottomBorder->Position(Graphics::SCREEN_WIDTH * -0.20f, Graphics::SCREEN_HEIGHT * 0.15f);
    mMainMenu2->Position(Graphics::SCREEN_WIDTH * -0.20f, Graphics::SCREEN_HEIGHT * 0.40f);
    mMainMenu3->Position(Graphics::SCREEN_WIDTH * -0.20f, Graphics::SCREEN_HEIGHT * 0.45f);
    mGameOver2->Position(Graphics::SCREEN_WIDTH * 0.49f, Graphics::SCREEN_HEIGHT * 0.5f);
}

void GameOverScreen::SetCurrentScore(int score) {
    mCurrentScore = score;
    mPlayerScore->Score(mCurrentScore);

    if (mCurrentScore > mHighScore) {
        mHighScore = mCurrentScore;
        mHighScoreBoard->Score(mHighScore);

        // Save new high score to file
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << mHighScore;
            file.close();
        }
    }
    else {
        mHighScoreBoard->Score(mHighScore);
    }
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
    mCreator->Render();
    mTopBorder->Render();
    mCreatorName->Render();
    mCreatorName2->Render();
    mCreatorName3->Render();
    mBottomBorder->Render();
    mMainMenu2->Render();
    mMainMenu3->Render();
    mGameOver2->Render();
}
