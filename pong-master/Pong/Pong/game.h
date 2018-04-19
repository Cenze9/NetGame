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
	void Update(int playerID, int newYPos);
	void Render();
	bool Running() { return isRunning; }
	void Clean();
	virtual ~game();

	SDL_Rect Ball;
	float ballX;
	float ballY;
	void RenderBall();

	paddle* Player1;
	paddle* Player2;
private:
	

	bool isRunning = false;
	SDL_Window* window;
	SDL_Renderer* renderer;
};
