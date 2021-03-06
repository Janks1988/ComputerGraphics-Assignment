#include "CompGraphicsAssignmentApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <imgui.h>
#include <string.h>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;
using std::string;

CompGraphicsAssignmentApp::CompGraphicsAssignmentApp() {

}

CompGraphicsAssignmentApp::~CompGraphicsAssignmentApp() {

}

bool CompGraphicsAssignmentApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	
	// Create new camer
	m_camera = new FlyCamera();

	return true;
}

void CompGraphicsAssignmentApp::shutdown() {

	delete m_camera;
	Gizmos::destroy();
}

void CompGraphicsAssignmentApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	//Add Cameras update function to allow the Camera to move around
	m_camera->update((float)glfwGetTime());

	// Create ImageGui
	ImGui::Begin("Camera");
	ImGui::Text("Camera Position: ", 11);
	ImGui::End();
	// End ImgGUI

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void CompGraphicsAssignmentApp::draw() {

	// wipe the screen to the background colour
	clearScreen();
	ImGui::Render();
	Gizmos::draw(m_camera->getProjectionView());
}