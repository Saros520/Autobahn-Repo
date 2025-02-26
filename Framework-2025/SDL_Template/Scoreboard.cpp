#include "Scoreboard.h"
#include "Graphics.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>

Scoreboard::Scoreboard()
    : Scoreboard({ 230, 230, 230 }) {
    LoadHighScore();
}

Scoreboard::Scoreboard(SDL_Color color) {
    mColor = color;
    LoadHighScore();
}

Scoreboard::~Scoreboard() {
    ClearBoard();
}

void Scoreboard::Score(int score) {
    ClearBoard();

    std::string str = std::to_string(score);

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

    if (score > mHighScoreValue) {
        mHighScoreValue = score;
        SaveHighScore();
    }
}

void Scoreboard::Distance(float distance) {
    ClearBoard();

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << distance << " km";
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

void Scoreboard::Update() {
    
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

void Scoreboard::SaveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << mHighScoreValue;
    }
    file.close();
}

void Scoreboard::LoadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> mHighScoreValue;
    }
    else {
        mHighScoreValue = 0;
    }
    file.close();
}

int Scoreboard::GetHighScore() {
    return mHighScoreValue;
}
