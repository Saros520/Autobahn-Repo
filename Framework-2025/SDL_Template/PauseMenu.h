/*#ifndef __PAUSEMENU_H
#define __PAUSEMENU_H

#include "GameEntity.h"
#include "Texture.h"
#include "InputManager.h"

using namespace SDLFramework;

class PauseMenu : public GameEntity {
private:
    InputManager* mInput;

    Texture* mBackgroundBox;
    Texture* mResumeText;
    Texture* mExitText;
    Texture* mCursor;

    Vector2 mCursorStartPos;
    Vector2 mCursorOffset;
    int mSelectedOption;

public:
    PauseMenu();
    ~PauseMenu();

    void ChangeOption(int change);
    int SelectedOption();
    void Update();
    void Render();
};

#endif*/

//commit
