#include "OptionsScreen.h"
#include "MathHelper.h"

OptionsScreen::OptionsScreen() {

	mBackground = new Texture("CarSelectScreen.png");
	mBackground->Parent(this);
	mBackground->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mTimer = Timer::Instance();
	mInput = InputManager::Instance();

	// Options Screen Entities 
	mSliderBar = new Texture("SliderBar.png");
	mSliderBar->Parent(this);
	mSliderBar->Position(Graphics::SCREEN_HEIGHT * 0.435f, Graphics::SCREEN_WIDTH * 0.95f);

	mSliderKnob = new Texture("SliderKnob.png");
	mSliderKnob->Parent(this);
	mSliderKnobStartPos = Vector2(mSliderBar->Position().x -120.0f, mSliderBar->Position().y + -453.0f);
	mSliderKnobEndPos = Vector2(mSliderBar->Position().x + 152.0f, mSliderBar->Position().y + -453.0f);
	mSliderKnob->Position(mSliderKnobStartPos);
	mSliderKnob->Position(Graphics::SCREEN_HEIGHT * 0.319f, Graphics::SCREEN_WIDTH * 0.445f);

	mTitle = new Texture("Volume Control", "emulogic.ttf", 33, { 0, 0, 0 });
	mTitle->Parent(this);
	mTitle->Position(Graphics::SCREEN_WIDTH * 0.5f, 400);

	mTitle2 = new Texture("Volume Control", "emulogic.ttf", 32, { 255, 0, 0 });
	mTitle2->Parent(this);
	mTitle2->Position(Graphics::SCREEN_WIDTH * 0.5f, 400);


	mSave = new Texture("Press ESC", "emulogic.ttf", 22, { 0, 0, 0 });
	mSave->Parent(this);
	mSave->Position(Graphics::SCREEN_WIDTH * 0.37f, 580);

	mCancel = new Texture("Too Return", "emulogic.ttf", 22, { 0, 0, 0 });
	mCancel->Parent(this);
	mCancel->Position(Graphics::SCREEN_WIDTH * 0.61f, 580);

	mSliderMinValue = 0.0f;
	mSliderMaxValue = 100.0f;
	mSliderCurrentValue = 10.0f; // Default music volume

}

OptionsScreen::~OptionsScreen() {

	delete mBackground;
	delete mTitle;
	delete mTitle2;
	delete mSave;
	delete mCancel;

	delete mSliderBar;
	mSliderBar = nullptr;

	delete mSliderKnob;
	mSliderKnob = nullptr;

	mTimer = nullptr;
	mInput = nullptr;

}

void OptionsScreen::Update() {
	if (mInput->MouseButtonDown(InputManager::Left)) {
		Vector2 mousePos = mInput->MousePosition();
		if (mousePos.x >= mSliderKnobStartPos.x && mousePos.x <= mSliderKnobEndPos.x) {
			float t = (mousePos.x - mSliderKnobStartPos.x) / (mSliderKnobEndPos.x - mSliderKnobStartPos.x);
			mSliderCurrentValue = mSliderMinValue + t * (mSliderMaxValue - mSliderMinValue);
			mSliderKnob->Position(Lerp(mSliderKnobStartPos, mSliderKnobEndPos, t));
			AudioManager::Instance()->SetMusicVolume(static_cast<int>(mSliderCurrentValue));
		}
	}
}

void OptionsScreen::Render() {
	mBackground->Render();
	mTitle->Render();
	mTitle2->Render();
	mSave->Render();
	mCancel->Render();
	mSliderBar->Render();
	mSliderKnob->Render();
	
}
