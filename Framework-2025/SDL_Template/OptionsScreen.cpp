#include "OptionsScreen.h"
#include "MathHelper.h"

OptionsScreen::OptionsScreen() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();

	// Options Screen Entities 
	mSliderBar = new Texture("SliderBar.png");
	mSliderBar->Parent(this);
	mSliderBar->Position(Graphics::SCREEN_HEIGHT * 0.5f, Graphics::SCREEN_WIDTH * 0.5f);

	mSliderKnob = new Texture("SliderKnob.png");
	mSliderKnob->Parent(this);
	mSliderKnobStartPos = Vector2(mSliderBar->Position().x - 200.0f, mSliderBar->Position().y);
	mSliderKnobEndPos = Vector2(mSliderBar->Position().x + 200.0f, mSliderBar->Position().y);
	mSliderKnob->Position(mSliderKnobStartPos);

	mSliderMinValue = 0.0f;
	mSliderMaxValue = 100.0f;
	mSliderCurrentValue = 50.0f; // Default music volume

}

OptionsScreen::~OptionsScreen() {
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
	mSliderBar->Render();
	mSliderKnob->Render();
}
