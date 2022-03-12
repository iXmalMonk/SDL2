#include<SDL.h>
#include<iostream>

// Resolution
const int w = 800;
const int h = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void DeInit(int error)
{
	if (renderer != NULL) SDL_DestroyRenderer(renderer);
	if (window != NULL) SDL_DestroyWindow(window);
	SDL_Quit();
	exit(error);
}

void Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	window = SDL_CreateWindow("Practice SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL_CreateWindow error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("SDL_CreateRenderer error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}
}

void Background(int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);
}

void Circle(int x_circle, int y_circle, int r)
{
	int x_point, y_point;
	for (double alpha = 0; alpha <= 360; alpha += 0.5)
	{
		x_point = x_circle + r * cos(alpha * M_PI / 180);
		y_point = y_circle + r * sin(alpha * M_PI / 180);
		SDL_RenderDrawPoint(renderer, x_point, y_point);
	}
}

void CircleUp(int x_circle, int y_circle, int r)
{
	int x_point, y_point;
	for (double alpha = 0; alpha <= 180; alpha += 0.5)
	{
		x_point = x_circle - r * cos(alpha * M_PI / 180);
		y_point = y_circle - r * sin(alpha * M_PI / 180);
		SDL_RenderDrawPoint(renderer, x_point, y_point);
	}
}

void Snowman() // Center
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

	for (int i = 0; i < 90; i++) // R = 90
		Circle(w / 2, h / 2, 90 - i); // big circle

	for (int i = 0; i < 60; i++) // R = 60
		Circle(w / 2, h / 4, 60 - i); // medium circle

	for (int i = 0; i < 30; i++) // R = 30
		Circle(w / 2, h / 10, 30 - i); // small circle


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black

	for (int i = 0, j = 15; i < 3; i++, j += 30)
		for (int k = 0; k < 9; k++) // R = 9
			Circle(w / 2, h / 3 - j, 9 - k); // button

	for (int i = 0, k = -10; i < 2; i++, k += 20)
		for (int j = 0; j < 6; j++) // R = 6
			Circle(w / 2 + k, h / 12, 6 - j); // eyes

	for (int i = -40, j = -90, k = 0; k < 2; i += 80, j += 180, k++)
		SDL_RenderDrawLine(renderer, w / 2 + i, h / 5, w / 2 + j, h / 5 + 50); //hands

	for (int i = 0, j = -50; i < 2; i++, j += 100)
		for (int k = 0; k < 45; k++) // R = 45
			CircleUp(w / 2 + j, h / 1.535, 45 - k); // legs
}

void Ornament()
{

}

int main(int argc, char* argv[])
{
	Init();

	Background(100, 100, 100); // Gray

	Snowman();

	SDL_RenderPresent(renderer);
	system("pause");
	DeInit(0);
	return 0;
}