#pragma once
#include "Scene.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "PointShape.h"
#include "LineShape.h"
#include "MovingBody.h"
#include "GeometryHelper.h"

class RedRectangleScene : public Component, public Scene {
public:
	RedRectangleScene() : circle(NULL), rect(NULL), point(NULL), line(NULL), body(NULL), mouseLine(NULL), closePoint(NULL) {
		circle = new CircleShape(Vector2f(50, 50), 25, Color(0x00, 0xFF, 0x00));
		rect = new RectangleShape(Vector2f(500, 300), Vector2f(50, 50), Color(0, 0xFF, 0));
		point = new PointShape(Vector2f(0, 0), Color(0x00, 0x00, 0xFF), 2);
		line = new LineShape(Vector2f(0, 0), Vector2f(0, 0), Color(0xFF, 0xFF, 0x00), 1);
		body = new MovingBody(circle->GetPos(), Vector2f(0, 0), Vector2f(vel, vel));
		mouseLine = new LineShape(Vector2f(SCREEN_WIDTH, 0), Vector2f(0, 0), Color(0xFF, 0x00, 0xFF));
		closePoint = new PointShape(Vector2f(0, 0), Color(0xFF, 0xFF, 0xFF), 2);
		AddComponent(circle);
		AddComponent(rect);
		AddComponent(point);
		AddComponent(line);
		AddComponent(body);
		AddComponent(mouseLine);
		AddComponent(closePoint);
		AddComponent(this);

		Color clr(0x00, 0x00, 0x00);
		SetBackgroundColor(clr);
	}
	~RedRectangleScene() {
		delete circle;
		delete rect;
		delete point;
		delete line;
		delete body;
		delete mouseLine;
		delete closePoint;
	}


	virtual void Update(Uint32 deltaTime) override {
		Move();
		HandleCollision();
		SetClosestPoint();
		SetLine();
	}

	void SetClosestPoint() {
		Vector2f closestPoint(0, 0);

		Vector2f circlePoint = circle->GetCenterPoint();
		float circleRadius = circle->GetRadius();
		Vector2f rectPoint = rect->GetPos();
		Vector2f rectDims = rect->GetDims();

		if (circlePoint.x < rectPoint.x) closestPoint.x = rectPoint.x;
		else if (circlePoint.x > rectPoint.x + rectDims.x - 1) closestPoint.x = rectPoint.x + rectDims.x - 1;
		else closestPoint.x = closestPoint.x = circlePoint.x;

		if (circlePoint.y < rectPoint.y) closestPoint.y = rectPoint.y;
		else if (circlePoint.y > rectPoint.y + rectDims.y - 1) closestPoint.y = rectPoint.y + rectDims.y - 1;
		else closestPoint.y = circlePoint.y;

		point->SetPos(closestPoint);
	}

	void SetLine() {
		line->SetPointB(point->GetPos());
	}

	virtual void NotifyEvent(SDL_Event* e) override {
		if (e->type == SDL_KEYDOWN) {
			KeyDown(e->key.keysym.sym);
		}
		else if (e->type == SDL_KEYUP) {
			KeyUp(e->key.keysym.sym);
		}
		else if (e->type == SDL_MOUSEMOTION) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			mouseLine->SetPointB(Vector2f((float)x, (float)y));
		}
	}

	void KeyDown(SDL_Keycode down) {
		Vector2f vel = body->GetVel();
		Vector2i dir(vel.x ? (vel.x < 0 ? -1 : 1) : 0, vel.y ? (vel.y < 0 ? -1 : 1) : 0);
		switch (down) {
			case SDLK_UP:
				dir.y = -1;
				break;
			case SDLK_DOWN:
				dir.y = 1;
				break;
			case SDLK_LEFT:
				dir.x = -1;
				break;
			case SDLK_RIGHT:
				dir.x = 1;
				break;
		}
		body->HitGoalVelocity(dir);
	}

	void KeyUp(SDL_Keycode up) {
		Vector2i modifier(1, 1);
		switch (up) {
		case SDLK_UP:
			modifier.y = 0;
			break;
		case SDLK_DOWN:
			modifier.y = 0;
			break;
		case SDLK_LEFT:
			modifier.x = 0;
			break;
		case SDLK_RIGHT:
			modifier.x = 0;
			break;
		}
		body->ModifyVelocity(modifier);
	}

	void Move() {
		circle->SetPos(body->GetPos());
	}

	void HandleCollision() {
		if (collisionManager.IsColliding(*circle, *rect)) {
			Color red(0xFF, 0x00, 0x00);
			circle->SetColor(red);
			rect->SetColor(red);
		}
		else {
			Color green(0x00, 0xFF, 0x00);
			circle->SetColor(green);
			rect->SetColor(green);
		}

		if (collisionManager.IsColliding(*circle, *line)) {
			line->SetColor(Color(0x00, 0x00, 0x00));
		}
		else line->SetColor(Color(0xFF, 0xFF, 0x00));

		if (collisionManager.IsColliding(*line, *mouseLine)) {
			mouseLine->SetColor(Color(0xFF, 0x00, 0x00));
		}
		else mouseLine->SetColor(Color(0xFF, 0x00, 0xFF));
	}

	virtual void NotifyKeystate(const Uint8* keys) override {
		/*if (keys[SDL_SCANCODE_UP]) dir.y = -1;
		else if (keys[SDL_SCANCODE_DOWN]) dir.y = 1;
		else dir.y = 0;
		if (keys[SDL_SCANCODE_RIGHT]) dir.x = 1;
		else if (keys[SDL_SCANCODE_LEFT]) dir.x = -1;
		else dir.x = 0;*/

		//std::cout << dir.x << ',' << dir.y << std::endl;
	}


private:
	CircleShape* circle;
	RectangleShape* rect;
	PointShape* point;
	LineShape* line;
	LineShape* mouseLine;
	PointShape* closePoint;
	MovingBody* body;
	float vel = 0.25;
};