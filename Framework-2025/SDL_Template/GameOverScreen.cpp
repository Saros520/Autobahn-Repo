#include "GameOverScreen.h"
#include "ScreenManager.h"
#include <fstream>

GameOverScreen::GameOverScreen() {
    mInput = InputManager::Instance();
    mPlayerScore = new Scoreboard();
   
}

GameOverScreen::~GameOverScreen() {
    delete mPlayerScore;
   
}

void GameOverScreen::SetCurrentScore(int score) {
    mCurrentScore = score;
    mPlayerScore->Score(mCurrentScore);
}

void GameOverScreen::Update() {
    if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        ScreenManager::Instance()->SetScreen(ScreenManager::Start);
    }
}

void GameOverScreen::Render() {
    mPlayerScore->Render();
  
}

