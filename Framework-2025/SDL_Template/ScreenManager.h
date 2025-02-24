#ifndef __SCREENMANAGER_H
#define __SCREENMANAGER_H

#include "StartScreen.h"
#include "PlayScreen.h"
#include "CarSelectScreen.h"
#include "OptionsScreen.h"
#include "GameOverScreen.h"
#include "InputManager.h"

class ScreenManager {
public:
    enum Screens { Start, CarSelect, Options, Play, GameOver };

private:
    static ScreenManager* sInstance;
    Screens mCurrentScreen;

    InputManager* mInput;
    StartScreen* mStartScreen;
    PlayScreen* mPlayScreen;
    CarSelectScreen* mCarSelectScreen;
    OptionsScreen* mOptionsScreen;
    GameOverScreen* mGameOverScreen;

    int mSelectedCar;

    ScreenManager();
    ~ScreenManager();

public:
    static ScreenManager* Instance();
    static void Release();

    void Update();
    void Render();

    void SetSelectedCar(int car);
    int GetSelectedCar();
    void SetScreen(Screens newScreen);
};

#endif
