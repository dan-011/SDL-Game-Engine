#pragma once
#include "Shape.h"


class PointShape : public Shape {
public:
	PointShape(const Vector2f& pos, const Color& clr = Color(0x00, 0x00, 0x00, 0xFF), int _layer = 0, bool visibility = true);
	virtual ~PointShape();

	virtual void Render() override;
};