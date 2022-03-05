#include<SDL.h>
#include<iostream>

SDL_Window* window = NULL;
SDL_Renderer* render = NULL;

int window_w = 1024, window_h = 768;

void DeInit(int error)
{
	if (render != NULL) SDL_DestroyRenderer(render);
	if (window != NULL) SDL_DestroyWindow(window);
	SDL_Quit();
	exit(error);
}

void Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	window = SDL_CreateWindow("Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_w, window_h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (render == NULL)
	{
		printf("Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}
}

double Circle(double x, double x0, double y0, double r)
{
	return sqrt(r * r - (x - x0) * (x - x0)) + y0;
}

void MathCoordsToScreen(double x, double y, double scale, int centerx, int centery, int &sx, int &sy)
{
	sx = round(centerx + x * scale);
	sy = round(centery - y * scale);
}

int main(int argc, char* argv[])
{
	Init();

	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);

	double x1, y1, x2, y2, scale = 2.0;
	int sx1, sy1, sx2, sy2;

	SDL_Rect a = { 0, 0, 3, 3 };
	
	#pragma region FIRST METHOD
	x1 = -100; x2 = 100;
	y1 = 0; y2 = 0;

	SDL_SetRenderDrawColor(render, 0, 0, 255, 255);

	MathCoordsToScreen(x1, y1, scale, window_w / 4, window_h / 2, sx1, sy1);
	MathCoordsToScreen(x2, y2, scale, window_w / 4, window_h / 2, sx2, sy2);

	SDL_RenderDrawLine(render, sx1, sy1, sx2, sy2);

	y1 = -100; y2 = 100;
	x1 = 0; x2 = 0;

	MathCoordsToScreen(x1, y1, scale, window_w / 4, window_h / 2, sx1, sy1);
	MathCoordsToScreen(x2, y2, scale, window_w / 4, window_h / 2, sx2, sy2);

	SDL_RenderDrawLine(render, sx1, sy1, sx2, sy2);

	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);

	for (x1 = -100; x1 <= 100; x1 += 0.005)
	{
		y1 = Circle(x1, 0, 0, 100);
		MathCoordsToScreen(x1, y1, scale, window_w / 4, window_h / 2, sx1, sy1);
		a.x = sx1 - 1;
		a.y = sy1 - 1;
		SDL_RenderFillRect(render, &a);

		y1 = -y1;
		MathCoordsToScreen(x1, y1, scale, window_w / 4, window_h / 2, sx1, sy1);
		a.x = sx1 - 1;
		a.y = sy1 - 1;
		SDL_RenderFillRect(render, &a);
	}

	#pragma endregion

	#pragma region SECOND METHOD
	x1 = -100; x2 = 100;
	y1 = 0; y2 = 0;

	SDL_SetRenderDrawColor(render, 0, 0, 255, 255);

	MathCoordsToScreen(x1, y1, scale, window_w / 4 * 3, window_h / 2, sx1, sy1);
	MathCoordsToScreen(x2, y2, scale, window_w / 4 * 3, window_h / 2, sx2, sy2);

	SDL_RenderDrawLine(render, sx1, sy1, sx2, sy2);

	y1 = -100; y2 = 100;
	x1 = 0; x2 = 0;

	MathCoordsToScreen(x1, y1, scale, window_w / 4 * 3, window_h / 2, sx1, sy1);
	MathCoordsToScreen(x2, y2, scale, window_w / 4 * 3, window_h / 2, sx2, sy2);

	SDL_RenderDrawLine(render, sx1, sy1, sx2, sy2);

	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);

	for (double alpha = 0; alpha <= 360; alpha += 0.5)
	{
		x1 = 100 * cos(alpha * M_PI / 180);
		y1 = 100 * sin(alpha * M_PI / 180);

		MathCoordsToScreen(x1, y1, scale, window_w / 4 * 3, window_h / 2, sx1, sy1);
		a.x = sx1 - 1;
		a.y = sy1 - 1;
		SDL_RenderFillRect(render, &a);
	}

	#pragma endregion

	SDL_RenderPresent(render);

	SDL_Delay(10000);

	DeInit(0);
	return(0);
}