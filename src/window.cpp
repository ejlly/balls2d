#include "window.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

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

	nbBall = 2000;

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

	//cam = Camera(window);

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
	/*
	glGenBuffers(1, &instanceModelVBO);
	glGenBuffers(1, &instanceColorVBO);
	*/

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

	/*
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
	*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Window::updateUniforms(const glm::mat4& instanceModel, const glm::vec3& instanceColor) {
	shaderProgram.uniform_4x4("instanceModel", 1, GL_FALSE, glm::value_ptr(instanceModel));
	shaderProgram.uniform_3f("instanceColor", 1, glm::value_ptr(instanceColor));
}

void Window::render() {
	// Clear color buffer
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();

	int const n = engine.getNbPointsCircle();

	glBindVertexArray(VAO);
	for(int i(0); i<maListe.getNbBalls(); i++){
		Ball& ball = maListe.getBall(i);
		//std::cout << ball.pos.x << " " << ball.pos.y << std::endl;
		updateUniforms(ball.get_model(), ball.color);
		glDrawElements(GL_TRIANGLES, static_cast<GLuint>(3*(n+1)), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	glDepthFunc(GL_LEQUAL); 

	// Swap the screen buffers
	glfwSwapBuffers(window);
}

void Window::freeInstanceBuffers() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Window::runLoop() {
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
		
		int add_number = engine.update();
 		if (maListe.nbBall < nbBall)
			maListe.addBall(add_number);

		GLfloat const timeBeforeUpdate = glfwGetTime();
		maListe.update(2e-2f);
		GLfloat const timeUpdate = glfwGetTime() - timeBeforeUpdate;
		std::cout << "time for update : " << timeUpdate << std::endl;


		GLfloat const timeBeforePlot = glfwGetTime();
		render();
		GLfloat const timePlot = glfwGetTime() - timeBeforePlot;
		std::cout << "time for plot : " << timePlot << std::endl;
    }
}