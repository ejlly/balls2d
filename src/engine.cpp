// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.hpp"

Engine::Engine(int _nb_points_circle) {
	nb_points_circle = _nb_points_circle;

	lastTime = glfwGetTime();
	addTime = 9e-2;
	lastAddTime = lastTime;
}

GLfloat Engine::getAngle(){
	return 2 * (glm::pi<GLfloat>()) / nb_points_circle;
}


std::vector<GLfloat> Engine::genVertices() {
	//Gen circle coordinates
	GLfloat theta = getAngle();
	std::vector<GLfloat> vertices(3*(nb_points_circle+1), 0);

	for(int i(0); i<nb_points_circle; i++){
		vertices[3*(i+1)]   = glm::cos(i*theta);
		vertices[3*(i+1)+1] = glm::sin(i*theta);
		vertices[3*(i+1)+2] = 0.f;
	}

	return vertices;
}

std::vector<GLuint> Engine::genIndices() {
	std::vector<GLuint> indices(3*nb_points_circle, 0);

	for(int i(0); i<nb_points_circle; i++){
		indices[3*i+1] = i+1;
		indices[3*i+2] = (i == nb_points_circle - 1)?1:(i+2);
	}

	return indices;
}

int Engine::getNbPointsCircle() const {
	return nb_points_circle;
}

int Engine::update() {
	GLfloat timeValue = glfwGetTime();
	//GLfloat dt = timeValue - lastTime;
	//lastTime = timeValue;
	int retValue = 0;


	if(addTime > 3){
		retValue = 10;
		addTime = 0;
	}
	addTime++;
	return retValue;
}
