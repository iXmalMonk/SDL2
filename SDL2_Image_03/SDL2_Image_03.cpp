#include<SDL.h>
#include<SDL_image.h>
#include<iostream>

#define DIR_RIGHT 1
#define DIR_LEFT 2

typedef unsigned char byte;

SDL_Window* window = NULL;
SDL_Renderer* render = NULL;

int window_w = 1400, window_h = 900;

void DeInit(int error)
{
	if (render != NULL) SDL_DestroyRenderer(render);
	if (window != NULL) SDL_DestroyWindow(window);
	IMG_Quit();
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

	int img;
	if ((img = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) == 0)
	{
		printf("Error: %s", SDL_GetError());
		system("pause");
		DeInit(1);
	}
	if (img & IMG_INIT_PNG) printf("PNG\n");
	else printf("Error PNG\n");
	if (img & IMG_INIT_JPG) printf("JPG\n");
	else printf("Error JPG\n");

	window = SDL_CreateWindow("Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_w, window_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

SDL_Texture* LoadTextureFromFile(const char* filename, SDL_Rect* rect)
{
	SDL_Surface* surface = IMG_Load(filename);
	if (surface == NULL)
	{
		printf("Error %s - %s", SDL_GetError(), filename);
		system("pause");
		DeInit(1);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
	*rect = { 0, 0, surface->w, surface->h };
	SDL_FreeSurface(surface);
	return texture;
}

int main(int argc, char* argv[])
{
	const Uint8* state = SDL_GetKeyboardState(NULL);

	SDL_Event event;
	bool isRunning = true;
	int FPS = 30;

	int x = 0, y = 0;
	int delta = 10;
	double angle = 0;

	Init();

	SDL_Rect background_rect;
	SDL_Texture* background_texture = LoadTextureFromFile("background.png", &background_rect);

	SDL_Rect animation_rect;
	SDL_Texture* animation_texture = LoadTextureFromFile("animation.png", &animation_rect);
	animation_rect.w = animation_rect.h;

	SDL_Rect dst_rect;
	int frame = 0;
	int frame_count = 6;
	int current_frametime = 0;
	int max_frametime = 125;

	int lasttime = SDL_GetTicks();
	int newtime;
	int dt = 0;

	bool isup, isdown, isleft, isright;
	isup = isdown = isleft = isright = false;
	bool animate = false;

	int direction = 0;

	//SDL_Rect texture_rect;
	//SDL_Texture* texture = LoadTextureFromFile("troll-face.jpg", &texture_rect);

	//
	//byte* pixel;
	//for (int i = 0; i < surface->h; i++)
	//	for (int j = 0; j < surface->w; j++)
	//	{
	//		pixel = (byte*)surface->pixels + i * surface->pitch + j * 3;
	//		int average = (pixel[0] + pixel[1] + pixel[2]) / 3;
	//		pixel[0] = pixel[1] = pixel[2] = average;
	//	}
	//SDL_Surface* window_surface = SDL_GetWindowSurface(window);
	//


	while (isRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_UP:		isup = true; break;
				case SDL_SCANCODE_DOWN:		isdown = true; break;
				case SDL_SCANCODE_RIGHT:	isright = true; direction = DIR_RIGHT; break;
				case SDL_SCANCODE_LEFT:		isleft = true; direction = DIR_LEFT; break;
				case SDL_SCANCODE_PAGEUP:	angle += delta / 2; break;
				case SDL_SCANCODE_PAGEDOWN:	angle -= delta / 2; break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_UP:		isup = false; break;
				case SDL_SCANCODE_DOWN:		isdown = false; break;
				case SDL_SCANCODE_RIGHT:	isright = false; break;
				case SDL_SCANCODE_LEFT:		isleft = false; break;
				}
				break;
				
			}
		}

		//SDL_SetColorKey(surface, 1, SDL_MapRGBA(surface->format, 255, 255, 255, 255));
		//SDL_BlitSurface(surface, NULL, window_surface, &dstrect);
		//SDL_UpdateWindowSurface(window);

		// Background
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);
		// Background

		// Troll-face
		//texture_rect = { x, y, 300, 300 };
		//SDL_Rect dst_rect = { x, y, texture_rect.w, texture_rect.h };
		//SDL_RenderCopyEx(render, texture, &texture_rect, &dst_rect, angle, NULL, SDL_FLIP_NONE);
		// Troll-face
		
		newtime = SDL_GetTicks();
		dt = newtime - lasttime;
		lasttime = newtime;

		if (isup) y -= delta;
		if (isdown) y += delta;
		if (isright) x += delta;
		if (isleft) x -= delta;
		
		animate = isup or isdown or isright or isleft;

		if (animate)
		{
			current_frametime += dt;
			if (current_frametime > max_frametime)
			{
				current_frametime -= max_frametime;
				frame = (frame + 1) % frame_count;
				animation_rect.x = animation_rect.w * frame;
			}
		}

		dst_rect = { x, y, animation_rect.w, animation_rect.h };
		SDL_RenderCopy(render, background_texture, &background_rect, NULL);

		if(direction == DIR_RIGHT)
			SDL_RenderCopy(render, animation_texture, &animation_rect, &dst_rect);
		else
			SDL_RenderCopyEx(render, animation_texture, &animation_rect, &dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);

		SDL_RenderPresent(render);

		SDL_Delay(1000 / FPS);
	}

	//SDL_DestroyTexture(texture);
	SDL_DestroyTexture(animation_texture);
	SDL_DestroyTexture(background_texture);
	//SDL_FreeSurface(surface);
	DeInit(0);
	return 0;
}