#include "PointShape.h"
#include "GameManager.h"

PointShape::PointShape(const Vector2f& pos, const Color& clr, int _layer, bool visibility) : Shape(pos, Vector2f(1, 1), clr, true, _layer, visibility) {}
PointShape::~PointShape() {}

void PointShape::Render() {
    SDL_Renderer* renderer = GameManager::Instance().GetRenderer();
    Color color = GetColor();
    Vector2f pos = GetPos();
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
    SDL_RenderDrawPoint(renderer, (int)pos.x, (int)pos.y);
}