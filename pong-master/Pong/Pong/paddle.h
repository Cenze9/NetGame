#pragma once
#include <SDL.h>


class paddle
{
public:
	paddle(int x, SDL_Renderer* ren);

	int posY=100;
	int posX;

	int width = 10;
	int height = 70;

	

	void MovePaddle();
	void RenderPaddle();
	//void CollisioPaddle();

private:
	

	SDL_Renderer* paddleRend;
	SDL_Rect paddleRect;

	virtual ~paddle();
};

