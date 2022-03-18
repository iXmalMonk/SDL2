#include<SDL.h>
#include<iostream>
#include<Windows.h>

SDL_Window* window = NULL;
SDL_Renderer* render = NULL;

int window_w = 1000, window_h = 800;

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

	window = SDL_CreateWindow("Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

void MathCoordsToScreen(double x, double y, double scale, int centerx, int centery, int& sx, int& sy)
{
	sx = round(centerx + x * scale);
	sy = round(centery - y * scale);
}

int main(int argc, char* argv[])
{
	//
	bool HideConsole = true;

	if (HideConsole)
	{
		HWND HC;
		AllocConsole();
		HC = FindWindowA("ConsoleWindowClass", NULL);
		ShowWindow(HC, 0);
	}
	//

	Init();

	#pragma region VAR

	// MathCoordsToScreen
	double x1, y1, x2, y2, scale = 1.5;
	int sx1, sy1, sx2, sy2;
	// MathCoordsToScreen

	// Circle
	int wanted_points = 3;
	bool rising = true;
	// Circle

	// Screen and mouse coords
	int coords_x = window_w / 2, coords_y = window_h / 2;
	int mouse_x = window_w / 2, mouse_y = window_h / 2;
	// Screen and mouse coords

	// Square
	float square_x, square_y;
	square_x = window_w / 2;
	square_y = window_h / 2;
	float speed = 300;
	SDL_Rect square = { square_x, square_y, 20, 20 };
	// Square

	// Keyboard
	bool isUpPressed = false, isDownPressed = false, isRightPressed = false, isLeftPressed = false;

	const Uint8* state = SDL_GetKeyboardState(NULL);
	// Keyboard

	SDL_Event event;
	bool isRunning = true;
	int FPS = 30;

	#pragma endregion

	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					//window_w = event.window.data1;
					//window_h = event.window.data2;
					coords_x = event.window.data1 / 2;
					coords_y = event.window.data2 / 2;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					coords_x = event.button.x;
					coords_y = event.button.y;
				}
				break;

			case SDL_MOUSEMOTION:
				mouse_x = event.motion.x;
				mouse_y = event.motion.y;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					isRunning = false;
					break;
				/*case SDL_SCANCODE_RIGHT:
					isRightPressed = true;
					break;
				case SDL_SCANCODE_LEFT:
					isLeftPressed = true;
					break;
				case SDL_SCANCODE_UP:
					isUpPressed = true;
					break;
				case SDL_SCANCODE_DOWN:
					isDownPressed = true;
					break;*/
				}
			/*	break;
			
			case SDL_KEYUP:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_RIGHT:
					isRightPressed = false;
					break;
				case SDL_SCANCODE_LEFT:
					isLeftPressed = false;
					break;
				case SDL_SCANCODE_UP:
					isUpPressed = false;
					break;
				case SDL_SCANCODE_DOWN:
					isDownPressed = false;
					break;
			
				}
				break;*/
			}
		}

		#pragma region KEYBOARD

		/*if (isUpPressed and !isDownPressed) mouse_y-=5;
		if (!isUpPressed and isDownPressed) mouse_y+=5;
		if (isRightPressed and !isLeftPressed) mouse_x+=5;
		if (!isRightPressed and isLeftPressed) mouse_x-=5;*/

		
		if (state[SDL_SCANCODE_UP] and !state[SDL_SCANCODE_DOWN]) coords_y -= 5;
		if (!state[SDL_SCANCODE_UP] and state[SDL_SCANCODE_DOWN]) coords_y += 5;
		if (state[SDL_SCANCODE_RIGHT] and !state[SDL_SCANCODE_LEFT]) coords_x += 5;
		if (!state[SDL_SCANCODE_RIGHT] and state[SDL_SCANCODE_LEFT]) coords_x -= 5;

		#pragma endregion

		#pragma region DRAWING
		
		// Background
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);
		// Background

		// Ox Oy For Circle
		x1 = -250; x2 = 250;
		y1 = 0; y2 = 0;

		SDL_SetRenderDrawColor(render, 0, 0, 255, 255);

		MathCoordsToScreen(x1, y1, scale, coords_x, coords_y, sx1, sy1);
		MathCoordsToScreen(x2, y2, scale, coords_x, coords_y, sx2, sy2);

		SDL_RenderDrawLine(render, sx1, sy1, sx2, sy2);

		y1 = -250; y2 = 250;
		x1 = 0; x2 = 0;

		MathCoordsToScreen(x1, y1, scale, coords_x, coords_y, sx1, sy1);
		MathCoordsToScreen(x2, y2, scale, coords_x, coords_y, sx2, sy2);

		SDL_RenderDrawLine(render, sx1, sy1, sx2, sy2);
		// Ox Oy For Circle

		// Animation Circle From Lines
		int point_count = wanted_points;
		SDL_Point* points = (SDL_Point *)malloc(sizeof(SDL_Point)*(point_count + 1));

		double alpha = 0, R = 250;
		for (int i = 0; i < point_count; i++)
		{
			alpha += 2.0 * M_PI / point_count;
			MathCoordsToScreen(R * cos(alpha), R * sin(alpha), scale, coords_x, coords_y, points[i].x, points[i].y);
		}
		points[point_count] = points[0];

		SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
		SDL_RenderDrawLines(render, points, point_count+1);

		free(points);

		if (rising)
			wanted_points++;
		else
			wanted_points--;

		if (rising and wanted_points > 30 or !rising && wanted_points <= 3)
			rising = !rising;
		// Animation Circle From Lines

		// Square
		float x = mouse_x - square_x;
		float y = mouse_y - square_y;

		float len = sqrt(x * x + y * y);
		if (len > speed / FPS)
		{
			x /= len;
			y /= len;

			square_x += x * speed / FPS;
			square_y += y * speed / FPS;
		}
		else
		{
			square_x = mouse_x;
			square_y = mouse_y;
		}

		square.x = square_x;
		square.y = square_y;

		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderFillRect(render, &square);
		// Square

		#pragma endregion

		SDL_RenderPresent(render);

		SDL_Delay(1000 / FPS);

	}

	DeInit(0);
	return 0;
}