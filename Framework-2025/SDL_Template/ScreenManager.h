#ifndef __SCREENMANAGER_H
#define __SCREENMANAGER_H

#include "PlayScreen.h"
#include "GameOverScreen.h"
#include "StartScreen.h"
#include "CarSelectScreen.h"
#include "OptionsScreen.h"
#include "InputManager.h"

class ScreenManager {
public:
    enum Screens { Start, CarSelect, Options, Play, GameOver };

private:
    static ScreenManager* sInstance;

    InputManager* mInput;

    StartScreen* mStartScreen;
    CarSelectScreen* mCarSelectScreen;
    OptionsScreen* mOptionsScreen;
    PlayScreen* mPlayScreen;
    GameOverScreen* mGameOverScreen;

    Screens mCurrentScreen;

    std::string mGameOverNorthRoadSprite;
    std::string mGameOverSouthRoadSprite;

    int mSelectedCar;

public:
    static ScreenManager* Instance();
    static void Release();

    void SetScreen(Screens newScreen);
    void SetGameOverBackground(std::string northRoadSprite, std::string southRoadSprite);

    void Update();
    void Render();

    void SetSelectedCar(int car);
    int GetSelectedCar();

private:
    ScreenManager();
    ~ScreenManager();
};

#endif
