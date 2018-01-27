#pragma once
#include <stdio.h>
#include <SDL.h>

#include "paddle.h"

#define window_width 800;
#define window_height 600;

class game
{
public:
	game();
	void Init(const char* title);
	void HandleEvents();
	void Update();
	void Render();
	bool Running() { return isRunning; }
	void Clean();
	virtual ~game();
	
private:
	paddle* Player1;
	paddle* Player2;

	bool isRunning = false;
	SDL_Window* window;
	SDL_Renderer* renderer;
};
