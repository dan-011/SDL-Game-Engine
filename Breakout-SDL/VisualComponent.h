#pragma once
#include "GameComponent.h"

#define MAX_LAYERS 16
// Has a position, layer, and supports Render
class VisualComponent : public GameComponent {
public:
	VisualComponent(float x, float y, int _layer = 0, bool visibility = true);
	VisualComponent(const Vector2f& position, int _layer = 0, bool visibility = true);
	virtual ~VisualComponent();

	void SetLayer(int lyr);
	int GetLayer();
	void SetVisibility(bool isVisible);
	bool GetVisibility();

	virtual void Render() = 0;

protected:
	int layer;
	bool visible;

};

class VisualComponent2D : public VisualComponent {
public:
	VisualComponent2D(const Vector2f& position, const Vector2f& dimensions, int _layer = 0, bool visibility = true);
	virtual ~VisualComponent2D();

	virtual void SetDims(float x, float y);
	virtual void SetDims(const Vector2f& dimensions);
	virtual Vector2f GetDims() const;

private:
	Vector2f dims;

};