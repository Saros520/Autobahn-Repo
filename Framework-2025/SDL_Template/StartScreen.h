#ifndef __STARTSCREEN_H
#define __STARTSCREEN_H
#include "AnimatedGLTexture.h"
#include "InputManager.h"
#include "Scoreboard.h"

using namespace SDLFramework;

class StartScreen : public GameEntity {

private:
	Timer * mTimer;
	InputManager * mInput;

	// TODO:
	// Top Bar Entities
	GameEntity * mTopBar;
	Texture * mPlayerScore;
	/*Texture * mPlayerTwo;
	Texture * mHiScore;*/
	Scoreboard * mPlayerScoreNumber;
	/*Scoreboard * mPlayerTwoScore;
	Scoreboard * mTopScore;*/

	// TODO:
	// Logo Entities
	Texture * mLogo;
	AnimatedGLTexture * mAnimatedLogo;

	// TODO:
	// Side Bar Entities
	GameEntity* mSideBar;
	
	GLTexture* mCars;

	// TODO:
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
	
	// TODO:
	Texture * mDreamTeamStudios;
	Texture * mDates;
	Texture * mRights;

	// Screen Animation Variables
	Vector2 mAnimationStartPos;
	Vector2 mAnimationEndPos;
	float mAnimationTotalTime;
	float mAnimationTimer;
	bool mAnimationDone;

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