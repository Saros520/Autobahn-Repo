/*#include "PauseMenu.h"
#include "ScreenManager.h"
#include "Graphics.h"

PauseMenu::PauseMenu() {
    mInput = InputManager::Instance();

    mResumeText = new Texture("Resume Game", "emulogic.ttf", 32, { 255, 255, 255 });
    mResumeText->Parent(this);
    mResumeText->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.45f);

    mExitText = new Texture("Exit Game", "emulogic.ttf", 32, { 255, 255, 255 });
    mExitText->Parent(this);
    mExitText->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.55f);

    mCursor = new Texture("Cursor.png");
    mCursor->Parent(this);
    mCursorStartPos = Vector2(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.45f);
    mCursorOffset = Vector2(0, 60);
    mSelectedOption = 0;
    mCursor->Position(mCursorStartPos);
}

PauseMenu::~PauseMenu() {
    delete mResumeText;
    delete mExitText;
    delete mCursor;
}

void PauseMenu::ChangeOption(int change) {
    mSelectedOption += change;
    if (mSelectedOption < 0) mSelectedOption = 1;
    if (mSelectedOption > 1) mSelectedOption = 0;
    mCursor->Position(mCursorStartPos + mCursorOffset * mSelectedOption);
}

int PauseMenu::SelectedOption() {
    return mSelectedOption;
}

void PauseMenu::Update() {
    if (mInput->KeyPressed(SDL_SCANCODE_UP)) {
        ChangeOption(-1);
    }
    else if (mInput->KeyPressed(SDL_SCANCODE_DOWN)) {
        ChangeOption(1);
    }

    if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        if (mSelectedOption == 0) { // Resume
            ScreenManager::Instance()->SetScreen(ScreenManager::Play);
        }
        else if (mSelectedOption == 1) { // Exit
            SDL_Quit();
            exit(0);
        }
    }
}

void PauseMenu::Render() {
    SDL_Renderer* renderer = Graphics::Instance()->GetRenderer();  

    // Enable transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Define a semi-transparent gray box
    SDL_Rect pauseBox = {
        static_cast<int>(Graphics::SCREEN_WIDTH / 2 - 200),
        static_cast<int>(Graphics::SCREEN_HEIGHT / 2 - 100),
        400, 200
    };

    // Set color (Gray with transparency)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 180);
    SDL_RenderFillRect(renderer, &pauseBox);

    mResumeText->Render();
    mExitText->Render();
    mCursor->Render();
}
