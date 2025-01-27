#include "EventHandler.h"

#include "Engine.h"
#include <SDL3/SDL_events.h>

SDL_Event event = {};

void EventHandler::HandleEvents()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			Engine::Quit();
			break;

		default:
			break;
		}
	}
}