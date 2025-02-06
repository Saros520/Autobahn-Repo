#ifndef __SCREENMANAGER_H
#define __SCREENMANAGER_H

#include "StartScreen.h"
#include "PlayScreen.h"
#include "CarSelectScreen.h" 
#include "InputManager.h"

class ScreenManager {
public:
    enum Screens { Start, CarSelect, Play };

private:
    static ScreenManager* sInstance;
    Screens mCurrentScreen;

    InputManager* mInput;
    StartScreen* mStartScreen;
    PlayScreen* mPlayScreen;
    CarSelectScreen* mCarSelectScreen;
    int mSelectedCar;

public:
    static ScreenManager* Instance();
    static void Release();

    ScreenManager();
    ~ScreenManager();

    void Update();
    void Render();

    void SetSelectedCar(int car);
    int GetSelectedCar();
    void SetScreen(Screens newScreen);
};

#endif
