#include "LineShape.h"
#include "GameManager.h"

LineShape::LineShape(const Vector2f& ptA, const Vector2f& ptB, const Color& clr, int _layer, bool visibility) : Shape(ptA, Vector2f(0, 0), clr, true, _layer, visibility), pointB(ptB) {}
LineShape::LineShape(float xA, float yA, float xB, float yB, const Color& clr, int _layer, bool visibility) : Shape(Vector2f(xA, xB), Vector2f(0, 0), clr, true, _layer, visibility), pointB(xB, yB) {}
LineShape::~LineShape() {}

void LineShape::SetPointA(const Vector2f& ptA) {
	SetPos(ptA);
}
void LineShape::SetPointA(float x, float y) {
	SetPos(x, y);
}
void LineShape::SetPointB(const Vector2f& ptB) {
	pointB = ptB;
}
void LineShape::SetPointB(float x, float y) {
	pointB = Vector2f(x, y);
}

void LineShape::SetPoints(const Vector2f& ptA, const Vector2f& ptB) {
	SetPos(ptA);
	pointB = ptB;
}
void LineShape::SetPoints(float xA, float yA, float xB, float yB) {
	SetPos(xA, yA);
	pointB = Vector2f(xB, yB);
}

void LineShape::GetPoints(Vector2f& destPtA, Vector2f& destPtB) const {
	destPtA = GetPos();
	destPtB = pointB;
}

void LineShape::Render() {
	SDL_Renderer* renderer = GameManager::Instance().GetRenderer();
	Color color = GetColor();
	Vector2f pointA = GetPos();
	SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);
	SDL_RenderDrawLine(renderer, (int)pointA.x, (int)pointA.y, (int)pointB.x, (int)pointB.y);
}