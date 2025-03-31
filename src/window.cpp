#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include "window.hpp"

const char * GetGLErrorStr(GLenum err) {
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

void CheckGLError() {
	std::cout << "Checking error\n";
    while(true){
        const GLenum err = glGetError();
        if (GL_NO_ERROR == err)
            break;

        std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;
    }
}

Window::Window(int _width, int _height) {
	width = _width;
	height = _height;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(width, height, "Balls", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glewExperimental = GL_TRUE;
    glewInit();
    glViewport(0, 0, width, height);

	cam = Camera(window);

	shaderProgram.init("src/shaders/boidShader.vs", "src/shaders/boidShader.fs");
}

Window::Window() {
	Window(1500, 900);
}

Window::~Window() {
	freeInstanceBuffers();
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* Window::getAddr() {
	return window;
}

void Window::setWindow(GLFWwindow* _window) {
	window = _window;
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

void Window::generateInstanceBuffers() {
	std::vector<GLfloat> vertices = engine.genVertices();
	std::vector<GLuint> indices = engine.genIndices();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &instanceModelVBO);
	glGenBuffers(1, &instanceColorVBO);

	//TODO: if different radii for each ball, add a radius buffer

	glBindVertexArray(VAO);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);

	//Index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    //Instance position buffer
    glBindBuffer(GL_ARRAY_BUFFER, instanceModelVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    //Instance color buffer
    glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Window::updateInstanceBuffer(const std::vector<glm::mat4>& instanceModels, const std::vector<glm::vec3>& instanceColors) {
    glBindBuffer(GL_ARRAY_BUFFER, instanceModelVBO);
    static size_t instanceModelBufferSize = 0;
    size_t newModelBufferSize = instanceModels.size() * sizeof(glm::mat4);
    //if (newModelBufferSize != instanceModelBufferSize) {
	if (true) {
        glBufferData(GL_ARRAY_BUFFER, newModelBufferSize, instanceModels.data(), GL_DYNAMIC_DRAW);
        instanceModelBufferSize = newModelBufferSize;
    }
	else
        glBufferSubData(GL_ARRAY_BUFFER, 0, newModelBufferSize, instanceModels.data());

    glBindBuffer(GL_ARRAY_BUFFER, instanceColorVBO);
    static size_t instanceColorBufferSize = 0;
    size_t newColorBufferSize = instanceColors.size() * sizeof(glm::vec3);
    //if (newColorBufferSize != instanceColorBufferSize) {
	if (true) {
        glBufferData(GL_ARRAY_BUFFER, newColorBufferSize, instanceColors.data(), GL_DYNAMIC_DRAW);
        instanceColorBufferSize = newColorBufferSize;
    }
	else
        glBufferSubData(GL_ARRAY_BUFFER, 0, newColorBufferSize, instanceColors.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Window::render() {
	// Clear color buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();

	int const n = engine.getNbPointsCircle(),
			  m = engine.getNbBalls();

	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLuint>(3*(n+1)), GL_UNSIGNED_INT, 0, m);
	glBindVertexArray(0);

	glDepthFunc(GL_LEQUAL); 

	// Swap the screen buffers
	glfwSwapBuffers(window);
}

void Window::freeInstanceBuffers() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &instanceModelVBO);
	glDeleteBuffers(1, &instanceColorVBO);
	glDeleteVertexArrays(1, &VAO);
}

void Window::runLoop() {

	//engine.addBall(1);

	std::vector<glm::mat4> models;
	glm::mat4 model(1.0f);
	
	glm::mat4 resize(1.0f);

	resize[0][0] = .4f;
	resize[1][1] = .4f;

	model = glm::translate(model, glm::vec3(0.f));

	model = model * resize;
	models.push_back(model);

	std::vector<glm::vec3> colors;
	colors.push_back(glm::vec3(1.0f, .0f, .5f));

	int const n = engine.getNbPointsCircle();
	engine.addBall(1);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
		//engine.update();

		//updateInstanceBuffer(engine.getModels(), engine.getColors());
		//updateInstanceBuffer(models, colors);
		//render();


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();
		for(int i(0); i < engine.maListe.nbBall; i++){
			Ball& ball = engine.maListe.list[i];
			shaderProgram.uniform_4x4("model", 1, GL_FALSE, glm::value_ptr(ball.get_model()));
			shaderProgram.uniform_3f("color", 1, glm::value_ptr(ball.color));
			std::cout << glm::length2(ball.speed) << " " << ball.pos.x << " " << ball.pos.y;
			//Draw the structure
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 3*(n+1), GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);

			glDepthFunc(GL_LEQUAL); 
			glfwSwapBuffers(window);
		}
		std::cout << std::endl;
    }
}