#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"

//Balls
#include "ball_list.hpp"
#include "keys.hpp"
#include "shader_progs.hpp"
#include "camera.hpp"

// Function prototypes
const char * GetGLErrorStr(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:          return "No error";
    case GL_INVALID_ENUM:      return "Invalid enum";
    case GL_INVALID_VALUE:     return "Invalid value";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    case GL_OUT_OF_MEMORY:     return "Out of memory";
    default:                   return "Unknown error";
    }
}

void CheckGLError(){
	std::cout << "Checking error\n";
    while(true){
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;

        std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;
    }
}

GLfloat mod_float(GLfloat x, GLfloat mod){
	int k = (int) (x/mod);
	return x-k*mod;
}

int main(){

    glfwInit();
	
	int const WIDTH = 900, HEIGHT = 900;
	Window win(WIDTH, HEIGHT);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow *window(win.getaddr());

    window = glfwCreateWindow(WIDTH, HEIGHT, "Balls", nullptr, nullptr);
	win.setWindow(window);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, WIDTH, HEIGHT);

	Camera cam(win);

	DrawingProgram shaderProgram("src/shaders/boidShader.vs", "src/shaders/boidShader.fs");
    // Set up vertex data (and buffer(s)) and attribute pointers

	int const NB_POINTS_CIRCLE = 30;

    GLfloat vertices[3*(NB_POINTS_CIRCLE+1)] = {0.f, 0.f, 0.f};
	//Gen circle coordinates
	
	GLfloat theta = 2*glm::pi<GLfloat>()/NB_POINTS_CIRCLE;
	GLuint indices[3*NB_POINTS_CIRCLE];

	GLuint edge_indices[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		0, 4,
		1, 4,
		2, 4,
		3, 4
	};

	for(int i(0); i<NB_POINTS_CIRCLE; i++){
		vertices[3*(i+1)]     = glm::cos(i*theta);
		vertices[3*(i+1) + 1] = glm::sin(i*theta);
		vertices[3*(i+1) + 2] = 0.f;
		
		indices[3*i]     = 0;
		indices[3*i + 1] = i+1;
		indices[3*i + 2] = (i == NB_POINTS_CIRCLE - 1)?1:(i+2);
	}
	
	int const NB_EBOS = 1;

    GLuint EBOs[NB_EBOS], VBOs[NB_EBOS], VAOs[NB_EBOS];
    glGenVertexArrays(NB_EBOS, VAOs);
    glGenBuffers(NB_EBOS, VBOs);
	glGenBuffers(NB_EBOS, EBOs);


	//Polyedra
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO

    glBindVertexArray(0); // Unbind VAO



	BallList maListe;
	
	GLfloat lastTime = glfwGetTime();
	GLfloat addTime = 9e-2;
	GLfloat lastAddTime = lastTime;


	bool pause=false;

    // Game loop
	int add_time = 0;
    while(!glfwWindowShouldClose(win.getaddr())){
        glfwPollEvents();
		GLfloat timeValue = glfwGetTime();
		//GLfloat dt = timeValue - lastTime;
		//lastTime = timeValue;

        // Clear the colorbuffer



		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		shaderProgram.use();

		if(add_time > 3){
			maListe.addBall(10);
			add_time = 0;
		}
		add_time++;
		

		GLfloat const timeBeforeUpdate = glfwGetTime();
		maListe.update(2e-2f);
		GLfloat const timeUpdate = glfwGetTime() - timeBeforeUpdate;

		std::cout << "time for update : " << timeUpdate << std::endl;


		GLfloat const timeBeforePlot = glfwGetTime();
		for(int i(0); i < maListe.nbBall; i++){
			Ball& ball = maListe.list[i];
			shaderProgram.uniform_4x4("model", 1, GL_FALSE, glm::value_ptr(ball.get_model()));
			shaderProgram.uniform_3f("color", 1, glm::value_ptr(ball.color));
			//std::cout << glm::length2(ball.speed) << " " << ball.pos.x << " " << ball.pos.y;
			//Draw the structure
			glBindVertexArray(VAOs[0]);
			glDrawElements(GL_TRIANGLES, 3*(NB_POINTS_CIRCLE+1), GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);

			glDepthFunc(GL_LEQUAL); 
		}
		GLfloat const timePlot = glfwGetTime() - timeBeforePlot;
		std::cout << "time for plot : " << timePlot << std::endl;

		//float const timeframe = .16666f;
		//GLfloat timeEndFrame = glfwGetTime();
		//float const timeframe = .1f;

		//float timeTaken = timeEndFrame - timeValue;
		
		//std::cout << "\t" << timeTaken << std::endl;


		//if(timeframe - timeTaken > 0) sleep(timeframe - timeTaken);

		// Swap the screen buffers
		glfwSwapBuffers(win.getaddr());
    }
    glDeleteVertexArrays(NB_EBOS, VAOs);
    glDeleteBuffers(NB_EBOS, VBOs);
	glDeleteBuffers(NB_EBOS, EBOs);

    glfwTerminate();
    return 0;
}
