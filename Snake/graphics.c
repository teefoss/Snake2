//
//  graphics.c
//  Snake
//
//  Created by Thomas Foster on 8/31/18.
//  Copyright © 2018 Thomas Foster. All rights reserved.
//

#include "graphics.h"

byte headdata[] =
{
	00,00,00,10,10,00,00,00,
	00,00,00,10,10,00,00,00,
	00,00,10,10,10,10,00,00,
	00,10,10,10,10,10,10,00,
	10,15,00,10,10,00,15,10,
	10,15,15,10,10,15,15,10,
	00,10,10,10,10,10,10,00,
	00,00,10,10,10,10,00,00,
};

byte bodydata[] =
{
	00,00,10,10,10,10,00,00,
	00,10,10,06,06,10,10,00,
	10,10,06,10,10,06,10,10,
	10,06,10,10,10,10,06,10,
	10,06,10,10,10,10,06,10,
	10,10,06,10,10,06,10,10,
	00,10,10,06,06,10,10,00,
	00,00,10,10,10,10,00,00,
};

byte appledata[] =
{
	00,00,00,00,00,06,06,00,
	00,00,04,04,06,06,00,00,
	00,04,04,04,06,04,04,00,
	04,04,04,04,04,04,04,04,
	04,04,04,04,04,04,04,04,
	04,04,04,04,04,04,04,04,
	00,04,04,04,04,04,04,00,
	00,00,04,04,04,04,00,00,
};

byte deadheaddata[] =
{
	00,00,00,04,04,00,00,00,
	00,00,00,04,04,00,00,00,
	00,00,04,04,04,04,00,00,
	00,04,04,04,04,04,04,00,
	04,15,15,04,04,15,00,04,
	04,00,15,04,04,15,15,04,
	00,04,04,04,04,04,04,00,
	00,00,04,04,04,04,00,00,
};

byte deadbodydata[] =
{
	00,00,04,04,04,04,00,00,
	00,04,04,04,04,04,04,00,
	04,04,04,04,04,04,04,04,
	04,04,04,04,04,04,04,04,
	04,04,04,04,04,04,04,04,
	04,04,04,04,04,04,04,04,
	00,04,04,04,04,04,04,00,
	00,00,04,04,04,04,00,00,
};

graphic_t head_g;
graphic_t body_g;
graphic_t apple_g;
graphic_t deadhead_g;
graphic_t deadbody_g;

graphic_t G_NewGraphic (byte *data, int w, int h)
{
	graphic_t new;
	new.data = data;
	new.w = w;
	new.h = h;
	return new;
}



void G_Init (void)
{
	head_g = G_NewGraphic(headdata, 8, 8);
	body_g = G_NewGraphic(bodydata, 8, 8);
	apple_g = G_NewGraphic(appledata, 8, 8);
	deadhead_g = G_NewGraphic(deadheaddata, 8, 8);
	deadbody_g = G_NewGraphic(deadbodydata, 8, 8);
}



//
//	DrawGraphic
///	Draws graphic at screen point (x, y)
/// rotation is in degrees (0, 90, 180, 270) (clockwise)
//
void G_DrawGraphic(graphic_t *gr, int x, int y, int rotation)
{
	int row, col;
	int w,h;
	byte *data;
	
	w = gr->w;
	h = gr->h;
	
	data = gr->data;
	if (rotation == 0)
	{
		for (row=0; row<gr->h; row++)
			for (col=0; col<gr->w; col++)
			{
				if (*data)
				{
					SetVGADrawColor(*data);
					SDL_RenderDrawPoint(renderer, x+col, y+row);
				}
				data++;
			}
	}
	
	else if (rotation == 90)
	{
		for (col=w-1; col>=0; col--)
			for (row=0; row<w; row++)
			{
				if (*data)
				{
					SetVGADrawColor(*data);
					SDL_RenderDrawPoint(renderer, x+col, y+row);
				}
				data++;
			}
	}
	
	else if (rotation == 180)
	{
		for (row=h-1; row>=0; row--)
			for (col=w-1; col>=0; col--)
			{
				if (*data)
				{
					SetVGADrawColor(*data);
					SDL_RenderDrawPoint(renderer, x+col, y+row);
				}
				data++;
			}
	}
	
	else if (rotation == 270)
	{
		for (col=0; col<w; col++)
			for (row=h-1; row>=0; row--)
			{
				if (*data)
				{
					SetVGADrawColor(*data);
					SDL_RenderDrawPoint(renderer, x+col, y+row);
				}
				data++;
			}
	}
}


