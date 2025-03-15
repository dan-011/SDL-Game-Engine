#pragma once
#include "Scene.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "MovingBody.h"
#include "DoublyLL.h"
#include <vector>

class Breakout : public Component, public Scene {
public:
	Breakout() : ball(NULL), paddle(NULL), ballMotion(NULL), paddleMotion(NULL), paddleLine(NULL), paddlePadding(NULL), started(false), prev(0), cur(0), holdingStart(false) {
		paddle = new RectangleShape(Vector2f((SCREEN_WIDTH / 2) - 50, SCREEN_HEIGHT - 60), Vector2f(100, 20), Color(0x00, 0xFF, 0x00));
		paddleMotion = new MovingBody(paddle->GetPos(), Vector2f(0, 0), Vector2f(1, 0));
		const float radius = 20;
		ball = new CircleShape(Vector2f(paddle->GetPos().x + (paddle->GetDims().x / 2), paddle->GetPos().y - radius), radius, Color(0xFF, 0xFF, 0xFF), true, 1);
		ballMotion = new MovingBody(ball->GetCenterPoint());
		paddleLine = new LineShape(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y), Color(0xFF, 0x00, 0xFF), 0, true);
		paddlePadding = new RectangleShape(paddle->GetPos(), Vector2f(paddle->GetDims().x, paddle->GetDims().y * 0.2), Color(0xFF, 0x00, 0xFF), true);
		AddComponent(paddle);
		AddComponent(paddleMotion);
		AddComponent(ball);
		AddComponent(ballMotion);
		AddComponent(paddleLine);
		AddComponent(paddlePadding);
		BuildBlocks();
		AddComponent(this);

		SetBackgroundColor(Color(0x00, 0x00, 0x00));
		DisableKeys();
	}

	~Breakout() {
		delete paddle;
		paddle = NULL;
		delete paddleMotion;
		paddleMotion = NULL;
		delete ball;
		ball = NULL;
		delete ballMotion;
		ballMotion = NULL;
		delete paddleLine;
		paddleLine = NULL;
		delete paddlePadding;
		paddlePadding = NULL;

		DeleteBlocks();
	}

	void Reset() {
		started = false;
		prev = 0;
		cur = 0;
		DeleteBlocks();
		paddle->SetPos(Vector2f((SCREEN_WIDTH / 2) - 50, SCREEN_HEIGHT - 60));
		paddleMotion->SetPos(paddle->GetPos());
		ball->SetPos(Vector2f(paddle->GetPos().x + (paddle->GetDims().x / 2), paddle->GetPos().y - ball->GetRadius()));
		ballMotion->SetPos(ball->GetPos());
		paddleLine->SetPoints(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y));
		paddlePadding->SetPos(paddle->GetPos());

		paddleMotion->SetVel(Vector2f(0, 0));
		ballMotion->SetVel(Vector2f(0, 0));

		BuildBlocks();
	}

	void DeleteBlock(RectangleShape* block) {
		RemoveComponent(block);
		blocks.Erase(block);
		delete block;
	}

	void DeleteBlocks() {
		RectangleShape* block;
		while (!blocks.Empty()) {
			block = blocks.GetTail();
			blocks.PopBack();
			RemoveComponent(block);
			delete block;
		}
	}

	void BuildBlocks() {
		float x = 15;
		float y = 10;
		float w = 50;
		float h = 20;
		for (int i = 0; i < 5; i++) {
			if (i & 1) x += 10;
			else x -= 10;
			int offset = 0;
			while (x + offset + w < SCREEN_WIDTH) {
				blocks.PushBack(new RectangleShape(Vector2f(x + offset, y), Vector2f(w, h), Color(0xFF, 0x00, 0x00), true));
				offset += 60;
				AddComponent(blocks.GetTail());
			}
			y += h + 10;
		}
	}

	virtual void Update(Uint32 deltaTime) override {
		InputHandling();
		BoundsCheck();
		BallPaddleCollision();
		BallBlockCollisions();
		paddle->SetPos(paddleMotion->GetPos()); // Maybe make an object that has a visual component and does this automatically
		ball->SetPos(ballMotion->GetPos());
		paddleLine->SetPoints(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y));
		paddlePadding->SetPos(paddle->GetPos());
	}

	void BeginGame() {
		started = true;
		ballMotion->SetVel(0.5, -0.5);
	}

	void InputHandling() {
		SceCtrlData* ctrl = GetCtrlData();
		if(ctrl->buttons & SCE_CTRL_START) {
			if(!holdingStart) {
				holdingStart = true;
				if(started) Reset();
				else BeginGame();
				return;
			}
		}
		else holdingStart = false;
		if(!started) return;
		
		bool right = (ctrl->buttons & SCE_CTRL_RIGHT) || (ctrl->lx >= 160);
		bool left = (ctrl->buttons & SCE_CTRL_LEFT) || (ctrl->lx <= 70);
		if(right && left) {
			if(cur == 0) {
				cur = 'r';
				prev = 'l';
			}
			else if(prev == 0) {
				if(cur == 'l') {
					prev = cur;
					cur = 'r';
				}
				else {
					prev = cur;
					cur = 'l';
				}
			}
		}
		else if(right) {
			prev = 0;
			cur = 'r';
		}
		else if(left) {
			prev = 0;
			cur = 'l';
		}
		else {
			prev = 0;
			cur = 0;
		}
		int dir = 0;
		switch (cur) {
			case 'l':
				dir = -1;
				break;
			case 'r':
				dir = 1;
		}
		paddleMotion->HitGoalVelocity(Vector2i(dir, 0));
	}

	virtual void NotifyEvent(SDL_Event* e) override {

		if (!started) {
			if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_s) {
				started = true;
				ballMotion->SetVel(0.5, -0.5);
				return;
			}
			else return;
		}
		switch (e->type) {
			case SDL_KEYUP:
				KeyUp();
				break;
			case SDL_KEYDOWN:
				KeyDown(e->key.keysym.sym);
				break;
		}
	}

	void KeyDown(SDL_Keycode key) {
		// updates the direction for the most recent keypress
		int dir = 0;
		switch (key) {
			case SDLK_RIGHT:
				dir = 1;
				break;
			case SDLK_LEFT:
				dir = -1;
				break;
			case SDLK_r:
				Reset();
				break;
		}
		paddleMotion->HitGoalVelocity(Vector2i(dir, 0));
	}

	void KeyUp() {
		// resets the motion but checks if any other keys are held down
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		int dir = 0;
		if (keys[SDL_SCANCODE_RIGHT]) dir = 1;
		else if (keys[SDL_SCANCODE_LEFT]) dir = -1;
		paddleMotion->HitGoalVelocity(Vector2i(dir, 0));
	}
	
	void BoundsCheck() {
		Vector2f paddlePos = paddleMotion->GetPos();
		float paddleW = paddle->GetDims().x;
		if (paddlePos.x + paddleW > SCREEN_WIDTH) {
			paddleMotion->SetPos(SCREEN_WIDTH - paddleW, paddlePos.y);
		}
		if (paddlePos.x < 0) paddleMotion->SetPos(0, paddlePos.y);

		Vector2f ballPos = ballMotion->GetPos();
		float radius = ball->GetRadius();
		Vector2f ballVel = ballMotion->GetVel();
		if (ballPos.x + radius > SCREEN_WIDTH || ballPos.x - radius < 0) {
			if(ballPos.x + radius > SCREEN_WIDTH) ballMotion->SetPos(SCREEN_WIDTH - radius, ballPos.y);
			else ballMotion->SetPos(radius, ballPos.y);
			ballMotion->SetXVel(FlipSign(ballVel.x));
		}
		if (ballPos.y - radius < 0) {
			ballMotion->SetPos(ballPos.x, radius);
			ballMotion->SetYVel(FlipSign(ballVel.y));
		}
		if (ballPos.y > SCREEN_HEIGHT + radius) {
			ballMotion->HitGoalVelocity(Vector2i(0, 0));
		}
	}

	void BallPaddleCollision() {
		if (collisionManager.IsColliding(*ball, *paddleLine)) {
			ballMotion->SetPos(ballMotion->GetPos().x, paddleMotion->GetPos().y - ball->GetRadius() - 1);
			ballMotion->SetYVel(FlipSign(ballMotion->GetVel().y));
		}
	}

	Vector2i GetCollisionDirection(RectangleShape& block) {
		Vector2f topLeft = block.GetPos();
		Vector2f dims = block.GetDims();
		Vector2f bottomRight(topLeft.x + dims.x, topLeft.y + dims.y);

		LineShape* side = new LineShape(topLeft, Vector2f(bottomRight.x, topLeft.y)); // top
		if (collisionManager.IsColliding(*ball, *side)) {
			delete side;
			return Vector2i(0, 1);
		}
		delete side;
		side = new LineShape(Vector2f(bottomRight.x, topLeft.y), bottomRight); // right
		if (collisionManager.IsColliding(*ball, *side)) {
			delete side;
			return Vector2i(1, 0);
		}
		delete side;
		side = new LineShape(bottomRight, Vector2f(topLeft.x, bottomRight.y)); // bottom
		if (collisionManager.IsColliding(*ball, *side)) {
			delete side;
			return Vector2i(0, -1);
		}
		delete side;
		side = new LineShape(Vector2f(topLeft.x, bottomRight.y), topLeft); // left
		if (collisionManager.IsColliding(*ball, *side)) {
			delete side;
			return Vector2i(-1, 0);
		}
		delete side;
		return Vector2i(0, 0);
	}

	void BallBlockCollisions() {
		std::vector<RectangleShape*> toDelete;
		for (RectangleShape* block : blocks) {
			if (collisionManager.IsColliding(*ball, *block)) {
				toDelete.push_back(block);

			}
		}
		if (!toDelete.empty()) {
			RectangleShape* responseBlock = toDelete[0];
			Vector2i dir = GetCollisionDirection(*responseBlock);
			if (dir.x) ballMotion->SetYVel(FlipSign(ballMotion->GetVel().y));
			else ballMotion->SetXVel(FlipSign(ballMotion->GetVel().x));
		}
		RectangleShape* block;
		while (!toDelete.empty()) {
			block = toDelete.back();
			toDelete.pop_back();
			DeleteBlock(block);
			block = NULL;
		}
	}

private:
	CircleShape* ball;
	MovingBody* ballMotion;
	DoublyLL<RectangleShape*> blocks;
	RectangleShape* paddle;
	MovingBody* paddleMotion;
	LineShape* paddleLine;
	RectangleShape* paddlePadding;
	bool started;
	char prev;
	char cur;
	bool holdingStart;
};