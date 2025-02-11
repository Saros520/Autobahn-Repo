#include "PauseGame.h"
#include "ScreenManager.h"
#include "Graphics.h"

PauseGame::PauseGame() {
    mInput = InputManager::Instance();

    // Load the transparent background
    mBackgroundBox = new Texture("TransparentBox.png");
    mBackgroundBox->Parent(this);
    mBackgroundBox->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

    mResumeText = new Texture("Resume Game", "emulogic.ttf", 32, { 255, 255, 255 });
    mResumeText->Parent(this);
    mResumeText->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.45f);

    mExitText = new Texture("Exit Game", "emulogic.ttf", 32, { 255, 255, 255 });
    mExitText->Parent(this);
    mExitText->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5111f);

    mCursor = new Texture("Cursor.png");
    mCursor->Parent(this);
    mCursorStartPos = Vector2(Graphics::SCREEN_WIDTH * 0.27f, Graphics::SCREEN_HEIGHT * 0.366f);
    mCursorOffset = Vector2(0, 69);
    mSelectedOption = 0;
    mCursor->Position(mCursorStartPos);
}

PauseGame::~PauseGame() {
    delete mBackgroundBox;
    delete mResumeText;
    delete mExitText;
    delete mCursor;
}

void PauseGame::ChangeOption(int change) {
    mSelectedOption += change;
    if (mSelectedOption < 0) mSelectedOption = 1;
    if (mSelectedOption > 1) mSelectedOption = 0;
    mCursor->Position(mCursorStartPos + mCursorOffset * mSelectedOption);
}

int PauseGame::SelectedOption() {
    return mSelectedOption;
}

void PauseGame::Update() {
    if (mInput->KeyPressed(SDL_SCANCODE_UP)) {
        ChangeOption(-1);
    }
    else if (mInput->KeyPressed(SDL_SCANCODE_DOWN)) {
        ChangeOption(1);
    }

    if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        if (mSelectedOption == 0) { // Resume
        }
        else if (mSelectedOption == 1) { // Exit to Main Menu
            ScreenManager::Instance()->SetScreen(ScreenManager::Start);
        }
    }

}

void PauseGame::Render() {
    // Render the transparent box background
    mBackgroundBox->Render();

    // Render text and cursor
    mResumeText->Render();
    mExitText->Render();
    mCursor->Render();
}
