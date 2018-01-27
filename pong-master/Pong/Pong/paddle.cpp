#include "paddle.h"



paddle::paddle(int x, SDL_Renderer* ren) : paddleRend(ren)
{
	posX = x;
}

void paddle:: MovePaddle()
{
	 if (posY > 700 - height)
	 {
		 posY = 700 - height;
	 }
	 if (posY < 0) 
	 {
		 posY = 0;
	 }
}
void paddle::RenderPaddle()
{
	paddleRect = { posX, posY, width, height };
	SDL_SetRenderDrawColor(paddleRend, 255, 255, 255, 255);
	SDL_RenderFillRect(paddleRend, &paddleRect);
}

paddle::~paddle()
{

}
