#ifndef __CARSELECTSCREEN_H
#define __CARSELECTSCREEN_H

#include "GLTexture.h"
#include "InputManager.h"
#include "Scoreboard.h"

using namespace SDLFramework;
class ScreenManager;
class CarSelectScreen : public GameEntity {
private:
    InputManager* mInput;

    Texture* mBackground;
    Texture* mTitle;
    Texture* mCars[6];
    Texture* mCursor;
    Vector2 mCursorStartPos;
    Vector2 mCursorOffset;
    int mSelectedCar;

public:
    CarSelectScreen();
    ~CarSelectScreen();

    int SelectedCar();
    void ChangeSelectedCar(int change);
    void Update() override;
    void Render() override;
};

#endif

