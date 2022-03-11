#include<SDL.h>
#include<iostream>

const int w = 1024;
const int h = 768;

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

	window = SDL_CreateWindow("Lab 3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
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

void Circle(int x_circle, int y_circle, int r)
{
	int x_point, y_point;
	for (int i = 0; i < 360; i++)
	{
		x_point = x_circle + r * cos(i * M_PI / 180);
		y_point = y_circle + r * sin(i * M_PI / 180);
		SDL_RenderDrawPoint(renderer, x_point, y_point);
	}
}

void ABCD(int& a, int& b, int& c, int& d)
{
	do {
		printf("������� A [-10;10] = ");
		scanf_s("%d", &a);
	} while (-10 > a or a > 10);

	do {
		printf("������� B [-10;10] = ");
		scanf_s("%d", &b);
	} while (-10 > b or b > 10);

	do {
		printf("������� C [-10;10] = ");
		scanf_s("%d", &c);
	} while (-10 > c or c > 10);

	do {
		printf("������� D [-10;30] = ");
		scanf_s("%d", &d);
	} while (-10 > d or d > 30);
}

int main(int argc, char* argv[])
{
	system("chcp 1251");
	system("cls");

	Init();

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);	//
	SDL_RenderClear(renderer);								// ���

	// �������� �����
	SDL_Rect rect, rect2;
	rect.x = 150;	// ����������
	rect.y = 300;	// ��������������
	rect.w = 100;	// ������
	rect.h = 100;	// ��������������

	rect2.x = 187.5;// ����������
	rect2.y = 350;	// �������������� 2
	rect2.w = 25;	// ������
	rect2.h = 25;	// �������������� 2

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);	// �������������
	SDL_RenderFillRect(renderer, &rect);					//
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);			// �����
	SDL_RenderDrawLine(renderer, 160, 300, 160, 250);		//
	SDL_RenderDrawLine(renderer, 240, 300, 240, 250);		//
	SDL_RenderDrawLine(renderer, 160, 250, 240, 250);		//
	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);	// ������������� 2
	SDL_RenderFillRect(renderer, &rect2);					//
	// �������� �����

	// ��������������� ����������
	SDL_SetRenderDrawColor(renderer, 75, 150, 225, 255);
	int r = 50;
	while (r > 0)
	{
		Circle(w / 2, h / 2, r);
		r -= 5;
	}
	// ��������������� ����������

	// ������ Y=Ax^3+Bx^2+Cx+D, [-10;10].
	int range_x1 = 500, range_x2 = 900, range_y1 = 300, range_y2 = 700, x0 = 750, y0 = 600, a, b, c, d;
	double x1, y1;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, range_x2, y0, range_x2 - 10, y0 - 10); // X ������� �����
	SDL_RenderDrawLine(renderer, range_x2, y0, range_x2 - 10, y0 + 10); // X ������� ����
	SDL_RenderDrawLine(renderer, x0, range_y1, x0 + 10, range_y1 + 10); // Y ������� ������
	SDL_RenderDrawLine(renderer, x0, range_y1, x0 - 10, range_y1 + 10); // Y ������� �����
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, range_x1, y0, range_x2, y0);	// X x0 = 750
	SDL_RenderDrawLine(renderer, x0, range_y1, x0, range_y2);	// Y y0 = 600
	SDL_Rect rect3 = { 0,0,3,3 };
	ABCD(a, b, c, d);
	for (x1 = -10; x1 <= 10; x1 += 0.01)
	{
		y1 = (a * x1 * x1 * x1) + (b * x1 * x1) + (c * x1) + d;
		rect3.x = x0 + x1 * 10;	// 10 ��������
		rect3.y = y0 - y1 * 10;	// 10 ��������
		if (rect3.y >= range_y1 and rect3.y <= range_y2)
			SDL_RenderFillRect(renderer, &rect3);
	}
	// ������ Y=Ax^3+Bx^2+Cx+D, [-10;10].

	SDL_RenderPresent(renderer);

	SDL_Delay(30000);

	DeInit(0);
	return 0;
}