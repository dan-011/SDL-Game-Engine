#pragma once
#include "Shape.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "PointShape.h"
#include "LineShape.h"

class CollisionManager {
public:
	CollisionManager();
	~CollisionManager();

	// point
	bool IsColliding(PointShape& shapeA, PointShape& shapeB);
	bool IsColliding(PointShape& shapeA, LineShape& shapeB);
	bool IsColliding(PointShape& shapeA, RectangleShape& shapeB);
	bool IsColliding(PointShape& shapeA, CircleShape& shapeB);

	// line
	bool IsColliding(LineShape& shapeA, LineShape& shapeB);
	bool IsColliding(LineShape& shapeA, PointShape& shapeB);
	bool IsColliding(LineShape& shapeA, RectangleShape& shapeB);

	// TODO FIX BUG
	bool IsColliding(LineShape& shapeA, CircleShape& shapeB);

	// rectangle
	bool IsColliding(RectangleShape& shapeA, RectangleShape& shapeB);
	bool IsColliding(RectangleShape& shapeA, PointShape& shapeB);
	bool IsColliding(RectangleShape& shapeA, LineShape& shapeB);
	bool IsColliding(RectangleShape& shapeA, CircleShape& shapeB);

	// Circle
	bool IsColliding(CircleShape& shapeA, CircleShape& shapeB);
	bool IsColliding(CircleShape& shapeA, PointShape& shapeB);
	bool IsColliding(CircleShape& shapeA, LineShape& shapeB);
	bool IsColliding(CircleShape& shapeA, RectangleShape& shapeB);

};