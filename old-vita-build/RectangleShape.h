#pragma once
#include "Shape.h"
#include "Color.h"

class RectangleShape : public Shape {
public:
	RectangleShape(const Vector2f& pos, const Vector2f& dimensions, const Color& clr = Color(0x00, 0x00, 0x00, 0xFF), bool fill = true, int layer = 0, bool visibility = true);
	virtual ~RectangleShape();

	void SetPos(float x, float y) override;
	void SetPos(const Vector2f& position) override;

	void SetDims(const Vector2f& dimensions) override;
	

	virtual void Render() override;

private:
	SDL_Rect rect;
};

