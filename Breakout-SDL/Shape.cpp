#include "Shape.h"

Shape::Shape(const Vector2f& pos, const Vector2f& dims, const Color& clr, bool fill, int _layer, bool visibility) : VisualComponent2D(pos, dims, _layer, visibility), color(clr), filled(fill) {}
Shape::~Shape() {}

void Shape::SetColor(const Color& clr) {
	color = clr;
}
Color Shape::GetColor() {
	return color;
}
void Shape::SetAlpha(Uint8 alpha) {
	// TODO
}

void Shape::SetFill(bool fill) {
	filled = fill;
}
bool Shape::GetFilled() {
	return filled;
}