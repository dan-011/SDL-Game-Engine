#pragma once
#include "CircleShape.h"
#include "MovingBody.h"

class ParticlesComponent : public CircleShape {
public:
	ParticlesComponent(int numParticles, int minLife, int minRadius);
	virtual ~ParticlesComponent();

private:


};