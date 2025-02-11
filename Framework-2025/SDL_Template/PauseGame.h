#ifndef __PAUSEGAME_H
#define __PAUSEGAME_H

#include "GameEntity.h"
#include "Texture.h"
#include "InputManager.h"

using namespace SDLFramework;

class PauseGame : public GameEntity {
private:
    InputManager* mInput;

    Texture* mBackgroundBox;  // The TransparentBox texture
    Texture* mResumeText;
    Texture* mExitText;
    Texture* mCursor;

    Vector2 mCursorStartPos;
    Vector2 mCursorOffset;
    int mSelectedOption;

public:
    PauseGame();
    ~PauseGame();

    void ChangeOption(int change);
    int SelectedOption();
    void Update();
    void Render();
};

#endif


