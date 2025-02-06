#ifndef __STARTSCREEN_H
#define __STARTSCREEN_H
#include "GLTexture.h"
#include "AnimatedGLTexture.h"
#include "InputManager.h"
#include "Scoreboard.h"

using namespace SDLFramework;

class StartScreen : public GameEntity {

private:
	Timer * mTimer;
	InputManager * mInput;

	// Title Screen Entities
	GLTexture* mTitleScreen;
	GLTexture* mTitleScreen2;
	GLTexture* mTitleScreen3;

	// Top Bar Entities
	GameEntity * mTopBar;
	Texture * mPlayerScore;

	Scoreboard * mPlayerScoreNumber;

	// Player Car Entities
	GLTexture* mPlayerCar;

	// Logo Entities
	Texture* mLogoRed;
	Texture* mLogoYellow;
	AnimatedGLTexture* mAnimatedLogo;

	// Side Bar Entities
	GameEntity* mSideBar;
	GLTexture* mZoomingCarLeft;
	GLTexture* mZoomingCarRight;
	float mZoomingCarLeftPosY;
	float mZoomingCarRightPosY;
	float mZoomingCarSpeed;
	float mZoomingCarTimer;

	// Play Mode Entities
	GameEntity * mPlayModes;
	Texture * mStartGame;
	Texture * mOptions;
	Texture * mExit;
	Texture * mCursor;
	Vector2 mCursorStartPos;
	Vector2 mCursorOffset;
	int mSelectedMode;

	// Bottom Bar Entities
	GameEntity * mBottomBar;
	Texture * mDreamTeamStudios;
	Texture * mDates;
	Texture * mRights;

	// Screen Animation Variables
	Vector2 mAnimationStartPos;
	Vector2 mAnimationEndPos;
	float mAnimationTotalTime;
	float mAnimationTimer;
	bool mAnimationDone;

	// Flicker Variables
	float mFlickerTimer;
	bool mFlickerRed;

	// Highway background variables
	float mHighwayPosY;
	float mHighwayPosY2;
	float mHighwayPosY3;
	float mHighwaySpeed;

public:
	StartScreen();
	~StartScreen();

	void ResetAnimation();

	int SelectedMode();

	void ChangeSelectedMode(int change);

	void Update() override;
	void Render() override;
};
#endif