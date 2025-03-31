#include "shader_progs.hpp"
#include "window.hpp"

int main() {
	int const width = 900, height = 900;

	Window win(width, height);

    win.generateInstanceBuffers();

    win.runLoop();
    
    return 0;
}