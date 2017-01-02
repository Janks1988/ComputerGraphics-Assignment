#include "FlyCamera.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Input.h"
#include <glm/ext.hpp>



FlyCamera::FlyCamera()
{
	m_speed = 0.1f;
}


FlyCamera::~FlyCamera()
{
}

void FlyCamera::update(float deltaTime)
{
	// Get delta time using get time function
	float time = (float)glfwGetTime();

	// Sets window to the current window
	GLFWwindow* window = glfwGetCurrentContext();

	glm::mat4 m_rotMat; // Temp Matrix used for rotation
	glm::vec3 cameraRight = glm::normalize(glm::cross(m_up, m_eye)); //Get camera's x-axis for pitch rotation

	// Movement functions
	// Camera should be able to move backwards, Forwards
	// left, right, up and down

	if (glfwGetKey(window, 'W') == GLFW_PRESS)
		this->m_worldTransform[3] -= m_worldTransform[2] * m_speed;
	if (glfwGetKey(window, 'S') == GLFW_PRESS)
		this->m_worldTransform[3] += m_worldTransform[2] * m_speed;
	if (glfwGetKey(window, 'A') == GLFW_PRESS)
		this->m_worldTransform[3] -= m_worldTransform[0] * m_speed;
	if (glfwGetKey(window, 'D') == GLFW_PRESS)
		this->m_worldTransform[3] += m_worldTransform[0] * m_speed;
	if (glfwGetKey(window, 'R') == GLFW_PRESS)
		this->m_worldTransform[3] += m_worldTransform[1] * m_speed;
	if (glfwGetKey(window, 'F') == GLFW_PRESS)
		this->m_worldTransform[3] -= m_worldTransform[1] * m_speed;

	// Roll Funtion
	// Camera Roll when Q and E are pressed
	if (glfwGetKey(window, 'Q') == GLFW_PRESS)
	{
		m_rotMat = glm::axisAngleMatrix(m_eye, m_speed / 10);
		m_worldTransform[0] = m_rotMat * m_worldTransform[0];
		m_worldTransform[1] = m_rotMat * m_worldTransform[1];
		m_worldTransform[2] = m_rotMat * m_worldTransform[2];
	}
	// Negated speed to roll camera the other way
	if (glfwGetKey(window, 'E') == GLFW_PRESS)
	{
		m_rotMat = glm::axisAngleMatrix(m_eye, -(m_speed / 10));
		m_worldTransform[0] = m_rotMat * m_worldTransform[0];
		m_worldTransform[1] = m_rotMat * m_worldTransform[1];
		m_worldTransform[2] = m_rotMat * m_worldTransform[2];
	}

	// Camera Rotation
	// Rotate Camera when right mouse button is clicked

	// Check if mouse button is pressed
	static bool rightMouseButDown = false;

	// Allow rotation if second mouse button is pressed
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		// Previous mouse positions set to 0 when right mouse is pressed
		static double siPrevMouseX = 0;
		static double siPrevMouseY = 0;

		// change Button down to true and get cursor offset
		if (rightMouseButDown == false) {
			rightMouseButDown = true;
			glfwGetCursorPos(window, &siPrevMouseX, &siPrevMouseY);
		}

		// Get current cursor postion
		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Find the amount a mouse has moved by subtractinbg the offset by the current
		// Cursor position
		double iDeltaX = mouseX - siPrevMouseX;
		double iDeltaY = mouseY - siPrevMouseY;

		siPrevMouseX = mouseX;
		siPrevMouseY = mouseY;

		// pitch
		if (iDeltaY != 0) {
			m_rotMat = glm::axisAngleMatrix(cameraRight, (float)-iDeltaY / 150.0f);
			m_worldTransform[0] = m_rotMat * m_worldTransform[0];
			m_worldTransform[1] = m_rotMat * m_worldTransform[1];
			m_worldTransform[2] = m_rotMat * m_worldTransform[2];
		}

		// yaw
		if (iDeltaX != 0) {
			m_rotMat = glm::axisAngleMatrix(m_up, (float)-iDeltaX / 150.0f);
			m_worldTransform[0] = m_rotMat * m_worldTransform[0];
			m_worldTransform[1] = m_rotMat * m_worldTransform[1];
			m_worldTransform[2] = m_rotMat * m_worldTransform[2];
		}
	}
	else
	{
		rightMouseButDown = false;
	}

	// Update the projection transform
	m_eye = vec3(m_worldTransform[3]);
	m_viewTransform = glm::inverse(m_worldTransform);
	updateProjectionView();
}

// Set Camera Speed
void FlyCamera::setSpeed(float a_speed)
{
	m_speed = a_speed;
}

