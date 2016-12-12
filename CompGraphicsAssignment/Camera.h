#ifndef CAMERA_H
#define	CAMERA_H

#include <glm\glm.hpp>
#include <glm\ext.hpp>

/*
This class define the base camera class for all other Cameras in the Game
*/

using glm::vec3;
using glm::mat4;

class Camera
{
public:
	//Constructors
	Camera();
	Camera(const vec3 a_FROM, const vec3& a_TO, const vec3& a_UP,
		float a_fov, float a_aspectRatio, float a_near, float a_far);
	~Camera();

	// Abstract update funciton for each camera
	virtual void update(float deltaTime) = 0;

	//Setter Functions
	void setPerspective(float a_fov, float a_aspectRatio, float a_near, float a_far); 
	void setLookAt(const vec3& a_FROM, const vec3& a_TO, const vec3& a_UP); 
	void setPosiition(vec3 a_position); 

	//Get Functions
	mat4 getWorldTransform(); 
	mat4 getView(); 
	mat4 getProjection(); 
	mat4 getProjectionView(); 
	vec3 getPosition();

protected:
	// Transforms
	mat4	m_worldTransform; 
	mat4	m_viewTransform; 
	mat4	m_projectionTransform;
	mat4	m_projectionViewTransform;

	// Position
	vec3	m_position;

	// Camera's 'up' direction
	vec3	m_up = vec3(0, 1, 0);
	vec3	m_eye;//from,position
	vec3	m_look;//looking at vector

	// Update the projection view transform
	void updateProjectionView();
};

#endif //CAMERA_H