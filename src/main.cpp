#include "window.hpp"
#include "constants.hpp"

int main() {
	loadConstants("constants.ini");

	int const width = 900, height = 900;

	Window window(width, height);
	window.generateInstanceBuffers();
	window.runLoop();

    return 0;
}