#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "ball_list.hpp"
#include "engine.hpp"
//#include "camera.hpp"
#include "shader_progs.hpp"
#include "keyboard.hpp"

class Window {
	private:
		GLFWwindow *window;
		int width, height;

        GLuint VAO, VBO, EBO;

        DrawingProgram shaderProgram;

		GLuint nbBall;      

        Engine engine;

        BallList maListe;
	
	public:
		Window(int _width, int _height);
		Window();

		~Window();

		GLFWwindow* getAddr();
		void setWindow(GLFWwindow* _window);
		int getWidth();
		int getHeight();

        void generateInstanceBuffers();

        void updateUniforms(const glm::mat4& instanceModel, const glm::vec3& instanceColor);

        void freeInstanceBuffers();
        
        void render();
        
        void runLoop();
};