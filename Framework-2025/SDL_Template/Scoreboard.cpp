#include "Scoreboard.h"
#include "Graphics.h"
#include <iomanip>
#include <sstream>

Scoreboard::Scoreboard() 
	: Scoreboard({230, 230, 230}) {
}

Scoreboard::Scoreboard(SDL_Color color) {
	mColor = color;
}

Scoreboard::~Scoreboard() {
	ClearBoard();
}

void Scoreboard::Score(int score) {
	ClearBoard();

	std::string str = std::to_string(score);

	for (int i = 0; i < str.length(); ++i) {
		mScore.push_back(new Texture(str.substr(i, 1), "emulogic.ttf", 32, mColor));
		mScore[i]->Parent(this);
		mScore[i]->Position(Vector2(-32.0f * (str.length() - i), 0.0f));
	}
}

void Scoreboard::Distance(float distance) {
	ClearBoard();

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << distance << " km";
	std::string str = oss.str();

	for (int i = 0; i < str.length(); ++i) {
		mScore.push_back(new Texture(str.substr(i, 1), "emulogic.ttf", 32, mColor));
		mScore[i]->Parent(this);
		mScore[i]->Position(Vector2(-32.0f * (str.length() - i), 0.0f));
	}
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
