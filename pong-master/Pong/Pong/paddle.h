#pragma once
#include <SDL.h>


class paddle
{
public:
	paddle(int x, SDL_Renderer* ren);

	int posY;


	int width = 10;
	int height = 70;

	

	void MovePaddle();
	void RenderPaddle();
	//void CollisioPaddle();

private:
	int posX;

	SDL_Renderer* paddleRend;
	SDL_Rect paddleRect;

	virtual ~paddle();
};

