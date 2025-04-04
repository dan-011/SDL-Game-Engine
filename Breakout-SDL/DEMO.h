#pragma once
#include "Scene.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "PointShape.h"
#include "LineShape.h"
#include "MovingBody.h"
#include "GeometryHelper.h"
#include "GameManager.h"
#include <stdlib.h>
#include <SDL_ttf.h>
#include <string>
#include <time.h>

#define DEBUG
#ifdef DEBUG
using namespace std;
#include <iostream>
void Log(const std::string& str) {
	cout << str << endl;
}
#endif

#define N 4
#define PADDING 5
#define DIM 100
#define GetIdx(pos)(((pos.x) * N) + pos.y)

Vector2f GetStartPos() {
	return Vector2f((SCREEN_WIDTH / 2) - ((((DIM + PADDING) * N) - PADDING) / 2), (SCREEN_HEIGHT / 2) - ((((DIM + PADDING) * N) - PADDING) / 2));
}
Vector2f GetTruePos(const Vector2i& gridPos) {
	Vector2f startPos = GetStartPos();
	return Vector2f(startPos.x + gridPos.y * (DIM + PADDING), startPos.y + gridPos.x * (DIM + PADDING));
}

class Tile : public RectangleShape {
public:
	Tile(const Vector2i& gridPos) : RectangleShape(Vector2f(0, 0), Vector2f(DIM, DIM), Color(255, 0, 0), true, 2), pos(gridPos), textWidth(0), textHeight(0), textTexture(NULL), destPos(gridPos) {
		srand((unsigned int)time(NULL));
		value = ((rand() % 2) + 1) * 2;
		Vector2f truePos = GetTruePos(gridPos);
		truePos.x += (DIM / 2) - 1;
		truePos.y += (DIM / 2) - 1;
		SetPos(truePos);
		SetDims(Vector2f(2, 2));
	}
	~Tile() {
		SDL_DestroyTexture(textTexture);
		textTexture = NULL;
		textWidth = 0;
		textHeight = 0;
	}

	void UpdatePos(const Vector2f& vel, Uint32 deltaTime) {
		Vector2f curPos = GetPos();
		curPos.x += (vel.x * deltaTime);
		curPos.y += (vel.y * deltaTime);
		SetPos(curPos);
	}

	void UpdateSize(Uint32 deltaTime) {
		float vel = 0.75;
		Vector2f dims = GetDims();
		dims.x = Min((float)DIM, dims.x + (vel * deltaTime));
		dims.y = Min((float)DIM, dims.y + (vel * deltaTime));
		SetDims(dims);
		Vector2f truePos = GetTruePos(pos);
		truePos.x += (DIM / 2) - (dims.x / 2);
		truePos.y += (DIM / 2) - (dims.y / 2);
		SetPos(truePos);
	}

	void Update(Uint32 deltaTime) {
		Vector2f dest = GetTruePos(destPos);
		Vector2f curPos = GetPos();
		// calculate direction to go
		// set velocity
		// move based on DT
		// set position if overshot
		if (!AtSize()) UpdateSize(deltaTime);
		else if (curPos.x - dest.x > 0.0001f) { // go to the left
			UpdatePos(Vector2f(-1, 0), deltaTime);
			if (GetPos().x < dest.x) {
				SetPos(dest);
				pos = destPos;
			}
		}
		else if (curPos.x - dest.x < -0.0001f) { // go to the right
			UpdatePos(Vector2f(1, 0), deltaTime);
			if (GetPos().x > dest.x) {
				SetPos(dest);
				pos = destPos;
			}
		}
		else if (curPos.y - dest.y > 0.0001f) { // go up (negative vel)
			UpdatePos(Vector2f(0, -1), deltaTime);
			if (GetPos().y < dest.y) {
				SetPos(dest);
				pos = destPos;
			}
		}
		else if (curPos.y - dest.y < -0.0001f) { // go down (positive vel)
			UpdatePos(Vector2f(0, 1), deltaTime);
			if (GetPos().y > dest.y) {
				SetPos(dest);
				pos = destPos;
			}
		}
	}
	void MoveTo(const Vector2i& gridPos) {
		destPos = gridPos;
	}
	int GetValue() {
		return value;
	}
	void SetValue(int val) {
		value = val;
	}
	Vector2i GetGridPos() {
		return pos;
	}
	bool AtSize() {
		Vector2f dims = GetDims();
		return FAbs(dims.x - DIM) < 0.0001f && FAbs(dims.y - DIM) < 0.0001f;
	}
	void UpdateAppearance(std::unordered_map<int, Color>& colors, TTF_Font* font) {
		SetColor(colors[value]);
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, std::to_string(value).c_str(), {0, 0, 0});
		SDL_DestroyTexture(textTexture);
		textTexture = SDL_CreateTextureFromSurface(GameManager::Instance().GetRenderer(), textSurface);
		textWidth = textSurface->w;
		textHeight = textSurface->h;
		SDL_FreeSurface(textSurface);
	}
	virtual void Render() override {
		RectangleShape::Render();
		if (textTexture) {
			int textXPos = (int)GetPos().x + ((DIM / 2) - (textWidth / 2));
			int textYPos = (int)GetPos().y + ((DIM / 2) - (textHeight / 2));
			SDL_Rect renderQuad = { textXPos, textYPos, textWidth, textHeight};
			if(AtSize()) SDL_RenderCopyEx(GameManager::Instance().GetRenderer(), textTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
		}
	}
	void Double() {
		value <<= 1;
	}
	void Clear() {
		value = 0;
	}
	bool AtDest() {
		Vector2f curPos = GetPos();
		Vector2f dest = GetTruePos(destPos);
		Vector2f dims = GetDims();
		return FAbs(curPos.x - dest.x) < 0.0001f && FAbs(curPos.y - dest.y) < 0.0001f && AtSize();
	}
	bool WillMove() {
		return !(pos == destPos);
	}
private:
	Vector2i pos;
	int value;
	SDL_Texture* textTexture;
	int textWidth;
	int textHeight;
	Vector2i destPos;
};

class RedRectangleScene : public Component, public Scene {
public:
	RedRectangleScene(): gameTiles(N * N, NULL), font(NULL), inputLocked(false) {
		BuildBoard();
		AddComponent(this);

		Color clr(0x00, 0x00, 0x00);
		SetBackgroundColor(clr);

		tileColors.insert({ 2, Color(83,165,35) });
		tileColors.insert({ 4, Color(30,144,210) });
		tileColors.insert({ 8, Color(255,198,66) });
		tileColors.insert({ 16, Color(82,0,31) });
		tileColors.insert({ 32, Color(169,245,233) });
		tileColors.insert({ 64, Color(249,170,120) });
		tileColors.insert({ 128, Color(54,164,47) });
		tileColors.insert({ 256, Color(248,11,178) });
		tileColors.insert({ 512, Color(133,49,25) });
		tileColors.insert({ 1024, Color(35,244,43) });
		tileColors.insert({ 2048, Color(232,100,67) });

		font = TTF_OpenFont("font.ttf", 28);
		if (font == NULL) {
			throw "Error";
		}

		StartGame();
	}
	~RedRectangleScene() {
	}

	void Free() {
		DestroyBoard();
		DestroyTiles();
		TTF_CloseFont(font);
		font = NULL;
	}

	void BuildBoard() {
		Vector2f startPos = GetStartPos();
		Vector2f pos = startPos;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				boardTiles.push_back(new RectangleShape(pos, Vector2f(DIM, DIM), Color(205, 127, 50), true, 1));
				AddComponent(boardTiles.back());
				pos.x += DIM + PADDING;
			}
			pos.x = startPos.x;
			pos.y += DIM + PADDING;
		}
		boardTiles.push_back(new RectangleShape(startPos - Vector2f(PADDING, PADDING), Vector2f(((DIM + PADDING) * N) + PADDING, ((DIM + PADDING) * N) + PADDING), Color(255, 255, 255), true, 0));
		AddComponent(boardTiles.back());
	}

	void StartGame() {
		CreateNewTile();
		CreateNewTile();
	}

	void CreateNewTile() {
		srand((unsigned int)time(NULL));
		Vector2i startGridPos;
		do {
			startGridPos.x = N - 1;// rand() % N;
			startGridPos.y = rand() % N;
		} while (gameTiles[GetIdx(startGridPos)]);
		Tile* tile = new Tile(startGridPos);
		tile->UpdateAppearance(tileColors, font);
		gameTiles[GetIdx(startGridPos)] = tile;
		AddComponent(tile);
	}

	void DestroyBoard() {
		for (auto& tile : boardTiles) {
			RemoveComponent(tile);
			delete tile;
		}
		boardTiles.clear();
	}

	void DestroyTiles() {
		for (int i = 0; i < (N * N); i++) {
			if (gameTiles[i]) {
				RemoveComponent(gameTiles[i]);
				delete gameTiles[i];
				gameTiles[i] = NULL;
			}
		}
	}

	virtual void Update(Uint32 deltaTime) override {
		if (inputLocked && TilesAtDest()) {
			UpdateTileAppearances();
			CleanTiles();
			if (tilesMoved) CreateNewTile();
			inputLocked = false;
			tilesMoved = false;
		}
	}
	bool TilesChanged(std::vector<Tile*>& copy) {
		for (int i = 0; i < N * N; i++) {
			if (copy[i] != gameTiles[i]) return true;
		}
		return false;
	}
	bool TilesAtDest() {
		for (int i = 0; i < N * N; i++) {
			if (gameTiles[i] && !gameTiles[i]->AtDest()) return false;
		}
		for (auto& tile : toClean) {
			if (!tile->AtDest()) return false;
		}
		return true;
	}

	void UpdateTileAppearances() {
		for (int i = 0; i < N * N; i++) {
			if (gameTiles[i]) gameTiles[i]->UpdateAppearance(tileColors, font);
		}
	}

	void CleanTiles() {
		Tile* tile;
		while (!toClean.empty()) {
			tile = toClean.back();
			toClean.pop_back();
			RemoveComponent(tile);
			delete tile;
		}
	}

	void MoveTiles(const Vector2i& dir) {
		inputLocked = true;
		// move the tiles to their final position for that round
		// then over the next few tics the blocks will move - input will be blocked
		// once all of the blocks are in their final position, input can be resumed
		// TODO: merge like adjacent tiles
		std::vector<Tile*> bucket;
		int idx;
		int len;
		int r;
		if (dir.x > 0) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					idx = GetIdx(Vector2i(i, j));
					if (gameTiles[idx]) {
						bucket.push_back(gameTiles[idx]);
						gameTiles[idx] = NULL;
					}
				}
				len = (int)bucket.size();
				for (int k = len - 1; k > 0; k--) {
					if (bucket[k]->GetValue() == bucket[k - 1]->GetValue()) {
						bucket[k]->Double();
						bucket[k - 1]->Clear();
					}
				}
				r = N;
				for (int k = len - 1; k >= 0; k--) {
					if (bucket[k]->GetValue()) {
						r--;
						gameTiles[GetIdx(Vector2i(i, r))] = bucket[k]; // only keep it if it's got a value
					}
					else toClean.push_back(bucket[k]);
					bucket[k]->MoveTo(Vector2i(i, r)); // we want to move to the grid position
				}
				bucket.clear();
			}
		}
		else if (dir.x < 0) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					idx = GetIdx(Vector2i(i, j));
					if (gameTiles[idx]) {
						bucket.push_back(gameTiles[idx]);
						gameTiles[idx] = NULL;
					}
				}
				int len = (int)bucket.size();
				for (int k = 0; k < len - 1; k++) {
					if (bucket[k]->GetValue() == bucket[k + 1]->GetValue()) {
						bucket[k]->Double();
						bucket[k+1]->Clear();
					}
				}
				r = -1;
				for (int k = 0; k < bucket.size(); k++) {
					if (bucket[k]->GetValue()) {
						r++;
						gameTiles[GetIdx(Vector2i(i, r))] = bucket[k];
					}
					else toClean.push_back(bucket[k]);
					bucket[k]->MoveTo(Vector2i(i, r));
				}
				bucket.clear();
			}
		}
		else if (dir.y > 0) {
			// down
			for (int j = 0; j < N; j++) {
				for (int i = 0; i < N; i++) {
					idx = GetIdx(Vector2i(i, j));
					if (gameTiles[idx]) {
						bucket.push_back(gameTiles[idx]);
						gameTiles[idx] = NULL;
					}
				}
				len = (int)bucket.size();
				for (int k = len - 1; k > 0; k--) {
					if (bucket[k]->GetValue() == bucket[k - 1]->GetValue()) {
						bucket[k]->Double();
						bucket[k - 1]->Clear();
					}
				}
				r = N;
				for (int k = len - 1; k >= 0; k--) {
					if (bucket[k]->GetValue()) {
						r--;
						gameTiles[GetIdx(Vector2i(r, j))] = bucket[k];
					}
					else toClean.push_back(bucket[k]);
					bucket[k]->MoveTo(Vector2i(r, j));
				}
				bucket.clear();
			}
		}
		else if (dir.y < 0) {
			for (int j = 0; j < N; j++) {
				for (int i = 0; i < N; i++) {
					idx = GetIdx(Vector2i(i, j));
					if (gameTiles[idx]) {
						bucket.push_back(gameTiles[idx]);
						gameTiles[idx] = NULL;
					}
				}
				len = (int)bucket.size();
				for (int k = 0; k < len - 1; k++) {
					if (bucket[k]->GetValue() == bucket[k + 1]->GetValue()) {
						bucket[k]->Double();
						bucket[k + 1]->Clear();
					}
				}
				r = -1;
				for (int k = 0; k < bucket.size(); k++) {
					if (bucket[k]->GetValue()) {
						r++;
						gameTiles[GetIdx(Vector2i(r, j))] = bucket[k];
					}
					else toClean.push_back(bucket[k]);
					bucket[k]->MoveTo(Vector2i(r, j));
				}
				bucket.clear();
			}
		}
		tilesMoved = !toClean.empty();
		if (tilesMoved) {
			cout << "tiles moved to begin with" << endl;
		}
		for (int i = 0; i < N * N && !tilesMoved; i++) {
			if (gameTiles[i] && gameTiles[i]->WillMove()) {
				cout << "moving" << gameTiles[i]->GetGridPos().x << "," << gameTiles[i]->GetGridPos().y << endl;
				tilesMoved = true;
			}
		}
		if (tilesMoved) {
			cout << "tiles moved" << endl;
		}
	}

	virtual void NotifyEvent(SDL_Event* e) override {
		if (!inputLocked && e->type == SDL_KEYDOWN) {
			switch (e->key.keysym.sym) {
				case SDLK_UP:
					MoveTiles(Vector2i(0, -1));
					break;
				case SDLK_DOWN:
					MoveTiles(Vector2i(0, 1));
					break;
				case SDLK_RIGHT:
					MoveTiles(Vector2i(1, 0));
					break;
				case SDLK_LEFT:
					MoveTiles(Vector2i(-1, 0));
					break;
			}
		}
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
	float vel = 0.25;
	std::vector<RectangleShape*> boardTiles;
	std::unordered_map<int, Color> tileColors;
	std::vector<Tile*> gameTiles;
	std::vector<Tile*> toClean;
	SDL_Texture* textTexture;
	TTF_Font* font;
	bool inputLocked;
	bool tilesMoved;
};