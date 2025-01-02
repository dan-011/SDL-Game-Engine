#pragma once
#include "VisualComponent.h"
#include "Color.h"

// maybe decouple shapes from visual shapes and use encapsulation
class Shape : public VisualComponent2D {
public:
	Shape(const Vector2f& pos, const Vector2f& dims, const Color& clr = Color(0x00, 0x00, 0x00, 0xFF), bool fill = true, int _layer = 0, bool visibility = true);
	virtual ~Shape();

	void SetColor(const Color& clr);
	Color GetColor();
	void SetAlpha(Uint8 alpha);

	void SetFill(bool fill);
	bool GetFilled();

private:
	Color color;
	bool filled;

};

// TODO: implement a point and a line as well, then create collisions for that