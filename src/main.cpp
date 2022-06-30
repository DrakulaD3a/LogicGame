#include "engine.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Forward function declarations
void Update(float dt);
void RenderFrame(float dt);

#define WW 1920
#define WH 1080

struct tile {
	SDL_Rect shell;
	bool elevated;
};

double det = 0.0009765625; // 1 / (32 * 16 - (-32 * 16));
int offset = 0; // WW / 2 - 32;


SDL_Point mouse, grid;
SDL_Texture* brick;
tile t[10][10];

//=============================================================================
int main(int argc, char* argv[])
{
	if (!InitSDL())
	{
		return 1;
	}

	if (!CreateWindow("The Game", WW, WH))
	{
		return 1;
	}


	brick = IMG_LoadTexture(gRenderer, "assets/Tile-test.png");

	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			t[x][y] = { { x * 32 + y * -32 + offset, x * 16 + y * 16, 64, 64 }, false };
		}
	}

	StartLoop(Update, RenderFrame);

	CleanUp();
	return 0;
}

//=============================================================================

void Update(float dt)
{
	GetMousePosition(&mouse.x, &mouse.y);
	if (IsKeyDown(SDL_SCANCODE_ESCAPE))
		ExitGame();




	grid.x = (int)(mouse.x * det * 32 + mouse.y * det * -16);
    grid.y = (int)(mouse.x * det * -16 + mouse.y * det * 32);



	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (i != grid.x || j != grid.y)
			{
				if (t[i][j].elevated)
				{
					t[i][j].shell.y += 10;
					t[i][j].elevated = false;
				}
				continue;
			}
			
			
			if (!t[i][j].elevated)
			{
				t[i][j].elevated = true;
				t[i][j].shell.y -= 10;
			}




			/*if(SDL_PointInRect(&mouse, &t[i][j].shell))
			{
				t[i][j].elevate = true;
			}

			if(t[i][j].elevate && !t[i][j].elevated)
			{
				t[i][j].elevate = false;
				t[i][j].elevated = true;
				t[i][j].shell.y -= 10;
			}
			else if(!SDL_PointInRect(&mouse, &t[i][j].shell) && t[i][j].elevated)
			{
				t[i][j].elevated = false;
				t[i][j].shell.y += 10;
			}*/
		}
	}
}

void RenderFrame(float interpolation)
{
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 65, 105, 225, 255);
	SDL_RenderClear(gRenderer);

	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
				SDL_RenderCopy(gRenderer, brick, NULL, &t[i][j].shell);
		}
	}
}