#ifndef __PAUSEGAME_H
#define __PAUSEGAME_H

#include "GameEntity.h"
#include "Texture.h"
#include "InputManager.h"

using namespace SDLFramework;

class PauseGame : public GameEntity {
private:
    InputManager* mInput;

    Texture* mBackgroundBox;  // The TransparentBox texture
    Texture* mTitleBorder;
    Texture* mPauseTitle;
    Texture* mTitleBorder2;
    Texture* mResumeText;
    Texture* mResumeText2;
    Texture* mExitText;
    Texture* mExitText2;
    Texture* mVolumeBorder;
    Texture* mVolume;
    Texture* mVolumeBorder2;
    Texture* mSliderBar;
    Texture* mSliderKnob;
    float mSliderMinValue;
    float mSliderMaxValue;
    float mSliderCurrentValue;
    Vector2 mSliderKnobStartPos;
    Vector2 mSliderKnobEndPos;
    Texture* mRadioBorder2;
    Texture* mRadioText;
    Texture* mRadioBorder;
    Texture* mTrackOne;
    Texture* mTrackTwo;
    Texture* mTrackThree;
    Texture* mTrackFour;
    Texture* mTrackFive;
    Texture* mTrackSix;
    Texture* mTrackSeven;
    Texture* mTrackEight;
    Texture* mTrackNine;
    Texture* mCursor;

    Vector2 mCursorStartPos;
    Vector2 mCursorOffset;
    int mSelectedOption;

public:
    PauseGame();
    ~PauseGame();

    void ChangeOption(int change);
    int SelectedOption();
    void Update();
    void Render();
};

#endif


