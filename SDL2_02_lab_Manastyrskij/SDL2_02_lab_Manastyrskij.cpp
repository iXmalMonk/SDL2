#include<SDL.h>
#include<iostream>

const int w = 600;
const int h = 600;

SDL_Window* window = NULL;
SDL_Surface* surface = NULL, *image = SDL_LoadBMP("12.bmp");

void DeInit(int error)
{
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

	window = SDL_CreateWindow("Lab 02", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL_CreateWindow error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	surface = SDL_GetWindowSurface(window);
	if (surface == NULL)
	{
		printf("SDL_GetWindowSurface error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	if (image == NULL)
	{
		printf("image error");
		system("pause");
		DeInit(1);
	}
}

void DrawImage(SDL_Surface* image, int x, int y)
{
	SDL_Rect i;
	i.x = x;
	i.y = y;

	SDL_BlitSurface(image, NULL, surface, &i);
}

int main(int argc, char* argv[])
{
	Init();

	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 148, 138, 84)); // заполнение фона

	SDL_SetColorKey(image, 1, SDL_MapRGB(image->format, 255, 255, 255)); // прозрачный цвет

	DrawImage(image, 295, 295); // фото

	SDL_UpdateWindowSurface(window);

	SDL_Delay(3000);

	DeInit(0);

	return 0;
}