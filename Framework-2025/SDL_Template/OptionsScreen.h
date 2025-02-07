#ifndef __OPTIONS_SCREEN_H
#define __OPTIONS_SCREEN_H

#include "GameEntity.h"
#include "Texture.h"
#include "AudioManager.h"
#include "Timer.h"
#include "InputManager.h"

using namespace SDLFramework;

class OptionsScreen : public GameEntity {

private:
	Timer* mTimer;
	InputManager* mInput;

	// Options Screen Entities 
	Texture* mSliderBar;
	Texture* mSliderKnob;
	float mSliderMinValue;
	float mSliderMaxValue;
	float mSliderCurrentValue;
	Vector2 mSliderKnobStartPos;
	Vector2 mSliderKnobEndPos;

public:
	OptionsScreen();
	~OptionsScreen();

	void Update() override;
	void Render() override;
};

#endif
