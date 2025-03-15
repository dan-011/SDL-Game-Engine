#include "CircleShape.h"
#include "GameManager.h"

CircleShape::CircleShape(const Vector2f& centerPoint, float radius, const Color& clr, bool filled, int _layer, bool visibility) : Shape(centerPoint, Vector2f(radius, radius), clr, filled, _layer, visibility) {}
CircleShape::~CircleShape() {}

void CircleShape::SetCenterPoint(const Vector2f& centerPoint) {
	SetPos(centerPoint);
}
Vector2f CircleShape::GetCenterPoint() {
	return GetPos();
}

void CircleShape::SetRadius(float radius) {
	SetDims(radius, radius);
}
float CircleShape::GetRadius() {
	return GetDims().x;
}
bool CircleShape::PointInside(const Vector2f& pt) {
    float radius = GetRadius();
    return GetPos().SqDistance(pt) <= ((double)radius * radius);
}

void CircleShape::Render() {
    SDL_Renderer* renderer = GameManager::Instance().GetRenderer();
    Color color = GetColor();
    SDL_SetRenderDrawColor(renderer, color.R, color.G, color.B, color.A);

    if (GetFilled()) DrawFilledCircle(renderer);
    else DrawEmptyCircle(renderer);
}

void CircleShape::DrawFilledCircle(SDL_Renderer* renderer) {
    int x = (int)GetRadius();
    int y = 0;
    int decisionOver2 = 1 - x; // Decision criterion divided by 2 evaluated at (radius, 0)
        
    int centerX = (int)GetCenterPoint().x;
    int centerY = (int)GetCenterPoint().y;

    while (y <= x) {
        SDL_RenderDrawLine(renderer, centerX + x, centerY + y, centerX - x, centerY + y);
        SDL_RenderDrawLine(renderer, centerX + y, centerY + x, centerX - y, centerY + x);
        SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX + x, centerY - y);
        SDL_RenderDrawLine(renderer, centerX - y, centerY - x, centerX + y, centerY - x);
        
        y++;        
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1;
        }
        else {
            x--;
            decisionOver2 += 2 * (y - x) + 1;
        }
    }
}

void CircleShape::DrawEmptyCircle(SDL_Renderer* renderer) {
    int x = (int)GetRadius();
    int y = 0;
    int decisionOver2 = 1 - x; // Decision criterion divided by 2 evaluated at (radius, 0)

    int centerX = (int)GetCenterPoint().x;
    int centerY = (int)GetCenterPoint().y;

    while (y <= x) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y); // Octant 1
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x); // Octant 2
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y); // Octant 4
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x); // Octant 3
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y); // Octant 5
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x); // Octant 6
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y); // Octant 7
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x); // Octant 8
        y++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1;
        }
        else {
            x--;
            decisionOver2 += 2 * (y - x) + 1;
        }
    }
}

//SDL_RenderDrawLine