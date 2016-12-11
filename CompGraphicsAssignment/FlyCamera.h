#ifndef FLYCAMERA_H
#define FLYCAMERA_H

#include "Camera.h"

/*
This class is for a Fly Camera that wil allow the user to move the
camera around the scene
*/

class FlyCamera :  public Camera
{
public:
	// Constructor
	FlyCamera();
	~FlyCamera();

	// Update function which will contain all the movement functions
	void update(float deltaTime);
	
	// Set speed of Camera movement
	void setSpeed(float a_speed);

protected:
	float	m_speed;

};

#endif //FLYCAMERA_H