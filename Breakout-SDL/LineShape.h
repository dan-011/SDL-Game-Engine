#pragma once
#include "Shape.h"

class LineShape : public Shape {
public:
	LineShape(const Vector2f& ptA, const Vector2f& ptB, const Color& clr = Color(0x00, 0x00, 0x00, 0xFF), int _layer = 0, bool visibility = true);
	LineShape(float xA, float yA, float xB, float yB, const Color& clr = Color(0x00, 0x00, 0x00, 0xFF), int _layer = 0, bool visibility = true);
	virtual ~LineShape();

	void SetPointA(const Vector2f& ptA);
	void SetPointA(float x, float y);
	void SetPointB(const Vector2f& ptB);
	void SetPointB(float x, float y);
	
	void SetPoints(const Vector2f& ptA, const Vector2f& ptB);
	void SetPoints(float xA, float yA, float xB, float yB);

	void GetPoints(Vector2f& destPtA, Vector2f& destPtB) const;

	virtual void Render() override;

private:
	Vector2f pointB;

};