#pragma once
#include "RectangleShape.h"
#include "GameManager.h"

RectangleShape::RectangleShape(const Vector2f& pos, const Vector2f& dimensions, const Color& clr, bool fill, int layer, bool visibility) : Shape(pos, dimensions, clr, fill, layer, visibility) {
	rect = { (int)pos.x, (int)pos.y, (int)dimensions.x, (int)dimensions.y };
}
RectangleShape::~RectangleShape() {}

void RectangleShape::SetPos(float x, float y) {
	VisualComponent::SetPos(x, y);
	rect.x = (int)x;
	rect.y = (int)y;
}
void RectangleShape::SetPos(const Vector2f& position) {
	VisualComponent::SetPos(position);
	rect.x = (int)position.x;
	rect.y = (int)position.y;
}

void RectangleShape::SetDims(const Vector2f& dimensions) {
	VisualComponent2D::SetDims(dimensions);
	rect.w = (int)dimensions.x;
	rect.h = (int)dimensions.y;
}

void RectangleShape::Render() {
	SDL_Renderer* renderer = GameManager::Instance().GetRenderer();
	Color color = GetColor();
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
	
	if (GetFilled()) SDL_RenderFillRect(renderer, &rect);
	else SDL_RenderDrawRect(renderer, &rect);
}