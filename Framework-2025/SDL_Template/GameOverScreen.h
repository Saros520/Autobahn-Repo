#ifndef __GAMEOVERSCREEN_H
#define __GAMEOVERSCREEN_H

#include "Texture.h"
#include "Scoreboard.h"
#include "InputManager.h"

class GameOverScreen : public GameEntity {
private:
    Texture* mNorthRoadBackground;
    Texture* mSouthRoadBackground;
    Texture* mBustedText;
    Scoreboard* mPlayerScore;
    Scoreboard* mHighScoreBoard;
    InputManager* mInput;
    int mCurrentScore;
    int mHighScore;

    Vector2 mBackgroundScaleFactor;

public:
    GameOverScreen(std::string northRoadSprite, std::string southRoadSprite);
    ~GameOverScreen();

    void SetBackground(std::string northRoadSprite, std::string southRoadSprite);
    void SetCurrentScore(int score);
    void Update() override;
    void Render() override;
};

#endif

