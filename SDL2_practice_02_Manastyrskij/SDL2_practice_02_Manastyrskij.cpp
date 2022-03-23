#include<SDL.h>
#include<iostream>
#include<Windows.h>

#define OUTPUT_HANDLE -11

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int w = 1000, h = 800;

float radian = M_PI / 180.0;

struct Color {
	int red;
	int green;
	int blue;
};

struct Drop {
	int x;
	int y;
	int speed;
	Color color;
};

enum DirectionSnake { up, down, left, right };

struct Ball {
	int x;
	int y;
	int radius;
	int angle;
	int speed;
	Color color;
	bool direction_x;
	bool direction_y;
};

struct Oval {
	int radius;
	int speed;
	int tracer_length;
	float angle;
	Color color;
};

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

void Event(SDL_Event& event, bool& launched, bool& drop_launched, bool& snake_launched, bool& ball_launched, bool& ball_mode, bool& oval_launched)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			launched = false;
		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_1:
				if (!drop_launched)
					drop_launched = true;
				else
					drop_launched = false;
				break;
			case SDL_SCANCODE_2:
				if (!snake_launched)
					snake_launched = true;
				else
					snake_launched = false;
				break;
			case SDL_SCANCODE_3:
				if (!ball_launched)
					ball_launched = true;
				else
					ball_launched = false;
				break;
			case SDL_SCANCODE_E:
				if (!ball_mode) 
					ball_mode = true;
				else 
					ball_mode = false;
				break;
			case SDL_SCANCODE_4:
				if (!oval_launched)
					oval_launched = true;
				else
					oval_launched = false;
				break;
			}

			system("cls");

			SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 15);
			printf("\t[1] Drop \t");
			if (drop_launched)
			{
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 11);
				printf("launched\n");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 12);
				printf("unlaunched\n");
			}

			SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 15);
			printf("\t[2] Snake\t");
			if (snake_launched)
			{
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 11);
				printf("launched\t");
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 15);
				printf("[UP, DOWN, LEFT, RIGHT] Direction\n");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 12);
				printf("unlaunched\n");
			}

			SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 15);
			printf("\t[3] Ball \t");
			if (ball_launched)
			{
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 11);
				printf("launched\t\t\t    ");
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 15);
				printf("[E] Mode\t\t");
				if (ball_mode)
				{
					SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 11);
					printf("auto\n");
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 12);
					printf("non-auto\t");
					SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 15);
					printf("[SPACE] Boost\n");
				}
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 12);
				printf("unlaunched\n");
			}

			SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 15);
			printf("\t[4] Oval \t");
			if (oval_launched)
			{

				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 11);
				printf("launched\n");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 12);
				printf("unlaunched\n");
			}
			SetConsoleTextAttribute(GetStdHandle(OUTPUT_HANDLE), 7);
		}
		
	}
}

void Background(int color_red, int color_green, int color_blue)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void CreateDrop(const int drop_count, Drop drop[])
{
	for (int i = 0; i < drop_count; i++)
	{
		drop[i].x = rand() % w;
		drop[i].y = rand() % h;
		drop[i].speed = rand() % 26 + 25;
		drop[i].color.red = rand() % 256;
		drop[i].color.green = rand() % 256;
		drop[i].color.blue = rand() % 256;
	}
}

void DropF(const int drop_count, Drop drop[], float drop_angle, float drop_length, float drop_speed_scale)
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

void CreateSnake(int snake_size, SDL_Rect snake[], DirectionSnake& snake_direction, int snake_scale)
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

void SnakeF(const Uint8* keyboard, const int snake_delay, const int snake_size, SDL_Rect* snake, SDL_Rect* snake_temp, DirectionSnake& snake_direction, int snake_scale)
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
				CreateSnake(snake_size, snake, snake_direction, snake_scale);
				//printf("Snake died\n");
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

void Circle(int circle_position_x, int circle_position_y, int radius, int circle_color_red, int circle_color_green, int cirle_color_blue)
{
	SDL_SetRenderDrawColor(renderer, circle_color_red, circle_color_green, cirle_color_blue, 255);
	do {
		for (double angle = 0; angle <= 360; angle += 0.5)
			SDL_RenderDrawPoint(renderer, circle_position_x + radius * cos(angle * radian), circle_position_y + radius * sin(angle * radian));
		radius--;
	} while (radius != 0);
}

void CreateBall(Ball& ball)
{
	ball.x = rand() % w / 4 + w / 2;
	ball.y = rand() % h / 4 + h / 2;
	ball.radius = rand() % 26 + 25;
	ball.angle = rand() % 46 + 45;
	ball.speed = rand() % 11 + 40;
	ball.color.red = rand() % 256;
	ball.color.green = rand() % 256;
	ball.color.blue = rand() % 256;
	if (rand() % 2 == 0) ball.direction_x = true;
	else ball.direction_x = false;
	if (rand() % 2 == 0) ball.direction_y = true;
	else ball.direction_y = false;
}

void BallF(const Uint8* keyboard, Ball& ball, float ball_speed_scale, bool ball_mode)
{
	static float ball_first_speed = ball.speed;

	Circle(ball.x, ball.y, ball.radius, ball.color.red, ball.color.green, ball.color.blue);

	if (ball.x + ball.radius <= w and ball.direction_x == true) 
		ball.x += cos(ball.angle * radian) * ball.speed * ball_speed_scale;
	else
		ball.direction_x = false;

	if (ball.x - ball.radius >= 0 and ball.direction_x == false) 
		ball.x -= cos(ball.angle * radian) * ball.speed * ball_speed_scale;
	else
		ball.direction_x = true;

	if (ball.y + ball.radius <= h and ball.direction_y == true) 
		ball.y += sin(ball.angle * radian) * ball.speed * ball_speed_scale;
	else
		ball.direction_y = false;

	if (ball.y - ball.radius >= 0 and ball.direction_y == false)
		ball.y -= sin(ball.angle * radian) * ball.speed * ball_speed_scale;
	else
		ball.direction_y = true;
	
	if (ball.speed > 0) ball.speed -= 1;
	else if (keyboard[SDL_SCANCODE_SPACE] and !ball_mode) ball.speed = ball_first_speed;
	else if (ball_mode) ball.speed = ball_first_speed;
}

void CreateOval(const int oval_count, Oval oval[])
{
	for (int i = 0; i < oval_count; i++)
	{
		oval[i].radius = rand() % 26 + 25;
		oval[i].speed = rand() % 2 + 1;
		oval[i].tracer_length = rand() % 25 + 10;
		oval[i].angle = rand() % 361;
		oval[i].color.red = rand() % 256;
		oval[i].color.green = rand() % 256;
		oval[i].color.blue = rand() % 256;
	}
}

void OvalF(const int oval_count, Oval oval[], int oval_radius)
{
	for (int i = 0; i < oval_count; i++)
		if (oval[i].angle != 360) oval[i].angle += oval[i].speed;
		else oval[i].angle = 0;

	for (int i = 0; i < oval_count; i++)
	{
		Circle(w / 2 + oval_radius * cos(oval[i].angle * radian), h / 2 + oval_radius * sin(oval[i].angle * radian) / 2, oval[i].radius, oval[i].color.red, oval[i].color.green, oval[i].color.blue);
		SDL_SetRenderDrawColor(renderer, oval[i].color.red, oval[i].color.green, oval[i].color.blue, 255);
		for (double j = oval[i].angle; j != oval[i].angle - oval[i].tracer_length; j -= 0.5)
			SDL_RenderDrawPoint(renderer, w / 2 + oval_radius * cos(j * radian), h / 2 + oval_radius * sin(j * radian) / 2);
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

	srand(time(NULL));

	bool HideConsole = false;
	HWND HC;
	AllocConsole();
	HC = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(HC, !HideConsole);
#pragma endregion

#pragma region VAR DROP
	const int drop_count = 50;
	Drop drop[drop_count];
	float drop_angle = 60.0;
	float drop_length = 100.0;
	float drop_speed_scale = 1.0;
	CreateDrop(drop_count, drop);

	bool drop_launched = false;
#pragma endregion

#pragma region VAR SNAKE
	const int snake_delay = 3;
	const int snake_size = 8;
	SDL_Rect snake[snake_size];
	SDL_Rect snake_temp[snake_size - 1];
	DirectionSnake snake_direction;
	int snake_scale = 20;
	CreateSnake(snake_size, snake, snake_direction, snake_scale);

	bool snake_launched = false;
#pragma endregion

#pragma region VAR BALL
	Ball ball;
	float ball_speed_scale = 1.5;
	CreateBall(ball);

	bool ball_launched = false;
	bool ball_mode = true;
#pragma endregion

#pragma region VAR OVAL
	const int oval_count = 3;
	Oval oval[oval_count];
	int oval_radius = 500;
	CreateOval(oval_count, oval);

	bool oval_launched = false;
#pragma endregion

	while (launched)
	{
		Event(event, launched, drop_launched, snake_launched, ball_launched, ball_mode, oval_launched);

		Background(0, 0, 0);

		if (drop_launched) DropF(drop_count, drop, drop_angle, drop_length, drop_speed_scale);

		if (ball_launched) BallF(keyboard, ball, ball_speed_scale, ball_mode);

		if (oval_launched) OvalF(oval_count, oval, oval_radius);

		if (snake_launched) SnakeF(keyboard, snake_delay, snake_size, snake, snake_temp, snake_direction, snake_scale);

		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / fps);
	}

	DeInit(0);
	return 0;
}
