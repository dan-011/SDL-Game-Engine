#pragma once
#include "Component.h"
#include "Vector2f.h"

class GameComponent : public Component {
public:
	GameComponent(float x, float y);
	GameComponent(const Vector2f& position);
	virtual ~GameComponent();

	Vector2f GetPos();
	virtual void SetPos(float x, float y);
	virtual void SetPos(const Vector2f& position);

private:
	Vector2f pos;
};