#include<SDL.h>
#include<iostream>
#include<Windows.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int w = 900, h = 900;

float radian = M_PI / 180.0;

struct Color {
	int red;
	int green;
	int blue;
};

struct Drop {
	float x;
	float y;
	float speed;
	Color color;
};

enum DirectionSnake { up, down, left, right };

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

void CreateDrop(const int drop_count, Drop drop[])
{
	for (int i = 0; i < drop_count; i++)
	{
		drop[i].x = rand() % w;
		drop[i].y = rand() % h;
		drop[i].speed = rand() % 26 + 25; // 25 or 50;
		drop[i].color.red = rand() % 76; // 0 or 75
		drop[i].color.green = rand() % 76; // 0 or 75
		drop[i].color.blue = rand() % 56 + 200; // 200 or 255;
	}
}

void DrawDrop(const int drop_count, Drop drop[], float drop_angle, float drop_length, float drop_speed_scale)
{
	for (int i = 0; i < drop_count; i++)
	{
		SDL_SetRenderDrawColor(renderer, drop[i].color.red, drop[i].color.green, drop[i].color.blue, 255);
		SDL_RenderDrawLine(renderer, drop[i].x, drop[i].y, drop[i].x - cos(drop_angle * radian) * drop_length, drop[i].y - sin(drop_angle * radian) * drop_length);

		drop[i].x += cos(drop_angle * radian) * drop[i].speed * drop_speed_scale;
		drop[i].y += sin(drop_angle * radian) * drop[i].speed * drop_speed_scale;

		if (drop[i].x - cos(drop_angle * radian) * drop_length >= w)
			drop[i].x = 0;
		else if (drop[i].x - cos(drop_angle * radian) * drop_length <= -w)
			drop[i].x = w;
		else if (drop[i].y - sin(drop_angle * radian) * drop_length >= h)
			drop[i].y = 0;
		else if (drop[i].y - sin(drop_angle * radian) * drop_length <= -h)
			drop[i].y = h;
	}
}

void GenerateStartPositionForSnakeOnCenter(int snake_size, SDL_Rect snake[], DirectionSnake& snake_direction, int snake_scale)
{
	for (int i = 0, j = 0; i < snake_size; i++, j += snake_scale)
	{
		snake[i].x = w / 2 - snake_scale / 2;
		snake[i].y = h / 2 - snake_scale / 2 + j;
		snake[i].w = snake_scale;
		snake[i].h = snake_scale;
	}
	snake_direction = up;
}

void Snake(const Uint8* keyboard, const int snake_delay, const int snake_size, SDL_Rect* snake, SDL_Rect* snake_temp, DirectionSnake& snake_direction, int snake_scale)
{
	static int snake_delay_counter = 0;

	if (keyboard[SDL_SCANCODE_UP] and snake_direction != down) snake_direction = up;
	else if (keyboard[SDL_SCANCODE_DOWN] and snake_direction != up) snake_direction = down;
	else if (keyboard[SDL_SCANCODE_LEFT] and snake_direction != right) snake_direction = left;
	else if (keyboard[SDL_SCANCODE_RIGHT] and snake_direction != left) snake_direction = right;

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

		for (int i = 1; i < snake_size; i++)
			if (snake[0].x == snake[i].x and snake[0].y == snake[i].y)
			{
				GenerateStartPositionForSnakeOnCenter(snake_size, snake, snake_direction, snake_scale);
				printf("Snake died\n");
			}
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
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	SDL_Event event;

	bool HideConsole = false;
	HWND HC;
	AllocConsole();
	HC = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(HC, !HideConsole);
#pragma endregion

#pragma region VAR DROPS
	const int drop_count = 10;
	Drop drop[drop_count];
	float drop_angle = 60.0;
	float drop_length = 100.0;
	float drop_speed_scale = 1.0;
	CreateDrop(drop_count, drop);
#pragma endregion

#pragma region VAR SNAKE
	const int snake_delay = 8;
	const int snake_size = 5;
	SDL_Rect snake[snake_size];
	SDL_Rect snake_temp[snake_size - 1];
	DirectionSnake snake_direction;
	int snake_scale = 20;
	GenerateStartPositionForSnakeOnCenter(snake_size, snake, snake_direction, snake_scale);
#pragma endregion

	while (launched)
	{
		Event(event, launched);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		DrawDrop(drop_count, drop, drop_angle, drop_length, drop_speed_scale);

		Snake(keyboard, snake_delay, snake_size, snake, snake_temp, snake_direction, snake_scale);

		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / fps);
	}

	DeInit(0);
	return 0;
}
