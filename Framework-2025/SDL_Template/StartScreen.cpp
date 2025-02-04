#include "StartScreen.h"

StartScreen::StartScreen() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	 
	// Title Screen Entities
	mTitleScreen = new GLTexture("TitleScreen.png");
	mTitleScreen->Parent(this);
	mTitleScreen->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	// Player Car Entities
	mPlayerCar = new GLTexture("PlayerCar1.png");
	mPlayerCar->Parent(this);
	mPlayerCar->Position(Graphics::SCREEN_WIDTH * 0.5f + 43.0f, Graphics::SCREEN_HEIGHT * 0.5f - 105.0f);
	mPlayerCar->Scale(Vector2(8.0f, 8.0f));

	// top bar entities
	mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, 80.0f);
	mPlayerScore = new GLTexture("High-Score", "emulogic.ttf", 32, { 200, 0, 0 });
	mPlayerScoreNumber = new Scoreboard();

	mTopBar->Parent(this);
	mPlayerScore->Parent(mTopBar);

	mPlayerScore->Position(Graphics::SCREEN_WIDTH * -0.12f, -48.0f);

	

	// Title Screen Entities
	mTitleScreen = new GLTexture("TitleScreen.png");
	mTitleScreen->Parent(this);
	mTitleScreen->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	// logo entities
	mLogoRed = new GLTexture("Auto-bahnLogoRed.png", 0, 0, 500, 200);
	mLogoYellow = new GLTexture("Auto-bahnLogoYellow.png", 0, 0, 500, 200);
	
	mLogoRed->Parent(this);
	mLogoYellow->Parent(this);

	mLogoRed->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.2f);
	mLogoYellow->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.22f);

	// Initialize flicker variables
	mFlickerTimer = 1.0f;
	mFlickerRed = true;

	// Side Bar Entities
	mSideBar = new GameEntity(Graphics::SCREEN_HEIGHT * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);
	/*mCars = new AnimatedGLTexture("Cars.png", 0, 0, 100, 100, 4, 0.2f, Animation::Layouts::Vertical);*/

	// play mode entities
	mPlayModes = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.55f);
	mStartGame = new GLTexture("Start Game", "emulogic.ttf", 32, { 200, 0, 0 });
	mOptions = new GLTexture("Options", "emulogic.ttf", 32, { 200, 0, 0 });
	mExit = new GLTexture("Exit", "emulogic.ttf", 32, { 200, 0, 0 });
	mCursor = new GLTexture("Cursor.png");

	mPlayModes->Parent(this);
	mStartGame->Parent(mPlayModes);
	mOptions->Parent(mPlayModes);
	mExit->Parent(mPlayModes);
	mCursor->Parent(mPlayModes);

	mStartGame->Position(0.0f, 105.0f);
	mOptions->Position(0.0f, 175.0f);
	mExit->Position(0.0f, 245.0f);
	mCursor->Position(-175.0f, 25.0f);

	mCursorStartPos = mCursor->Position(Local);
	mCursorOffset = Vector2(50.0f, 70.0f);
	mSelectedMode = 0;

	// bottom bar entities
	mBottomBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.7f);
	mDreamTeamStudios = new GLTexture("dream team studios", "namco__.ttf", 20, { 200, 0, 0 });
	mDates = new GLTexture("2025.", "emulogic.ttf", 20, { 200, 0, 0 });
	mRights = new GLTexture("ALL RIGHTS RESERVED", "emulogic.ttf", 10, { 200, 0, 0 });

	mBottomBar->Parent(this);
	mDreamTeamStudios->Parent(mBottomBar);
	mDates->Parent(mBottomBar);
	mRights->Parent(mBottomBar);

	mDreamTeamStudios->Position(0.0f, 265.0f);
	mDates->Position(-345.0f, 265.0f);
	mRights->Position(0.0f, 285.0f);
}


StartScreen::~StartScreen() {
	// Title Screen Entities
	delete mTitleScreen;  
	mTitleScreen = nullptr;
	
	// top bar entities
	delete mTopBar;
	mTopBar = nullptr;
	delete mPlayerScore;
	mPlayerScore = nullptr;
	delete mPlayerScoreNumber;
	mPlayerScoreNumber = nullptr;

	// Player Car Entities
	delete mPlayerCar;
	mPlayerCar = nullptr;

	// logo entities
	delete mLogoRed;
	mLogoRed = nullptr;
	delete mLogoYellow;
	mLogoYellow = nullptr;

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

		if (mInput->KeyPressed(SDL_SCANCODE_S) || mInput->KeyPressed(SDL_SCANCODE_W) || mInput->KeyPressed(SDL_SCANCODE_DOWN) || mInput->KeyPressed(SDL_SCANCODE_UP)) {
			mAnimationTimer = mAnimationTotalTime;
		}
	}
	else {
		// Update flicker timer
		mFlickerTimer += mTimer->DeltaTime();
		if (mFlickerTimer >= 0.5f) { // Switch every 0.5 seconds
			mFlickerRed = !mFlickerRed;
			mFlickerTimer = 0.0f;
		}

		if (mInput->KeyPressed(SDL_SCANCODE_S) || mInput->KeyPressed(SDL_SCANCODE_DOWN)) {
			ChangeSelectedMode(1);
		}
		else if (mInput->KeyPressed(SDL_SCANCODE_W) || mInput->KeyPressed(SDL_SCANCODE_UP)) {
			ChangeSelectedMode(-1);
		}
	}
}

void StartScreen::Render() {
	mTitleScreen->Render();

	mPlayerCar->Render();

	mPlayerScore->Render();

	// Render the flickering logo
	if (mFlickerRed) {
		mLogoRed->Render();
	}
	else {
		mLogoYellow->Render();
	}

	mStartGame->Render();
	mOptions->Render();
	mExit->Render();
	mCursor->Render();

	mDreamTeamStudios->Render();
	mDates->Render();
	mRights->Render();
}
