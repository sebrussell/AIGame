#include "Image.h"

Image::Image()
{
	animationDelay = 1;
}
										//constructor and deconstructor
Image::~Image()
{
}

void Image::SetBitmap(const char* name)
{
	bitmap = al_load_bitmap(name);		//loads the bitmap
	if (!bitmap)						//checks it exists
	{
		fprintf(stderr, "failed to load relevant bitmap!\n");
    }
	al_convert_mask_to_alpha(bitmap, al_map_rgb(255, 0, 255));		//converts the pink of the bitmap to the allegro 5 standard which uses alpha channels
}

void Image::SetImageValues(float sX, float sY, float sWidth, float sHeight, float cX, float cY, float mX, float mY, float delayInSeconds, float hexagonWidth)
{
	sourceX = sX;
	sourceY = sY;
	sourceWidth = sWidth;
	sourceHeight = sHeight;
	currentX = cX;												//a function to set all the image values, used by player and zombie
	currentY = cY;
	maxX = mX;
	maxY = mY;
	animationDelay = delayInSeconds * 1000;
	imageScale = hexagonWidth / (sourceWidth * 1.5);
}

void Image::NextAnimation()
{
	if (timeSinceAnimationUpdate >= animationDelay)				//if the time is up to change
	{
		timeSinceAnimationUpdate = 0;					//reset the animation timer
		begin = clock();								//begin the timer again
		if (currentX + sourceWidth >= maxX)				
		{
			currentX = sourceX;							//change the animation to a different picture
		}
		else
		{
			currentX += sourceWidth;
		}
	}
	end = clock();
	double elapsed_time = double(end - begin) / CLOCKS_PER_SEC;
	timeSinceAnimationUpdate += elapsed_time;
}

void Image::DrawAnimation(Cell::Position position)
{
	//al_draw_tinted_scaled_rotated_bitmap_region(bitmap, currentX, currentY, sourceWidth, sourceHeight, al_map_rgba_f(1, 1, 1, 1), position.m_x, position.m_y,
		//(position.m_x / imageScale) - (sourceWidth * 0.5 * imageScale), (position.m_y / imageScale) - (sourceHeight * 0.5 * imageScale), imageScale, imageScale, 0, 0);

	al_draw_tinted_scaled_rotated_bitmap_region(bitmap, currentX, currentY, sourceWidth, sourceHeight, al_map_rgba_f(1, 1, 1, 1), 0, 0,
		position.m_x - (sourceWidth / 2), position.m_y - (sourceHeight / 2), 1, 1, 0, 0);															//draw animation function

	//al_draw_circle(position.m_x, position.m_y, 1, al_map_rgb(255, 0, 0), 5);

	//al_draw_bitmap_region(bitmap, currentX, currentY, sourceWidth, sourceHeight, position.m_x, position.m_x, NULL);
	
	NextAnimation();			//calls the next animation function, to change the picture
}
