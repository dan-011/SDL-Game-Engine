#pragma once
#include "DoublyLL.h"
#include "Component.h"
#include "VisualComponent.h"
#include "Color.h"
#include "CollisionManager.h"
#include <vector>

// we need a scene stager to control which scenes have the stage
typedef void(*UpdateFunc)(Uint32);
typedef void(*ConstructFunc)();

class Scene {
public:
	Scene();
	~Scene();

	void Play();
	void Pause();
	void Stop();

	virtual void NotifyEvent(SDL_Event* e);
	virtual void NotifyKeystate(const Uint8* keys);

	// we may modify this to only add components when they support an update construct (save space and time)
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	void RenderComponents();
	void Construct();
	void UpdateComponents();

	void EndScene();

	void SetBackgroundColor(const Color& color);
	Color GetBackgroundColor();

	SDL_Event GetEvent();

	void EnableEvents();
	void DisableEvents();
	void EnableKeys();
	void DisableKeys();

protected:
	CollisionManager collisionManager;

private:
	DoublyLL<VisualComponent*> visualComponents[MAX_LAYERS];
	DoublyLL<Component*> components;
	Uint32 ticks;
	Color backgroundColor;
	bool quit;
	SDL_Event e;
	bool eventsEnabled;
	bool keysEnabled;

};