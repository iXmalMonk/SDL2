#include<SDL.h>
#include<iostream>
#include<Windows.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int w = 900, h = 900;

float radian = M_PI / 180.0;

struct Drop {
	float x1;
	float y1;
	float x2;
	float y2;
};

enum DirectionSnake { up, down, left, right};

void DeInit(int error)
{
	if (renderer != NULL) SDL_DestroyRenderer(renderer);
	if (window != NULL) SDL_DestroyWindow(window);
	SDL_Quit();
	exit(error);
}

void Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != NULL)
	{
		printf("SDL_Init error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}

	window = SDL_CreateWindow("Practice", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
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

void GenerateDrop(Drop drop[], int i, float drop_angle, float drop_length)
{
	if (rand() % 2 == 0)
	{
		drop[i].x1 = rand() % (w+1);
		drop[i].y1 = 0.0;
	}
	else
	{
		drop[i].x1 = 0.0;
		drop[i].y1 = rand() % (h+1);
	}

	drop[i].x2 = drop[i].x1 - cos(drop_angle * radian) * drop_length;
	drop[i].y2 = drop[i].y1 - sin(drop_angle * radian) * drop_length;
}

void Drops(const int drop_count, Drop* drop, float drop_speed, float drop_angle, float drop_length)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	for (int i = 0; i < drop_count; i++)
	{
		SDL_RenderDrawLine(renderer, drop[i].x1, drop[i].y1, drop[i].x2, drop[i].y2);

		drop[i].x1 += drop_speed;
		drop[i].x2 += drop_speed;
		drop[i].y1 += drop_speed;
		drop[i].y2 += drop_speed;

		if (drop[i].x1 >= w or drop[i].y1 >= h) GenerateDrop(drop, i, drop_angle, drop_length);
	}
}

void GenerateStartPositionForSnakeOnCenter(SDL_Rect snake[], int snake_size, int snake_scale)
{
	for (int i = 0, j = 0; i < snake_size; i++, j+= snake_scale)
	{
		snake[i].x = w/2 - snake_scale / 2;
		snake[i].y = h/2 - snake_scale / 2 + j;
		snake[i].w = snake_scale;
		snake[i].h = snake_scale;
	}
}

void Snake(const Uint8* keyboard, const int snake_size, SDL_Rect* snake, SDL_Rect* snake_temp, DirectionSnake& snake_direction, int snake_scale, const int snake_delay, int& snake_delay_counter)
{
	if (keyboard[SDL_SCANCODE_UP] and snake_direction != down) snake_direction = up;
	if (keyboard[SDL_SCANCODE_DOWN] and snake_direction != up) snake_direction = down;
	if (keyboard[SDL_SCANCODE_LEFT] and snake_direction != right) snake_direction = left;
	if (keyboard[SDL_SCANCODE_RIGHT] and snake_direction != left) snake_direction = right;

	if (snake_delay_counter == snake_delay)
	{
		for (int i = 0; i < snake_size - 1; i++) snake_temp[i] = snake[i];

		if (snake_direction == up)
		{
			snake[0].y -= snake_scale;
			for (int i = 1; i < snake_size; i++)
				snake[i] = snake_temp[i - 1];
		}
		else if (snake_direction == down)
		{
			snake[0].y += snake_scale;
			for (int i = 1; i < snake_size; i++)
				snake[i] = snake_temp[i - 1];
		}
		else if (snake_direction == left)
		{
			snake[0].x -= snake_scale;
			for (int i = 1; i < snake_size; i++)
				snake[i] = snake_temp[i - 1];
		}
		else
		{
			snake[0].x += snake_scale;
			for (int i = 1; i < snake_size; i++)
				snake[i] = snake_temp[i - 1];
		}
		snake_delay_counter = 0;

		if (snake[0].x <= -snake_scale) snake[0].x = w - snake_scale;
		else if (snake[0].x >= w) snake[0].x = 0;
		else if (snake[0].y <= -snake_scale) snake[0].y = h - snake_scale;
		else if (snake[0].y >= h) snake[0].y = 0;
	}
	snake_delay_counter++;

	
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (int i = 0; i < snake_size; i++)
		SDL_RenderFillRect(renderer, &snake[i]);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (int i = 1; i < snake_size; i++)
		SDL_RenderDrawRect(renderer, &snake[i]);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &snake[0]);
}

void Event(SDL_Event& event, bool& launched)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			launched = false;
		}
	}
}

int main(int argc, char* argv[])
{
	Init();

	#pragma region VAR
	bool launched = true;
	const int fps = 30;
	SDL_Event event;
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);

	bool HideConsole = true;
	if (HideConsole)
	{
		HWND HC;
		AllocConsole();
		HC = FindWindowA("ConsoleWindowClass", NULL);
		ShowWindow(HC, 0);
	}
	#pragma endregion

	#pragma region VAR DROPS
	const int drop_count = 50;
	Drop drop[drop_count];
	float drop_speed = 25.0;
	float drop_angle = 45.0;
	float drop_length = 100.0;
	
	for (int i = 0; i < drop_count; i++)
		GenerateDrop(drop, i, drop_angle, drop_length);
	#pragma endregion

	#pragma region VAR SNAKE
	const int snake_size = 3;
	SDL_Rect snake[snake_size];
	SDL_Rect snake_temp[snake_size - 1];
	DirectionSnake snake_direction = up;
	int snake_scale = 20;

	const int snake_delay = 5;
	int snake_delay_counter = 0;

	GenerateStartPositionForSnakeOnCenter(snake, snake_size, snake_scale);
	#pragma endregion

	while (launched)
	{
	Event(event, launched);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	Drops(drop_count, drop, drop_speed, drop_angle, drop_length);

	Snake(keyboard, snake_size, snake, snake_temp, snake_direction, snake_scale, snake_delay, snake_delay_counter);

	SDL_RenderPresent(renderer);
	SDL_Delay(1000 / fps);
	}

	DeInit(0);
	return 0;
}