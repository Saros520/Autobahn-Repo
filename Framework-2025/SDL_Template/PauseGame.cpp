#include "PauseGame.h"
#include "ScreenManager.h"
#include "Graphics.h"
#include "OptionsScreen.h"

PauseGame::PauseGame() {
    mInput = InputManager::Instance();

    // Load the transparent background
    mBackgroundBox = new Texture("TransparentBox.png");
    mBackgroundBox->Parent(this);
    mBackgroundBox->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

    mTitleBorder = new Texture("---------------", "emulogic.ttf", 32, { 255, 0, 0 });
    mTitleBorder->Parent(this);
    mTitleBorder->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.02f);

    mPauseTitle = new Texture("Game Paused", "emulogic.ttf", 33, { 255, 255, 255 });
    mPauseTitle->Parent(this);
    mPauseTitle->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.04f);

    mTitleBorder2 = new Texture("---------------", "emulogic.ttf", 32, { 255, 0, 0 });
    mTitleBorder2->Parent(this);
    mTitleBorder2->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.06f);

    mResumeText = new Texture("Resume Game", "emulogic.ttf", 33, { 255, 255, 255 });
    mResumeText->Parent(this);
    mResumeText->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.13f);

    mResumeText2 = new Texture("Resume Game", "emulogic.ttf", 32, { 255, 0, 0 });
    mResumeText2->Parent(this);
    mResumeText2->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.13f);

    mExitText = new Texture("Exit Game", "emulogic.ttf", 33, { 255, 255, 255 });
    mExitText->Parent(this);
    mExitText->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.18f);

    mExitText2 = new Texture("Exit Game", "emulogic.ttf", 32, { 255, 0, 0 });
    mExitText2->Parent(this);
    mExitText2->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.18f);

    mVolumeBorder = new Texture("------------------", "emulogic.ttf", 32, { 255, 0, 0 });
    mVolumeBorder->Parent(this);
    mVolumeBorder->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.26f);

    mVolume = new Texture("Volume Control", "emulogic.ttf", 32, { 255, 255, 255 });
    mVolume->Parent(this);
    mVolume->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.28f);

    mVolumeBorder2 = new Texture("------------------", "emulogic.ttf", 32, { 255, 0, 0 });
    mVolumeBorder2->Parent(this);
    mVolumeBorder2->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.30f);

    mSliderBar = new Texture("SliderBar.png");
    mSliderBar->Parent(this);
    mSliderBar->Position(Graphics::SCREEN_HEIGHT * 0.435f, Graphics::SCREEN_WIDTH * 0.85f);

    mSliderKnob = new Texture("SliderKnob.png");
    mSliderKnob->Parent(this);
    mSliderKnobStartPos = Vector2(mSliderBar->Position().x - 120.0f, mSliderBar->Position().y + -453.0f);
    mSliderKnobEndPos = Vector2(mSliderBar->Position().x + 152.0f, mSliderBar->Position().y + -453.0f);
    mSliderKnob->Position(mSliderKnobStartPos);
    mSliderKnob->Position(Graphics::SCREEN_HEIGHT * 0.319f, Graphics::SCREEN_WIDTH * 0.345f);

    mRadioBorder2 = new Texture("---------", "emulogic.ttf", 32, { 255, 0, 0 });
    mRadioBorder2->Parent(this);
    mRadioBorder2->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.46f);

    mRadioText = new Texture("Radio", "emulogic.ttf", 32, { 255, 255, 255 });
    mRadioText->Parent(this);
    mRadioText->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.48f);

    mRadioBorder = new Texture("---------", "emulogic.ttf", 32, { 255, 0, 0 });
    mRadioBorder->Parent(this);
    mRadioBorder->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.50f);

    mTrackOne = new Texture("1.Tokyo Drift", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackOne->Parent(this);
    mTrackOne->Position(Graphics::SCREEN_WIDTH * 0.32f, Graphics::SCREEN_HEIGHT * 0.55f);

    mTrackTwo = new Texture("2.Good Old Boys ", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackTwo->Parent(this);
    mTrackTwo->Position(Graphics::SCREEN_WIDTH * 0.375f, Graphics::SCREEN_HEIGHT * 0.60f);

    mTrackThree = new Texture("3.East Bound And Down  ", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackThree->Parent(this);
    mTrackThree->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.65f);

    mTrackFour = new Texture("4.Kickstart My Heart   ", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackFour->Parent(this);
    mTrackFour->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.70f);

    mTrackFive = new Texture("5.Panama  ", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackFive->Parent(this);
    mTrackFive->Position(Graphics::SCREEN_WIDTH * 0.27f, Graphics::SCREEN_HEIGHT * 0.75f);

    mTrackSix = new Texture("6.LaGrande  ", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackSix->Parent(this);
    mTrackSix->Position(Graphics::SCREEN_WIDTH * 0.305f, Graphics::SCREEN_HEIGHT * 0.80f);

    mTrackSeven = new Texture("7.Rollin   ", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackSeven->Parent(this);
    mTrackSeven->Position(Graphics::SCREEN_WIDTH * 0.286f, Graphics::SCREEN_HEIGHT * 0.85f);

    mTrackEight = new Texture("8.Ridin  ", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackEight->Parent(this);
    mTrackEight->Position(Graphics::SCREEN_WIDTH * 0.25f, Graphics::SCREEN_HEIGHT * 0.90f);

    mTrackNine = new Texture("9.Get Low  ", "emulogic.ttf", 32, { 255, 0, 0 });
    mTrackNine->Parent(this);
    mTrackNine->Position(Graphics::SCREEN_WIDTH * 0.285f, Graphics::SCREEN_HEIGHT * 0.95f);

    mCursor = new Texture("Cursor.png");
    mCursor->Parent(this);
    mCursorStartPos = Vector2(Graphics::SCREEN_WIDTH * 0.27f, Graphics::SCREEN_HEIGHT * 0.0494f);
    mCursorOffset = Vector2(0, 53);
    mSelectedOption = 0;
    mCursor->Position(mCursorStartPos);
}

PauseGame::~PauseGame() {
    delete mBackgroundBox;
    delete mTitleBorder;
    delete mPauseTitle;
    delete mTitleBorder2;
    delete mResumeText;
    delete mResumeText2;
    delete mExitText;
    delete mExitText2;
    delete mVolumeBorder;
    delete mVolume;
    delete mVolumeBorder2;
    delete mSliderBar;
    delete mSliderKnob;
    delete mRadioBorder2;
    delete mRadioText;
    delete mRadioBorder;
    delete mTrackOne;
    delete mTrackTwo;
    delete mTrackThree;
    delete mTrackFour;
    delete mTrackFive;
    delete mTrackSix;
    delete mTrackSeven;
    delete mTrackEight;
    delete mTrackNine;
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

    if (mInput->MouseButtonDown(InputManager::Left)) {
        Vector2 mousePos = mInput->MousePosition();
        if (mousePos.x >= mSliderKnobStartPos.x && mousePos.x <= mSliderKnobEndPos.x) {
            float t = (mousePos.x - mSliderKnobStartPos.x) / (mSliderKnobEndPos.x - mSliderKnobStartPos.x);
            mSliderCurrentValue = mSliderMinValue + t * (mSliderMaxValue - mSliderMinValue);
            mSliderKnob->Position(Lerp(mSliderKnobStartPos, mSliderKnobEndPos, t));
            AudioManager::Instance()->SetMusicVolume(static_cast<int>(mSliderCurrentValue));
        }
    }

    if (mInput->KeyPressed(SDL_SCANCODE_UP) || mInput->KeyPressed(SDL_SCANCODE_W)) {
        ChangeOption(-1);
    }
    else if (mInput->KeyPressed(SDL_SCANCODE_DOWN) || mInput->KeyPressed(SDL_SCANCODE_S)) {
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
    mTitleBorder->Render();
    mPauseTitle->Render();
    mTitleBorder2->Render();
    mResumeText->Render();
    mResumeText2->Render();
    mExitText->Render();
    mExitText2->Render();
    mVolumeBorder->Render();
    mVolume->Render();
    mVolumeBorder2->Render();
    mSliderBar->Render();
    mSliderKnob->Render();
    mRadioBorder2->Render();
    mRadioText->Render();
    mRadioBorder->Render();
    mTrackOne->Render();
    mTrackTwo->Render();
    mTrackThree->Render();
    mTrackFour->Render();
    mTrackFive->Render();
    mTrackSix->Render();
    mTrackSeven->Render();
    mTrackEight->Render();
    mTrackNine->Render();
    mCursor->Render();
}
