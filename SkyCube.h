#pragma once
#include"Drawable.h"

#define DIST_FROM_SKY 200.0f
#define GROUND_TEXTURE_MULTIPLIER 500
#define SIZE_OF_GROUND 1000.0f

//!  A class used to draw the sky cube
class SkyCube : public Drawable
{
public:
	SkyCube();
	//! The function used to draw the object on the screen
	void Draw();
	//! Sets the center of the sky cube. Usually needs to be set to the position of the camera.
	/*!
		\param point, a point representing the center of the scky cube
	*/
	void SetPoz(Point point);
};
