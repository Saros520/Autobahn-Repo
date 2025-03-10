#ifndef __SPIKESTRIP_H
#define __SPIKESTRIP_H

#include "Texture.h"
#include "PhysEntity.h"
#include "Timer.h"

class SpikeStrip : public PhysEntity {
private:
	Timer* mTimer;
	Texture* mTexture;

public:
	SpikeStrip();
	~SpikeStrip();

	void Update() override;
	void Render() override;
	bool IsOutOfBounds();

};

#endif
