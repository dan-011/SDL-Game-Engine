#pragma once
#include "Scene.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "MovingBody.h"
#include "DoublyLL.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "InputManager.h"
#include <vector>

#if VERSION == PC
class Breakout : public Component, public Scene {
public:
	Breakout() : ball(NULL), paddle(NULL), ballMotion(NULL), paddleMotion(NULL), paddleLine(NULL), paddlePadding(NULL), started(false), img(NULL), scoreMsg(NULL), score(NULL), pts(0), pressStart(NULL) {
		paddle = new RectangleShape(Vector2f((SCREEN_WIDTH / 2) - 50, SCREEN_HEIGHT - 60), Vector2f(100, 20), Color(0x00, 0xFF, 0x00));
		paddleMotion = new MovingBody(paddle->GetPos(), Vector2f(0, 0), Vector2f(1, 0));
		const float radius = 20;
		ball = new CircleShape(Vector2f(paddle->GetPos().x + (paddle->GetDims().x / 2), paddle->GetPos().y - radius), radius, Color(0xFF, 0xFF, 0xFF), true, 1);
		ballMotion = new MovingBody(ball->GetCenterPoint());
		paddleLine = new LineShape(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y), Color(0xFF, 0x00, 0xFF), 0, true);
		paddlePadding = new RectangleShape(paddle->GetPos(), Vector2f(paddle->GetDims().x, paddle->GetDims().y * 0.2), Color(0xFF, 0x00, 0xFF), true);
		img = new ImageComponent("img.png", Vector2f(0, 0));
		scoreMsg = new TextComponent("Score:", Color(0xFF, 0xFF, 0xFF), "font.otf", 32, Vector2f(10, SCREEN_HEIGHT - 32 - 10), 15);
		score = new TextComponent("0", Color(0xFF, 0xFF, 0xFF), "font.otf", 32, Vector2f(scoreMsg->GetPos().x + scoreMsg->GetDims().x + 10, SCREEN_HEIGHT - 32 - 10), 15);
		pressStart = new TextComponent("PRESS START", Color(0xFF, 0xFF, 0xFF), "font.ttf", 64, Vector2f(0, 0), 15);
		pressStart->SetPos((SCREEN_WIDTH / 2) - (pressStart->GetDims().x / 2), (SCREEN_HEIGHT / 2) - (pressStart->GetDims().y / 2));
		//AddComponent(img);
		AddComponent(paddle);
		AddComponent(paddleMotion);
		AddComponent(ball);
		AddComponent(ballMotion);
		AddComponent(paddleLine);
		AddComponent(paddlePadding);
		AddComponent(scoreMsg);
		AddComponent(score);
		AddComponent(pressStart);
		BuildBlocks();
		AddComponent(this);

		SetBackgroundColor(Color(0x00, 0x00, 0x00));
		DisableKeys();

		InputManager::Instance().AddInput(SDL_SCANCODE_S);
		InputManager::Instance().AddInput(SDL_SCANCODE_R);
		InputManager::Instance().AddInput(SDL_SCANCODE_RIGHT);
		InputManager::Instance().AddInput(SDL_SCANCODE_LEFT);

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
		delete img;
		img = NULL;
		delete scoreMsg;
		scoreMsg = NULL;
		delete score;
		score = NULL;

		DeleteBlocks();
	}

	void Reset() {
		started = false;
		pts = 0;
		DeleteBlocks();
		paddle->SetPos(Vector2f((SCREEN_WIDTH / 2) - 50, SCREEN_HEIGHT - 60));
		paddleMotion->SetPos(paddle->GetPos());
		ball->SetPos(Vector2f(paddle->GetPos().x + (paddle->GetDims().x / 2), paddle->GetPos().y - ball->GetRadius()));
		ballMotion->SetPos(ball->GetPos());
		paddleLine->SetPoints(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y));
		paddlePadding->SetPos(paddle->GetPos());
		score->SetText(std::to_string(pts));

		paddleMotion->SetVel(Vector2f(0, 0));
		ballMotion->SetVel(Vector2f(0, 0));
		pressStart->SetVisibility(true);

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
		BoundsCheck();
		BallPaddleCollision();
		BallBlockCollisions();
		paddle->SetPos(paddleMotion->GetPos()); // Maybe make an object that has a visual component and does this automatically
		ball->SetPos(ballMotion->GetPos());
		paddleLine->SetPoints(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y));
		paddlePadding->SetPos(paddle->GetPos());
	}

	virtual void NotifyEvent(SDL_Event* e) override {

		if (!started) {
			if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_s) {
				pressStart->SetVisibility(false);
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

			pts += toDelete.size() * 10;
			score->SetText(std::to_string(pts));
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

	ImageComponent* img;
	TextComponent* scoreMsg;
	TextComponent* score;
	TextComponent* pressStart;
	unsigned int pts;
};
#endif

#if VERSION == VITA
class Breakout : public Component, public Scene {
public:
	Breakout() : ball(NULL), paddle(NULL), ballMotion(NULL), paddleMotion(NULL), paddleLine(NULL), paddlePadding(NULL), started(false), pts(0), img(NULL), scoreMsg(NULL), score(NULL), pressStart(NULL) {
		const std::string path = "ux0:app/VSDK01011/assets/";
		paddle = new RectangleShape(Vector2f((SCREEN_WIDTH / 2) - 50, SCREEN_HEIGHT - 60), Vector2f(100, 20), Color(0x00, 0xFF, 0x00));
		paddleMotion = new MovingBody(paddle->GetPos(), Vector2f(0, 0), Vector2f(1, 0));
		const float radius = 20;
		ball = new CircleShape(Vector2f(paddle->GetPos().x + (paddle->GetDims().x / 2), paddle->GetPos().y - radius), radius, Color(0xFF, 0xFF, 0xFF), true, 1);
		ballMotion = new MovingBody(ball->GetCenterPoint());
		paddleLine = new LineShape(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y), Color(0xFF, 0x00, 0xFF), 0, true);
		paddlePadding = new RectangleShape(paddle->GetPos(), Vector2f(paddle->GetDims().x, paddle->GetDims().y * 0.2), Color(0xFF, 0x00, 0xFF), true);
		img = new ImageComponent(path + "img.png", Vector2f(0, 0));
		scoreMsg = new TextComponent("Score:", Color(0xFF, 0xFF, 0xFF), path + "font.otf", 32, Vector2f(10, SCREEN_HEIGHT - 32 - 10), 15);
		score = new TextComponent("0", Color(0xFF, 0xFF, 0xFF), path + "font.otf", 32, Vector2f(scoreMsg->GetPos().x + scoreMsg->GetDims().x + 10, SCREEN_HEIGHT - 32 - 10), 15);
		pressStart = new TextComponent("PRESS START", Color(0xFF, 0xFF, 0xFF), path + "font.ttf", 64, Vector2f(0, 0), 15);
		pressStart->SetPos((SCREEN_WIDTH / 2) - (pressStart->GetDims().x / 2), (SCREEN_HEIGHT / 2) - (pressStart->GetDims().y / 2));
		img->SetVisibility(false);
		AddComponent(img);
		AddComponent(paddle);
		AddComponent(paddleMotion);
		AddComponent(ball);
		AddComponent(ballMotion);
		AddComponent(paddleLine);
		AddComponent(paddlePadding);
		AddComponent(scoreMsg);
		AddComponent(score);
		AddComponent(pressStart);
		BuildBlocks();
		AddComponent(this);

		SetBackgroundColor(Color(0x00, 0x00, 0x00));
		DisableKeys();
		DisableEvents();
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
		delete img;
		img = NULL;
		delete scoreMsg;
		scoreMsg = NULL;
		delete score;
		score = NULL;
		delete pressStart;
		pressStart = NULL;

		DeleteBlocks();
	}

	void BeginGame() {
		started = true;
		ballMotion->SetVel(0.5, -0.5);
		pressStart->SetVisibility(false);
	}

	void Reset() {
		started = false;
		pts = 0;
		DeleteBlocks();
		paddle->SetPos(Vector2f((SCREEN_WIDTH / 2) - 50, SCREEN_HEIGHT - 60));
		paddleMotion->SetPos(paddle->GetPos());
		ball->SetPos(Vector2f(paddle->GetPos().x + (paddle->GetDims().x / 2), paddle->GetPos().y - ball->GetRadius()));
		ballMotion->SetPos(ball->GetPos());
		paddleLine->SetPoints(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y));
		paddlePadding->SetPos(paddle->GetPos());
		score->SetText(std::to_string(pts));

		paddleMotion->SetVel(Vector2f(0, 0));
		ballMotion->SetVel(Vector2f(0, 0));
		pressStart->SetVisibility(true);

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
		BoundsCheck();
		BallPaddleCollision();
		BallBlockCollisions();
		paddle->SetPos(paddleMotion->GetPos()); // Maybe make an object that has a visual component and does this automatically
		ball->SetPos(ballMotion->GetPos());
		paddleLine->SetPoints(paddle->GetPos(), Vector2f(paddle->GetPos().x + paddle->GetDims().x - 1, paddle->GetPos().y));
		paddlePadding->SetPos(paddle->GetPos());
	}

	virtual void NotifyEvent(SDL_Event* e) override {}

	virtual void HandleInputs() override {
		if (InputManager::Instance().InputPressedOnce(SCE_CTRL_SELECT)) QuitGame();
		if (InputManager::Instance().InputPressedOnce(SCE_CTRL_CROSS)) img->SetVisibility(!img->GetVisibility());
		if (InputManager::Instance().InputPressedOnce(SCE_CTRL_START)) {
			if (started) Reset();
			else BeginGame();
			return;
		}
		if (!started) return;
		int dir = 0;
		if (InputManager::Instance().InputPressed(SCE_CTRL_RIGHT) || InputManager::Instance().GetLX() >= 160) dir = 1;
		else if (InputManager::Instance().InputPressed(SCE_CTRL_LEFT) || InputManager::Instance().GetLX() <= 70) dir = -1;
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
			if (ballPos.x + radius > SCREEN_WIDTH) ballMotion->SetPos(SCREEN_WIDTH - radius, ballPos.y);
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

			pts += toDelete.size() * 10;
			score->SetText(std::to_string(pts));
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

	ImageComponent* img;
	TextComponent* scoreMsg;
	TextComponent* score;
	TextComponent* pressStart;
	unsigned int pts;
};
#endif