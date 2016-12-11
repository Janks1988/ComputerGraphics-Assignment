#include "Camera.h"


// Contstructors
// has a default constructor and desructor
// Will also allow user to ser their own initial position

#pragma region Contstructors

Camera::Camera()
{
	setLookAt(glm::vec3(10, 10, 10),
		glm::vec3(0),
		glm::vec3(0, 1, 0));
	setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	updateProjectionView();

	m_worldTransform = glm::inverse(m_viewTransform);
}

Camera::Camera(const vec3 a_FROM, const vec3 & a_TO, const vec3 & a_UP, 
	float a_fov, float a_aspectRatio, float a_near, float a_far)
{
	setLookAt(a_FROM, a_TO, a_UP);
	setPerspective(a_fov, a_aspectRatio, a_near, a_far);

	updateProjectionView();

	m_worldTransform = glm::inverse(m_viewTransform);
}

Camera::~Camera()
{
}

#pragma endregion

void Camera::update(float deltaTime)
{
}

// Set functions to set different transforms for the camera
#pragma region Setters
void Camera::setPerspective(float a_fov, float a_aspectRatio, float a_near, float a_far)
{
	m_projectionTransform = glm::perspective(a_fov, a_aspectRatio, a_near, a_far);
}

void Camera::setLookAt(const vec3 & a_FROM, const vec3 & a_TO, const vec3 & a_UP)
{
	m_viewTransform = glm::lookAt(a_FROM, a_TO, a_UP);

	m_eye = a_FROM;
	m_look = a_TO;
	m_up = a_UP;
}

void Camera::setPosiition(vec3 a_position)
{
	m_position = a_position;
}

#pragma endregion


// Get funcitons to return the values of the current camera
#pragma region Getters
mat4 Camera::getWorldTransform()
{
	return m_worldTransform;
}

mat4 Camera::getView()
{
	return m_viewTransform;
}

mat4 Camera::getProjection()
{
	return m_projectionTransform;
}

mat4 Camera::getProjectionView()
{
	return m_projectionViewTransform;
}

vec3 Camera::getPosition()
{
	return m_position;
}

#pragma endregion

// Update the camera's current projection view
//		This is projection * view 
void Camera::updateProjectionView()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}


