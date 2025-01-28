#include "Engine.h"

#include "Rendering/Renderer.h"
#include "EventHandler.h"
#include "Debug.h"
#include <SDL3/SDL.h>
#include <format>

bool isRunning = true;

static void InitializeSDL()
{
	bool initSDLResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
	ASSERT(initSDLResult, true, std::format("SDL_Init error: %s", SDL_GetError()));
}

void Engine::Initialize()
{
	Debug::Initialize();
	InitializeSDL();
	Renderer::Initialize();
}

void Engine::Destroy()
{
	Renderer::Destroy();
	SDL_Quit();
	Debug::Destroy();
}

void Engine::Run()
{
	while (isRunning)
	{
		EventHandler::HandleEvents();
		Renderer::RenderFrame();
	}
}

void Engine::Quit()
{
	isRunning = false;
}