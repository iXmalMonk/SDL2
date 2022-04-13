#include<SDL.h>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

void DeInit(int error, SDL_Window*& window, SDL_Renderer*& renderer)
{
	if (renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	if (window != NULL)
	{
		SDL_DestroyWindow(window);
		window = NULL;
	}

	SDL_Quit();
	exit(error);
}

void Init(SDL_Window*& window, SDL_Renderer*& renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) != NULL)
	{
		printf("SDL_Init error: %s", SDL_GetError());
		system("pause");
		DeInit(1, window, renderer);
	}

	window = SDL_CreateWindow("Top-Down Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("SDL_CreateWindow error: %s", SDL_GetError());
		system("pause");
		DeInit(1, window, renderer);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
	{
		printf("SDL_CreateRenderer error: %s", SDL_GetError());
		system("pause");
		DeInit(1, window, renderer);
	}
}

int main(int argc, char* argv[])
{
	// -----
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	// -----

	Init(window, renderer);

	DeInit(0, window, renderer);

	return 0;
}