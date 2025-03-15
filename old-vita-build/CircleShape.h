#pragma once
#include "Shape.h"

class CircleShape : public Shape {
public:
	CircleShape(const Vector2f& centerPoint, float radius, const Color& clr = Color(0x00, 0x00, 0x00), bool filled = true, int _layer = 0, bool visibility = true);
	virtual ~CircleShape();

	void SetCenterPoint(const Vector2f& centerPoint);
	Vector2f GetCenterPoint();
	void SetRadius(float radius);
	float GetRadius();
	bool PointInside(const Vector2f& pt);

	virtual void Render() override;

private:
	void DrawFilledCircle(SDL_Renderer* renderer);
	void DrawEmptyCircle(SDL_Renderer* renderer);

};