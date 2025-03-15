#include "CollisionManager.h"
#include <algorithm>
#include "MathHelper.h"
#include "Vector2f.h"
#include "GeometryHelper.h"

CollisionManager::CollisionManager() {}
CollisionManager::~CollisionManager() {}

bool CollisionManager::IsColliding(PointShape& shapeA, PointShape& shapeB) {
    return shapeA.GetPos() == shapeB.GetPos();
}
bool CollisionManager::IsColliding(PointShape& shapeA, LineShape& shapeB) {
    Vector2f ptA;
    Vector2f ptB;
    Vector2f ptC = shapeA.GetPos();
    shapeB.GetPoints(ptA, ptB);
    float area = 0.5f * ((ptA.x * (ptB.y - ptC.y)) + (ptB.x * (ptC.y - ptA.y)) + (ptC.x * (ptA.y - ptB.y)));
    if (area > FLOAT_PAD) return false;
    return
        std::min(ptA.x, ptB.x) <= ptC.x &&
        ptC.x <= std::max(ptA.x, ptB.x) &&
        std::min(ptA.y, ptB.y) <= ptC.y &&
        ptC.y <= std::max(ptA.y, ptB.y);
}
bool CollisionManager::IsColliding(PointShape& shapeA, RectangleShape& shapeB) {
    Vector2f ptA = shapeA.GetPos();
    Vector2f topLeft = shapeB.GetPos();
    Vector2f dims = shapeB.GetDims();
    Vector2f bottomRight(topLeft.x + dims.x, topLeft.y + dims.y);

    return
        topLeft.x <= ptA.x &&
        topLeft.y <= ptA.y &&
        ptA.x <= bottomRight.x &&
        ptA.y <= bottomRight.y;
}
bool CollisionManager::IsColliding(PointShape& shapeA, CircleShape& shapeB) {
    Vector2f ptA = shapeA.GetPos();
    Vector2f ptB = shapeB.GetCenterPoint();
    float radius = shapeB.GetRadius();
    return ptA.SqDistance(ptB) <= ((double)radius * radius);
}

// line
bool CollisionManager::IsColliding(LineShape& shapeA, LineShape& shapeB) {
    Vector2f pt1A;
    Vector2f pt2A;
    Vector2f pt1B;
    Vector2f pt2B;

    shapeA.GetPoints(pt1A, pt2A);
    shapeB.GetPoints(pt1B, pt2B);

    return LinesCollide(pt1A, pt2A, pt1B, pt2B);
}
bool CollisionManager::IsColliding(LineShape& shapeA, PointShape& shapeB) {
    return IsColliding(shapeB, shapeA);
}
bool CollisionManager::IsColliding(LineShape& shapeA, RectangleShape& shapeB) {
    Vector2f pt1;
    Vector2f pt2;
    shapeA.GetPoints(pt1, pt2);

    Vector2f topLeft = shapeB.GetPos();
    Vector2f dims = shapeB.GetDims();
    Vector2f bottomRight(topLeft.x + dims.x, topLeft.y + dims.y);
    Vector2f topRight(bottomRight.x, topLeft.y);
    Vector2f bottomLeft(topLeft.x, bottomRight.y);

    return
        LinesCollide(pt1, pt2, topLeft, topRight) ||
        LinesCollide(pt1, pt2, topRight, bottomRight) ||
        LinesCollide(pt1, pt2, bottomRight, bottomLeft) ||
        LinesCollide(pt1, pt2, bottomLeft, topLeft);
}

bool CollisionManager::IsColliding(LineShape& shapeA, CircleShape& shapeB) {
    Vector2f pt1;
    Vector2f pt2;
    shapeA.GetPoints(pt1, pt2);
    if (shapeB.PointInside(pt1) || shapeB.PointInside(pt2)) return true;

    Vector2f centerPoint = shapeB.GetCenterPoint();

    Vector2f lineVec = pt2 - pt1;
    Vector2f pointVec = centerPoint - pt1;

    double lineLengthSq = lineVec.Dot(lineVec);
    double t = Max(0.0, Min(1.0, pointVec.Dot(lineVec) / lineLengthSq));

    Vector2f closestPt(pt1.x + (float)(t * lineVec.x), pt1.y + (float)(t * lineVec.y));
    
    return shapeB.PointInside(closestPt);
}

// rectangle
bool CollisionManager::IsColliding(RectangleShape& shapeA, RectangleShape& shapeB) {
    Vector2f posA = shapeA.GetPos();
    Vector2f dimsA = shapeA.GetDims();
    Vector2f posB = shapeB.GetPos();
    Vector2f dimsB = shapeB.GetDims();

    float leftA = posA.x;
    float rightA = leftA + dimsA.x;
    float topA = posA.y;
    float bottomA = topA + dimsA.y;

    float leftB = posB.x;
    float rightB = leftB + dimsB.x;
    float topB = posB.y;
    float bottomB = topB + dimsB.y;

    return leftA < rightB&& rightA > leftB && topA < bottomB&& bottomA > topB;
}
bool CollisionManager::IsColliding(RectangleShape& shapeA, PointShape& shapeB) {
    return IsColliding(shapeB, shapeA);
}
bool CollisionManager::IsColliding(RectangleShape& shapeA, LineShape& shapeB) {
    return IsColliding(shapeB, shapeA);
}
bool CollisionManager::IsColliding(RectangleShape& shapeA, CircleShape& shapeB) {
    return IsColliding(shapeB, shapeA);
}

// Circle
bool CollisionManager::IsColliding(CircleShape& shapeA, CircleShape& shapeB) {
    float totalRadius = shapeA.GetRadius() + shapeB.GetRadius();

    Vector2f centerA = shapeA.GetCenterPoint();
    Vector2f centerB = shapeB.GetCenterPoint();

    return centerA.SqDistance(centerB) <= ((double)totalRadius * totalRadius);
}
bool CollisionManager::IsColliding(CircleShape& shapeA, PointShape& shapeB) {
    return IsColliding(shapeB, shapeA);
}
bool CollisionManager::IsColliding(CircleShape& shapeA, LineShape& shapeB) {
    return IsColliding(shapeB, shapeA);
}
bool CollisionManager::IsColliding(CircleShape& shapeA, RectangleShape& shapeB) {
    Vector2f closestPoint(0, 0);

    Vector2f circlePoint = shapeA.GetCenterPoint();
    float circleRadius = shapeA.GetRadius();
    Vector2f rectPoint = shapeB.GetPos();
    Vector2f rectDims = shapeB.GetDims();

    if (circlePoint.x < rectPoint.x) closestPoint.x = rectPoint.x;
    else if (circlePoint.x > rectPoint.x + rectDims.x - 1) closestPoint.x = rectPoint.x + rectDims.x - 1;
    else closestPoint.x = closestPoint.x = circlePoint.x;

    if (circlePoint.y < rectPoint.y) closestPoint.y = rectPoint.y;
    else if (circlePoint.y > rectPoint.y + rectDims.y - 1) closestPoint.y = rectPoint.y + rectDims.y - 1;
    else closestPoint.y = circlePoint.y;

    return circlePoint.SqDistance(closestPoint) < ((double)circleRadius * circleRadius);
}
