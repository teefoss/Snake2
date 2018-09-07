//
//  graphics.h
//  Snake
//
//  Created by Thomas Foster on 8/31/18.
//  Copyright © 2018 Thomas Foster. All rights reserved.
//

#ifndef graphics_h
#define graphics_h

#include <stdio.h>
#include "sdl.h"

typedef struct
{
	int 	w, h;
	byte	*data;
} graphic_t;

extern graphic_t head_g;
extern graphic_t body_g;
extern graphic_t apple_g;
extern graphic_t deadhead_g;
extern graphic_t deadbody_g;

void G_Init (void);
void G_DrawGraphic (graphic_t *gr, int x, int y, int degrees);

#endif /* graphics_h */
