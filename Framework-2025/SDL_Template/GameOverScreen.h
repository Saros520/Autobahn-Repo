#ifndef __GAMEOVERSCREEN_H
#define __GAMEOVERSCREEN_H

#include "GameEntity.h"
#include "Scoreboard.h"
#include "InputManager.h"

class ScreenManager;

class GameOverScreen : public GameEntity {
private:
    Scoreboard* mPlayerScore;
    InputManager* mInput;

    int mCurrentScore;

public:
    GameOverScreen();
    ~GameOverScreen();

    void SetCurrentScore(int score);
    void Update() override;
    void Render() override;
};

#endif
