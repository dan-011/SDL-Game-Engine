#include "VisualComponent.h"
#define LAYER_ADJ(a)((a) < 0 ? 0 : ((a) >= MAX_LAYERS ? (MAX_LAYERS - 1) : (a)))

VisualComponent::VisualComponent(float x, float y, int _layer, bool visibility) : GameComponent(x, y), layer(LAYER_ADJ(_layer)), visible(visibility) {}
VisualComponent::VisualComponent(const Vector2f& position, int _layer, bool visibility) : GameComponent(position), layer(LAYER_ADJ(_layer)), visible(visibility) {}
VisualComponent::~VisualComponent() {}

void VisualComponent::SetLayer(int lyr) {
	layer = LAYER_ADJ(lyr);
}
int VisualComponent::GetLayer() {
	return layer;
}
void VisualComponent::SetVisibility(bool isVisible) {
	visible = isVisible;
}
bool VisualComponent::GetVisibility() {
	return visible;
}

VisualComponent2D::VisualComponent2D(const Vector2f& position, const Vector2f& dimensions, int _layer, bool visibility) : VisualComponent(position, _layer, visibility), dims(dimensions) {}
VisualComponent2D::~VisualComponent2D() {}

void VisualComponent2D::SetDims(float x, float y) {
	dims.x = x;
	dims.y = y;
}
void VisualComponent2D::SetDims(const Vector2f& dimensions) {
	dims = dimensions;
}
Vector2f VisualComponent2D::GetDims() {
	return dims;
}