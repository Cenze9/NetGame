#include "game.h"


game *Game = nullptr;

int main(int argc, char* arg[])
{
	const int fps = 60;
	const int frameDelay = 1000 / fps;

	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last = 0;

	Uint32 frameStart;
	int frameTime;

	Game = new game();
	Game->Init("Pong: 1 vs. 1");
	while (Game->Running())
	{
		frameStart = SDL_GetTicks();

		Game->HandleEvents();
		Game->Update();
		Game->Render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) 
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	Game->Clean();

	return 0;
}