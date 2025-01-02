#pragma once
#include "GameComponent.h"
#include "Vector2i.h"
#include <SDL.h>

class MovingBody : public GameComponent {
public:
	MovingBody(const Vector2f& position, const Vector2f& velocity = Vector2f(0, 0), const Vector2f& goalVelocity = Vector2f(0, 0), const Vector2i& interpolation = Vector2i(0, 0));
	virtual ~MovingBody();

	virtual void Update(Uint32 deltaTime) override;
	void UpdatePosition(Uint32 deltaTime);
	void UpdateXVelInterpolated(Uint32 deltaTime);
	void UpdateYVelInterpolated(Uint32 deltaTime);
	void UpdateVelocity(Uint32 deltaTime);
	void HitGoalVelocity(const Vector2i& modifier);

	Vector2f GetVel();
	void SetVel(const Vector2f& velocity);
	void SetVel(float xVel, float yVel);
	void SetXVel(float val);
	void SetYVel(float val);
	void ModifyVelocity(const Vector2i& modifier);

	Vector2f GetGoalVel();
	void SetGoalVel(const Vector2f& velocity);
	void SetGoalVel(float xVel, float yVel);

	Vector2i GetInterpolationVals();
	void SetInterpolationVals(const Vector2i& interpolation);
	void ActivateInterpolation(const Vector2i& interpolation);
	void DeactivateInterpolation();
	void ActivateXInterpolation(int val);
	void DeactivateXInterpolation();
	void ActivateYInterpolation(int val);
	void DeactivateYInterpolation();

private:
	Vector2f vel;
	Vector2f goalVel;
	Vector2i interpolationVals;
};