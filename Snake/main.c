//
//  main.c
//  Snake
//
//  Created by Thomas Foster on 8/31/18.
//  Copyright © 2018 Thomas Foster. All rights reserved.
//

#include <stdio.h>
#include "sdl.h"
#include "graphics.h"

#define MAXLENGTH 		1200
#define CYCLE			15
#define ENDCYCLE		120

typedef struct
{
	int 	numsegs;
	int		dx, dy;
	point_t	segs[MAXLENGTH];
	enum	{ inactive, moving, dying, dead } state;
	int		headangle;
} snake_t;

Mix_Chunk	*dead_s;
Mix_Chunk	*pickup_s;
Mix_Chunk	*disappear_s;

enum		{ title_st, game_st, scores_st } gamestate;
snake_t		snake;
int			tics;
bool		keypressed;
point_t		apple;


void Quit (void)
{
	Mix_FreeChunk(dead_s);
	Mix_FreeChunk(pickup_s);
	Mix_FreeChunk(disappear_s);
	StopSDL();
	exit(0);
}


#pragma mark -

void ChangeSpeed (int dx, int dy)
{
	snake.dx = dx;
	snake.dy = dy;
	keypressed = true;
	if (snake.state == inactive)
		snake.state = moving;
}

void ProcessKeyDown (SDL_Event *ev)
{
	if (keypressed)
		return;
	
	switch (ev->key.keysym.sym)
	{
		case SDLK_ESCAPE:
			Quit();
			break;
		case SDLK_UP:
			if (!snake.dy)
				ChangeSpeed(0, -8);
			break;
		case SDLK_DOWN:
			if (!snake.dy)
				ChangeSpeed(0, 8);
			break;
		case SDLK_LEFT:
			if (!snake.dx)
				ChangeSpeed(-8, 0);
			break;
		case SDLK_RIGHT:
			if (!snake.dx)
				ChangeSpeed(8, 0);
			break;

		default:
			break;
	}
}

void ProcessEvents (SDL_Event *ev)
{
	while (SDL_PollEvent(ev))
	{
		if (ev->type == SDL_QUIT)
			Quit();
		if (ev->type == SDL_KEYDOWN)
			ProcessKeyDown(ev);
	}
}




#pragma mark -

void KillSnake (void)
{
	snake.state = dying;
	Mix_PlayChannel(-1, dead_s, 0);
	tics = 0;
}

void ProcessGame (void)
{
	int 	i, j;
	point_t	*seg;
	
	tics++;
	
	// not moving yet, do nothing
	if (snake.state == inactive)
		return;
	
	// just hit self, freeze for 1 frame
	if (snake.state == dying)
	{
		if (tics == ENDCYCLE) {
			Mix_PlayChannel(-1, disappear_s, 0);
			snake.state = dead;
			tics = 0;
		}
		return;
	}
	
	if (snake.state == dead)
	{
		if (tics == ENDCYCLE)
			Quit(); // TODO display scores
		return;
	}
	
	//
	// active game:
	//
	
	if (tics % CYCLE != 0)
		return;
	
	if ( snake.dy < 0 )	snake.headangle = 0;
	if ( snake.dy > 0 )	snake.headangle = 180;
	if ( snake.dx < 0 ) snake.headangle = 270;
	if ( snake.dx > 0 ) snake.headangle = 90;
	
	// move the snake
	
	for ( i=snake.numsegs-1 ; i>=0 ; i-- )
	{
		if ( i == 0 ) // head
		{
			snake.segs[i].x += snake.dx; // move
			snake.segs[i].y += snake.dy;
			
			if ( snake.segs[i].x < 0 || snake.segs[i].x > SCREENWIDTH-8 ) {
				KillSnake();
				return;
			}
			else if ( snake.segs[i].y < 0 || snake.segs[i].y > SCREENHEIGHT-8 ) {
				KillSnake();
				return;
			}
			
			// pick up apple
			if ( TFPointsAreEqual ( &snake.segs[i], &apple ) )
			{
				Mix_PlayChannel ( -1, pickup_s, 0 );
				snake.numsegs++; // grow!
				snake.segs[ snake.numsegs-1 ].x = -10;
				snake.segs[ snake.numsegs-1 ].y = -10;
				apple.x = apple.y = -10;
				do {
					apple = MakePoint ( (rand()%40)*8, (rand()%30)*8 );
				} while ( TFPointsAreEqual ( &apple, &snake.segs[0] ) );
			}
			
			// check for collision with body
			for ( j=1 ; j<snake.numsegs ; j++ )
			{
				seg = &snake.segs[j];
				if ( TFPointsAreEqual ( &snake.segs[0], seg ) )
				{
					KillSnake();
					return;
				}
			}
		}
		else // body
		{
			snake.segs[i].x = snake.segs[i-1].x;
			snake.segs[i].y = snake.segs[i-1].y;
		}
		
	}
	
	tics = 0;
	keypressed = false;
}



#pragma mark -

void DisplaySnakeHead
( int x,
  int y )
{
	if ( snake.state < dying )
		G_DrawGraphic ( &head_g, x, y, snake.headangle );
	else if (snake.state == dying)
		G_DrawGraphic ( &deadhead_g, x, y, snake.headangle );
}



void
DisplaySnakeBody
( int x,
  int y )
{
	if ( snake.state < dying )
		G_DrawGraphic ( &body_g, x, y, snake.headangle );
	else if (snake.state == dying)
		G_DrawGraphic ( &deadbody_g, x, y, snake.headangle );
}



void Display (void)
{
	int i;
	
	ClearScreenVGAColor ( VGA_BLACK );
	
	SetVGADrawColor ( VGA_WHITE );
	for ( i=snake.numsegs-1 ; i>=0 ; i-- )
	{
		if ( i == 0 )
			DisplaySnakeHead ( snake.segs[i].x, snake.segs[i].y );
		else
			DisplaySnakeBody( snake.segs[i].x, snake.segs[i].y );
	}
	
	G_DrawGraphic ( &apple_g, apple.x, apple.y, 0 );
	
	SDL_RenderPresent ( renderer );
}


#pragma mark -

int
main
( int 			argc,
  const char 	*argv[] )
{
	SDL_Event 	ev;
	int			i;
	
	StartSDL();
	G_Init();
	
	dead_s = 		Mix_LoadWAV("dead.wav");
	pickup_s = 		Mix_LoadWAV("pickup.wav");
	disappear_s = 	Mix_LoadWAV("disappear.wav");
	
	gamestate = game_st;
	
	// init snake
	snake.state = inactive;
	snake.numsegs = 4;
	snake.dx = snake.dy = 0;
	for ( i=0 ; i<4 ; i++ )
	{
		snake.segs[i].x = SCREENWIDTH / 2;
		snake.segs[i].y = SCREENHEIGHT / 2;
	}
	
	apple = MakePoint ( (rand () % 40) * 8, (rand() % 30) * 8 );
	
	while (1)
	{
		StartFrame();
		
		ProcessEvents ( &ev );
		ProcessGame();
		Display();
		
		LimitFrameRate();
	}
	
	return 0;
}
