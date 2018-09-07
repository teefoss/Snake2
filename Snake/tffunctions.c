#include "tffunctions.h"


point_t MakePoint(float x, float y)
{
	point_t pt;
	
	pt.x = x;
	pt.y = y;
	return pt;
}

sizetype MakeSize(float w, float h)
{
	sizetype s;
	
	s.w = w;
	s.h = h;
	return s;
}


rect_t
MakeRect
( float x,
  float y,
  float w,
  float h )
{
	rect_t r;
	
	r.pt = MakePoint(x, y);
	r.size = MakeSize(w, h);
	return r;
}


rect_t MakeRectFromPoint(point_t pt, sizetype size)
{
	rect_t r;
	r.pt = pt;
	r.size = size;
	return r;
}
#pragma mark -



//
// TFRectToSDL
// Converts a rect_t to SDL_Rect
//
void TFRectToSDL(SDL_Rect *dest, rect_t *src)
{
	dest->x = src->pt.x;
	dest->y = src->pt.y;
	dest->w = src->size.w;
	dest->h = src->size.h;
}




//
//	TFBoxFromRect
///	Make a bounding box from given rect_t
//
void
TFBoxFromRect
( box_t 		*destbox,
  rect_t const 	*srcrect )
{
	destbox->left = srcrect->pt.x;
	destbox->right = srcrect->pt.x + srcrect->size.w;
	destbox->top = srcrect->pt.y;
	destbox->bottom = srcrect->pt.y + srcrect->size.h;
}




//
//	TFPointsAreEqual
//
bool TFPointsAreEqual(point_t *pt1, point_t *pt2)
{
	if (pt1->x == pt2->x && pt1->y == pt2->y)
		return true;
	return false;
}




bool TFRectsCollide(rect_t *aRect, rect_t *bRect)
{
	box_t abox, bbox;
	bool xaligned, yaligned;
	
	TFBoxFromRect(&abox, aRect);
	TFBoxFromRect(&bbox, bRect);
	xaligned = !(abox.top >= bbox.bottom || abox.bottom <= bbox.top);
	yaligned = !(abox.left >= bbox.right || abox.right <= bbox.left);
	
	if (xaligned && yaligned)
		return true;
	
	return false;
}
bool TFRectsCollide2(rect_t *aRect, rect_t *bRect)
{
	box_t abox, bbox;
	
	TFBoxFromRect(&abox, aRect);
	TFBoxFromRect(&bbox, bRect);

//	(!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
	return !(abox.left > bbox.right || bbox.left > abox.right ||
	abox.top > bbox.bottom || bbox.top > abox.bottom);
}


alignment_t TFRectAlignment(rect_t *aRect, rect_t *bRect)
{
	box_t abox, bbox;
	
	TFBoxFromRect(&abox, aRect);
	TFBoxFromRect(&bbox, bRect);

	if (abox.top >= bbox.top-aRect->size.h && abox.bottom <= bbox.bottom+aRect->size.h)
		return horizontal;
	else if (abox.right <= bbox.right-aRect->size.w && abox.left >= bbox.left+aRect->size.h)
		return vertical;
	else
		return none;
}


// Returns position of a relative to b
position_t TFRectPosition(rect_t *aRect, rect_t *bRect)
{
	box_t abox, bbox;
	
	TFBoxFromRect(&abox, aRect);
	TFBoxFromRect(&bbox, bRect);
	
	if (abox.right < bbox.left)
		return leftside;
	else if (abox.left > bbox.right)
		return rightside;
	
	if (abox.bottom < bbox.top)
		return above;
	else if (abox.top > bbox.bottom)
		return below;
	
	printf("oops\n");
	return 0;
}
