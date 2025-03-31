#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "engine.hpp"
#include "camera.hpp"
#include "shader_progs.hpp"

class Window {
	private:
		GLFWwindow *window;
		int width, height;

        GLuint VAO, VBO, EBO, instanceModelVBO, instanceColorVBO;

		Camera cam;

        DrawingProgram shaderProgram;

        Engine engine;
	
	public:
		Window(int _width, int _height);
		Window();

		~Window();

		GLFWwindow* getAddr();
		void setWindow(GLFWwindow* _window);
		int getWidth();
		int getHeight();

        void generateInstanceBuffers();

        void updateInstanceBuffer(const std::vector<glm::mat4>& instanceModels, const std::vector<glm::vec3>& instanceColors);

        void freeInstanceBuffers();
        
        void render();
        
        void runLoop();
};