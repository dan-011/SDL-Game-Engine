#pragma once
#include "MathHelper.h"
#include "Vector2f.h"

float Determinant(const Vector2f& vectorA, const Vector2f& vectorB);

bool LineSegment(const Vector2f& ptA, const Vector2f& ptB, float& m, float& b);

bool LinesCollinear(const Vector2f& pt1A, const Vector2f& pt2A, const Vector2f& pt1B, const Vector2f& pt2B);

bool LinesCollide(const Vector2f& pt1A, const Vector2f& pt2A, const Vector2f& pt1B, const Vector2f& pt2B);