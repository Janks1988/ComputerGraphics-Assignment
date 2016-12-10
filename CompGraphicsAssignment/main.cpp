#include "CompGraphicsAssignmentApp.h"

int main() {
	
	auto app = new CompGraphicsAssignmentApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}