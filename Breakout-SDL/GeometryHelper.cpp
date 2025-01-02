#include "GeometryHelper.h"

float Determinant(const Vector2f& vectorA, const Vector2f& vectorB) {
    return (vectorA.x * vectorB.y) - (vectorB.x * vectorA.y);
}


bool LineSegment(const Vector2f& ptA, const Vector2f& ptB, float& m, float& b) {
    Vector2f diff = ptB - ptA;
    if (FEq(diff.x, 0)) return false;
    m = diff.y / diff.x;
    b = ptA.y - (m * ptA.x);
    return true;
}

bool LinesCollinear(const Vector2f& pt1A, const Vector2f& pt2A, const Vector2f& pt1B, const Vector2f& pt2B) {
    float m;
    float b;
    if (LineSegment(pt1A, pt2A, m, b)) return FEq(pt1B.y, (m * pt1B.x) + b);
    else return FEq(pt1A.x, pt1B.x);
}

bool LinesCollide(const Vector2f& pt1A, const Vector2f& pt2A, const Vector2f& pt1B, const Vector2f& pt2B) {
    // TODO: Understand this
    Vector2f dirVectorA = pt2A - pt1A;
    Vector2f dirVectorB = pt2B - pt1B;

    float den = Determinant(dirVectorA, dirVectorB);
    if (den == 0) return LinesCollinear(pt1A, pt2A, pt1B, pt2B);
    bool denPos = den > 0;

    Vector2f segVector = pt1A - pt1B;
    float s = Determinant(dirVectorA, segVector);
    if ((s < 0) == denPos) return false;

    float t = Determinant(dirVectorB, segVector);
    if ((t < 0) == denPos) return false;

    if (((s > den) == denPos) || (t > den) == denPos) return false;

    return true;
}