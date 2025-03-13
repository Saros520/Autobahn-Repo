#ifndef __SCOREBOARD_H
#define __SCOREBOARD_H

#include "Texture.h"
#include <vector>
#include <string>

using namespace SDLFramework;

class Scoreboard : public GameEntity {
private:
	std::vector<Texture *> mScore;
	int mScoreValue;
	int mHighScoreValue;
	float mDistanceValue;

	SDL_Color mColor;

public:
	Scoreboard();
	Scoreboard(SDL_Color color);
	~Scoreboard();

	void Score(float score);
	void Distance(float distance);

	void Update() override;
	void Render() override;

	float GetHighScore() const;
	void SetHighScore(float highScore);

private:
	void ClearBoard();
	void UpdateScoreDisplay();

};
#endif
