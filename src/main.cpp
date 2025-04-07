#include "shader_progs.hpp"
#include "window.hpp"

int main() {
	int const width = 900, height = 900;

	Window window(width, height);
	window.generateInstanceBuffers();
	window.runLoop();

    return 0;
}