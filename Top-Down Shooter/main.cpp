#include<SDL.h>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

struct Keyboard {
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
};

void deInit(int error, SDL_Window*& window, SDL_Renderer*& renderer)
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

void init(SDL_Window*& window, SDL_Renderer*& renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) != NULL)
	{
		printf("SDL_Init error: %s", SDL_GetError());
		system("pause");
		deInit(1, window, renderer);
	}

	window = SDL_CreateWindow("Top-Down Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("SDL_CreateWindow error: %s", SDL_GetError());
		system("pause");
		deInit(1, window, renderer);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL)
	{
		printf("SDL_CreateRenderer error: %s", SDL_GetError());
		system("pause");
		deInit(1, window, renderer);
	}
}

void background(SDL_Renderer* renderer, int color_red, int color_green, int color_blue)
{
	SDL_SetRenderDrawColor(renderer, color_red, color_green, color_blue, 255);
	SDL_RenderClear(renderer);
}

void eventLoop(SDL_Event& event, bool& gameloop_launched, Keyboard& keyboard)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameloop_launched = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_W:
				keyboard.up = true;
				break;
			case SDL_SCANCODE_S:
				keyboard.down = true;
				break;
			case SDL_SCANCODE_A:
				keyboard.left = true;
				break;
			case SDL_SCANCODE_D:
				keyboard.right = true;
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_W:
				keyboard.up = false;
				break;
			case SDL_SCANCODE_S:
				keyboard.down = false;
				break;
			case SDL_SCANCODE_A:
				keyboard.left = false;
				break;
			case SDL_SCANCODE_D:
				keyboard.right = false;
				break;
			}
			break;
		}
	}
}

void logicPlayer(SDL_Rect& player_hitbox, Keyboard keyboard)
{
	if (keyboard.up and !keyboard.down) player_hitbox.y -= 1;
	if (keyboard.down and !keyboard.up) player_hitbox.y += 1;
	if (keyboard.left and !keyboard.right) player_hitbox.x -= 1;
	if (keyboard.right and !keyboard.left) player_hitbox.x += 1;
}

void drawPlayer(SDL_Renderer* renderer, SDL_Rect player_hitbox)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &player_hitbox);
}

int main(int argc, char* argv[])
{
	// -----
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Event event;
	SDL_Rect player_hitbox = { WIDTH / 2, HEIGHT / 2, 25, 25 };

	const unsigned short int fps = 30;

	bool gameloop_launched = true;

	Keyboard keyboard;
	// -----

	init(window, renderer);

	while (gameloop_launched)
	{
		background(renderer, 0, 0, 0);

		eventLoop(event, gameloop_launched, keyboard);

		logicPlayer(player_hitbox, keyboard);

		drawPlayer(renderer, player_hitbox);

		SDL_RenderPresent(renderer);

		SDL_Delay(fps / 1000);
	}

	deInit(0, window, renderer);

	return 0;
}