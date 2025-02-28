#include "Scoreboard.h"
#include "Graphics.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>

Scoreboard::Scoreboard()
    : Scoreboard({ 230, 230, 230 }) {
    mHighScoreValue = 0.0f;
}

Scoreboard::Scoreboard(SDL_Color color) {
    mColor = color;
    mHighScoreValue = 0.0f;
}

Scoreboard::~Scoreboard() {
    ClearBoard();
}

void Scoreboard::Score(float score) {
    mScoreValue = score;
    UpdateScoreDisplay();
}

void Scoreboard::Distance(float distance) {
    mScoreValue = distance;
    UpdateScoreDisplay();
}

void Scoreboard::Update() {
    // If you need to update the score dynamically, you can add logic here
    // For example, you could check if the score has changed and update the display
}

void Scoreboard::Render() {
    for (int i = 0; i < mScore.size(); ++i) {
        mScore[i]->Render();
    }
}

void Scoreboard::ClearBoard() {
    for (int i = 0; i < mScore.size(); ++i) {
        delete mScore[i];
        mScore[i] = nullptr;
    }

    mScore.clear();
}

float Scoreboard::GetHighScore() const {
    return mHighScoreValue;
}

void Scoreboard::SetHighScore(float highScore) {
    mHighScoreValue = highScore;
}

void Scoreboard::UpdateScoreDisplay() {
    ClearBoard();

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << mScoreValue;
    std::string str = oss.str();

    for (int i = 0; i < str.length(); ++i) {
        Texture* texture = new Texture(str.substr(i, 1), "emulogic.ttf", 32, mColor);
        if (texture->IsValid()) {
            mScore.push_back(texture);
            mScore.back()->Parent(this);
            mScore.back()->Position(Vector2(-32.0f * (str.length() - i), 0.0f));
        }
        else {
            std::cerr << "Unable to load texture for character: " << str.substr(i, 1) << std::endl;
            delete texture;
        }
    }
}
