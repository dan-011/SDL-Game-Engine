#include "Scene.h"
#include "GameManager.h"
#include <SDL.h>

Scene::Scene() : ticks(0), quit(false), backgroundColor(0xFF, 0xFF, 0xFF), e(), eventsEnabled(true), keysEnabled(true) {
	
}
Scene::~Scene() {}

void Scene::Play() {
	Construct();
	ticks = SDL_GetTicks();
	while (!quit)
	{
		//Handle events on queue
		

		//const Uint8* keys = SDL_GetKeyboardState(NULL);
		//NotifyKeystate(keys);
		if (ticks - SDL_GetTicks() >= 16) {
			UpdateComponents();
			ticks = SDL_GetTicks();

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT) quit = true;
				else if (eventsEnabled) NotifyEvent(&e);
			}
			if (keysEnabled) {
				const Uint8* keys = SDL_GetKeyboardState(NULL);
				NotifyKeystate(keys);
			}
		}

		SDL_Renderer* renderer = GameManager::Instance().GetRenderer();
		SDL_SetRenderDrawColor(renderer, backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
		SDL_RenderClear(renderer);
		RenderComponents();
		SDL_RenderPresent(renderer);
	}
}
void Scene::Pause() {

}
void Scene::Stop() {

}

void Scene::NotifyEvent(SDL_Event* e) {}
void Scene::NotifyKeystate(const Uint8* keys) {}

void Scene::AddComponent(Component* component) {
	VisualComponent* visualComponent = dynamic_cast<VisualComponent*>(component);
	if(visualComponent) visualComponents[visualComponent->GetLayer()].PushBack(visualComponent);
	components.PushBack(component);
}
void Scene::RemoveComponent(Component* component) {
	VisualComponent* visualComponent = dynamic_cast<VisualComponent*>(component);
	if (visualComponent) {
		DoublyLL<VisualComponent*>& layer = visualComponents[visualComponent->GetLayer()];
		layer.Erase(visualComponent);
	}
	components.Erase(component);
}

void Scene::RenderComponents() {
	for (int layer = 0; layer < MAX_LAYERS; layer++) {
		for (VisualComponent* component : visualComponents[layer]) {
			if (component != NULL && component->GetVisibility()) component->Render();
		}
	}
}

void Scene::UpdateComponents() {
	for (auto& component : components) {
		component->Update(SDL_GetTicks() - ticks);
	}
}

void Scene::Construct() {
	for (auto& component : components) component->Construct();
}

void Scene::EndScene() {
	quit = true;
}

void Scene::SetBackgroundColor(const Color& color) {
	backgroundColor = color;
}
Color Scene::GetBackgroundColor() {
	return backgroundColor;
}

SDL_Event Scene::GetEvent() { return e; }

void Scene::EnableEvents() {
	eventsEnabled = true;
}
void Scene::DisableEvents() {
	eventsEnabled = false;
}
void Scene::EnableKeys() {
	keysEnabled = true;
}
void Scene::DisableKeys() {
	keysEnabled = false;
}