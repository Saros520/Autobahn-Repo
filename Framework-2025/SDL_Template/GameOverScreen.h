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
    Texture* mYouTraveled;
	Texture* mHighScoreLabel;
    Scoreboard* mPlayerScore;
    Scoreboard* mHighScoreBoard;
    InputManager* mInput;
    // Bottom Bar Entities
    GameEntity* mBottomBar;
    Texture* mCreator;
    Texture* mTopBorder;
    Texture* mCreatorName;
    Texture* mCreatorName2;
    Texture* mCreatorName3;
    Texture* mBottomBorder;
    Texture* mMainMenu2;
    Texture* mMainMenu3;
    GameEntity* mTopBar;
    Texture* mGameOver2;
    int mCurrentScore;
    int mHighScore;

    Vector2 mBackgroundScaleFactor;

public:
    GameOverScreen(std::string northRoadSprite, std::string southRoadSprite, float currentScore);
    ~GameOverScreen();

    void SetBackground(std::string northRoadSprite, std::string southRoadSprite);
    void Update() override;
    void Render() override;
};

#endif
