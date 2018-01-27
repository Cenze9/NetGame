#include "game.h"




game::game()
{

}

void game::Init(const char* title)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 700, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer) 
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderPresent(renderer);
			isRunning = true;
		}
		else
		{
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		}

	}
//	Player1 = new paddle(10, renderer);
	//Player2 = new paddle(780, renderer);
}

void game::HandleEvents()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent));
		const Uint8* KeyState = SDL_GetKeyboardState(NULL);
	SDL_PollEvent(&sdlEvent);

	if (KeyState[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}
	if(KeyState[SDL_SCANCODE_UP])
	{
		Player1->posY -= 5;
	}
	if (KeyState[SDL_SCANCODE_DOWN]) 
	{
		Player1->posY += 5;
	}
	if (KeyState[SDL_SCANCODE_W])
	{
		Player2->posY -= 5;
	}
	if (KeyState[SDL_SCANCODE_S])
	{
		Player2->posY += 5;
	}
}

void game::Update()
{
	Player1->MovePaddle();
	Player2->MovePaddle();
}

void game::Render()
{
	SDL_RenderClear(renderer);
	Player1->RenderPaddle();
	Player2->RenderPaddle();
	RenderBall();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderPresent(renderer);
}

void game::RenderBall() 
{
	Ball = { ballX, ballY, 10, 10 };
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_RenderFillRect(renderer, &Ball);
}

void game::Clean() 
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

game::~game()
{

}
