#include "CarSelectScreen.h"
#include "ScreenManager.h"

CarSelectScreen::CarSelectScreen() {
    mInput = InputManager::Instance();

    mBackground = new Texture("CarSelectScreen.png");
    mBackground->Parent(this);
    mBackground->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

    mTitle = new Texture("Select Your Car", "emulogic.ttf", 32, { 255, 255, 255 });
    mTitle->Parent(this);
    mTitle->Position(Graphics::SCREEN_WIDTH * 0.5f, 50);

    for (int i = 0; i < 6; i++) {
        std::string fileName = "PlayerCar" + std::to_string(i + 1) + ".png";
        mCars[i] = new Texture(fileName);
        mCars[i]->Parent(this);
        mCars[i]->Position(200 + i * 150, Graphics::SCREEN_HEIGHT * 0.5f);
    }

    mCursor = new Texture("Cursor.png");
    mCursor->Parent(this);
    mCursorStartPos = Vector2(150, Graphics::SCREEN_HEIGHT * 0.5f);
    mCursorOffset = Vector2(150, 0);
    mSelectedCar = 0;
    mCursor->Position(mCursorStartPos);
}

CarSelectScreen::~CarSelectScreen() {
    delete mBackground;
    delete mTitle;
    delete mCursor;

    for (int i = 0; i < 6; i++) {
        delete mCars[i];
    }
}

int CarSelectScreen::SelectedCar() {
    return mSelectedCar;
}

void CarSelectScreen::ChangeSelectedCar(int change) {
    mSelectedCar += change;
    if (mSelectedCar < 0) mSelectedCar = 5;
    if (mSelectedCar > 5) mSelectedCar = 0;
    mCursor->Position(mCursorStartPos + mCursorOffset * mSelectedCar);
}

void CarSelectScreen::Update() {
    if (mInput->KeyPressed(SDL_SCANCODE_LEFT)) {
        ChangeSelectedCar(-1);
    }
    if (mInput->KeyPressed(SDL_SCANCODE_RIGHT)) {
        ChangeSelectedCar(1);
    }
    if (mInput->KeyPressed(SDL_SCANCODE_RETURN)) {
        ScreenManager::Instance()->SetSelectedCar(mSelectedCar);
        ScreenManager::Instance()->SetScreen(ScreenManager::Play);
    }
}

void CarSelectScreen::Render() {
    mBackground->Render();
    mTitle->Render();

    for (int i = 0; i < 6; i++) {
        mCars[i]->Render();
    }

    mCursor->Render();
}