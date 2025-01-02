#include "MovingBody.h"

MovingBody::MovingBody(const Vector2f& position, const Vector2f& velocity, const Vector2f& goalVelocity, const Vector2i& interpolation) : GameComponent(position), vel(velocity), goalVel(goalVelocity), interpolationVals(interpolation) {}
MovingBody::~MovingBody() {}

void MovingBody::Update(Uint32 deltaTime) {
	UpdateVelocity(deltaTime);
	UpdatePosition(deltaTime);
}
void MovingBody::UpdatePosition(Uint32 deltaTime) {
	Vector2f pos = GetPos();
	pos.x += vel.x * deltaTime;
	pos.y += vel.y * deltaTime;
	SetPos(pos);

}
void MovingBody::UpdateXVelInterpolated(Uint32 deltaTime) {
	vel.x = Interpolate(goalVel.x, vel.x, deltaTime * interpolationVals.x);
}
void MovingBody::UpdateYVelInterpolated(Uint32 deltaTime) {
	vel.y = Interpolate(goalVel.y, vel.y, deltaTime * interpolationVals.y);
}
void MovingBody::UpdateVelocity(Uint32 deltaTime) {
	if (interpolationVals.x) UpdateXVelInterpolated(deltaTime);
	if (interpolationVals.y) UpdateYVelInterpolated(deltaTime);
}
void MovingBody::HitGoalVelocity(const Vector2i& modifier) {
	vel.x = goalVel.x * modifier.x;
	vel.y = goalVel.y * modifier.y;
}

Vector2f MovingBody::GetVel() { return vel; }
void MovingBody::SetVel(const Vector2f& velocity) { vel = velocity; }
void MovingBody::SetVel(float xVel, float yVel) {
	vel.x = xVel;
	vel.y = yVel;
}
void MovingBody::SetXVel(float val) { vel.x = val; }
void MovingBody::SetYVel(float val) { vel.y = val; }

Vector2f MovingBody::GetGoalVel() { return goalVel; }
void MovingBody::SetGoalVel(const Vector2f& velocity) { goalVel = velocity; }
void MovingBody::SetGoalVel(float xVel, float yVel) {
	goalVel.x = xVel;
	goalVel.y = yVel;
}
void MovingBody::ModifyVelocity(const Vector2i& modifier) {
	vel.x *= modifier.x;
	vel.y *= modifier.y;
}

Vector2i MovingBody::GetInterpolationVals() { return interpolationVals; }
void MovingBody::SetInterpolationVals(const Vector2i& interpolation) { interpolationVals = interpolation; }
void MovingBody::ActivateInterpolation(const Vector2i& interpolation) { SetInterpolationVals(interpolation); }
void MovingBody::DeactivateInterpolation() {
	interpolationVals.x = 0;
	interpolationVals.y = 0;
}
void MovingBody::ActivateXInterpolation(int val) { interpolationVals.x = val; }
void MovingBody::DeactivateXInterpolation() { interpolationVals.x = 0; }
void MovingBody::ActivateYInterpolation(int val) { interpolationVals.y = val; }
void MovingBody::DeactivateYInterpolation() { interpolationVals.y = 0; }
