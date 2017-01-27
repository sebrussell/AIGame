#pragma once

#include <allegro5\allegro.h>
#include <stdio.h>
#include <ctime>						//the header files it uses 
#include "Cell.h"

class Image
{
protected:
	Image();
	~Image();												//because I want the children of this class to inherit the functions I have used protected
	virtual void SetBitmap(const char* name);
	virtual void SetImageValues(float sX, float sY, float sWidth, float sHeight, float cX, float cY, float mX, float mY, float delayInSeconds, float hexagonWidth);
	void DrawAnimation(Cell::Position playerPosition);
private:
	void NextAnimation();
	ALLEGRO_BITMAP * bitmap;																														//private variblaes of the class
	float sourceX, sourceY, sourceWidth, sourceHeight, currentX, currentY, maxX, maxY, animationDelay, timeSinceAnimationUpdate, imageScale;
	std::clock_t begin, end;
};

