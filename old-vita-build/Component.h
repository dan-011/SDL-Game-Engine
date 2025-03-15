#pragma once
#include <SDL2/SDL.h>

class Component {
public:
	Component() {}
	virtual ~Component() {}

	virtual void Construct() {}
	virtual void Update(Uint32 deltaTime) {}
};