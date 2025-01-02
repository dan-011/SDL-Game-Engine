#include "GameComponent.h"

GameComponent::GameComponent(float x, float y) : pos(x, y) {}
GameComponent::GameComponent(const Vector2f& position) : pos(position) {}
GameComponent::~GameComponent() {}

Vector2f GameComponent::GetPos() { return pos; }
void GameComponent::SetPos(float x, float y) {
	pos.x = x;
	pos.y = y;
}
void GameComponent::SetPos(const Vector2f& position) {
	pos = position;
}