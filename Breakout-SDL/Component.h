#pragma once
#include <SDL.h>

class Component {
public:
	Component() {}
	virtual ~Component() {}

	virtual void Construct() {}
	virtual void Update(Uint32 deltaTime) {}
};