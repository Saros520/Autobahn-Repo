#include "StartScreen.h"

StartScreen::StartScreen() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();

	// top bar entities
	mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, 80.0f);
	mPlayerScore = new GLTexture("PlayerScore", "emulogic.ttf", 32, { 200, 0, 0 });
	mPlayerScoreNumber = new Scoreboard();

	mTopBar->Parent(this);
	mPlayerScore->Parent(mTopBar);

	mPlayerScore->Position(-Graphics::SCREEN_WIDTH * 0.0f, 0.0f);

	// logo entities
	mLogo = new GLTexture("Auto-bahnLogo.png", 0, 0, 500, 200);
	mAnimatedLogo = new AnimatedGLTexture("Auto-bahnLogo.png", 0, 0, 500, 200, 3, 0.0f, Animation::Layouts::Vertical);
	
	mLogo->Parent(this);
	mAnimatedLogo->Parent(this);

	mLogo->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.22f);
	mAnimatedLogo->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.2f);

	// Side Bar Entities
	mSideBar = new GameEntity(Graphics::SCREEN_HEIGHT * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);
	/*mCars = new AnimatedGLTexture("Cars.png", 0, 0, 100, 100, 4, 0.2f, Animation::Layouts::Vertical);*/

	// play mode entities
	mPlayModes = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.55f);
	mStartGame = new GLTexture("Start Game", "emulogic.ttf", 32, { 230, 230, 230 });
	mOptions = new GLTexture("Options", "emulogic.ttf", 32, { 230, 230, 230 });
	mExit = new GLTexture("Exit", "emulogic.ttf", 32, { 230, 230, 230 });
	mCursor = new GLTexture("Cursor.png");

	mPlayModes->Parent(this);
	mStartGame->Parent(mPlayModes);
	mOptions->Parent(mPlayModes);
	mExit->Parent(mPlayModes);
	mCursor->Parent(mPlayModes);

	mStartGame->Position(0.0f, 105.0f);
	mOptions->Position(0.0f, 175.0f);
	mExit->Position(0.0f, 245.0f);
	mCursor->Position(-185.0f, 105.0f);

	mCursorStartPos = mCursor->Position(Local);
	mCursorOffset = Vector2(50.0f, 70.0f);
	mSelectedMode = 0;

	// bottom bar entities
	mBottomBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.7f);
	mDreamTeamStudios = new GLTexture("Dream Team Studios", "namco__.ttf", 10, { 200, 0, 0 });
	mDates = new GLTexture("2025.", "emulogic.ttf", 15, { 230, 230, 230 });
	mRights = new GLTexture("ALL RIGHTS RESERVED", "emulogic.ttf", 10, { 230, 230, 230 });

	mBottomBar->Parent(this);
	mDreamTeamStudios->Parent(mBottomBar);
	mDates->Parent(mBottomBar);
	mRights->Parent(mBottomBar);

	mDreamTeamStudios->Position(300.0f, 180.0f);
	mDates->Position(-300.0f, 200.0f);
	mRights->Position(300.0f, 200.0f);

	// screen animation variables
	ResetAnimation();

	// Initialize flicker variables
	mFlickerTimer = 0.0f;
	mFlickerRed = true;
}


StartScreen::~StartScreen() {
	// top bar entities
	delete mTopBar;
	mTopBar = nullptr;
	delete mPlayerScore;
	mPlayerScore = nullptr;
	delete mPlayerScoreNumber;
	mPlayerScoreNumber = nullptr;
	/*delete mPlayerTwo;
	mPlayerTwo = nullptr;
	delete mHiScore;
	mHiScore = nullptr;
	delete mPlayerOneScore;
	mPlayerOneScore = nullptr;
	delete mPlayerTwoScore;
	mPlayerTwoScore = nullptr;
	delete mTopScore;
	mTopScore = nullptr;*/

	// logo entities
	delete mLogo;
	mLogo = nullptr;
	delete mAnimatedLogo;
	mAnimatedLogo = nullptr;

	// Sidebar entities
	delete mSideBar;
	mSideBar = nullptr;
	delete mCars;
	mCars = nullptr;

	// play mode entities
	delete mPlayModes;
	mPlayModes = nullptr;
	delete mStartGame;
	mStartGame = nullptr;
	delete mOptions;
	mOptions = nullptr;
	delete mExit;
	mExit = nullptr;
	delete mCursor;
	mCursor = nullptr;

	// bottom bar entities
	delete mBottomBar;
	mBottomBar = nullptr;
	delete mDreamTeamStudios;
	mDreamTeamStudios = nullptr;
	delete mDates;
	mDates = nullptr;
	delete mRights;
	mRights = nullptr;

	mTimer = nullptr;
	mInput = nullptr;
}

void StartScreen::ResetAnimation() {
	mAnimationStartPos = Vector2(0.0f, Graphics::SCREEN_HEIGHT);
	mAnimationEndPos = Vec2_Zero;
	mAnimationTotalTime = 5.0f;
	mAnimationTimer = 0.0f;
	mAnimationDone = false;

	/*Position(mAnimationStartPos);*/
}

int StartScreen::SelectedMode() {
	return mSelectedMode;
}

void StartScreen::ChangeSelectedMode(int change) {
	mSelectedMode += change;

	if (mSelectedMode < 0) {
		mSelectedMode = 2;
	}
	else if (mSelectedMode > 2) {
		mSelectedMode = 0;
	}

	mCursor->Position(mCursorStartPos + mCursorOffset * (float)mSelectedMode);
}

void StartScreen::Update() {
	if (!mAnimationDone) {
		mAnimationTimer += mTimer->DeltaTime();
		Position(Lerp(mAnimationStartPos, mAnimationEndPos, mAnimationTimer / mAnimationTotalTime));

		if (mAnimationTimer >= mAnimationTotalTime) {
			mAnimationDone = true;
		}

		if (mInput->KeyPressed(SDL_SCANCODE_S) || mInput->KeyPressed(SDL_SCANCODE_W)) {
			mAnimationTimer = mAnimationTotalTime;
		}
	}
	else {
		mAnimatedLogo->Update();

		if (mInput->KeyPressed(SDL_SCANCODE_S)) {
			ChangeSelectedMode(1);
		}
		else if (mInput->KeyPressed(SDL_SCANCODE_W)) {
			ChangeSelectedMode(-1);
		}

		// Update flicker timer
		mFlickerTimer += mTimer->DeltaTime();
		if (mFlickerTimer >= 1.5f) { // Change color every 0.5 seconds
			mFlickerRed = !mFlickerRed;
			mFlickerTimer = 0.0f;

			if (mFlickerRed) {
				mAnimatedLogo->SetColor(255, 0, 0); // Red
			}
			else {
				mAnimatedLogo->SetColor(255, 255, 0); // Yellow
			}
		}
	}
}

void StartScreen::Render() {
	mPlayerScore->Render();
	/*mPlayerTwo->Render();
	mHiScore->Render();
	mPlayerOneScore->Render();
	mPlayerTwoScore->Render();
	mTopScore->Render();*/

	if (!mAnimationDone) {
		mLogo->Render();
	}
	else {
		mAnimatedLogo->Render();
	}

	mStartGame->Render();
	mOptions->Render();
	mExit->Render();
	mCursor->Render();

	mDreamTeamStudios->Render();
	mDates->Render();
	mRights->Render();
}