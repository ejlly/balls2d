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

	GLfloat const timeBeforeUpdate = glfwGetTime();
	GLfloat const timeUpdate = glfwGetTime() - timeBeforeUpdate;

	std::cout << "time for update : " << timeUpdate << std::endl;


	GLfloat const timeBeforePlot = glfwGetTime();
	//for(int i(0); i < maListe.nbBall; i++){
		//Ball& ball = maListe.list[i];
		////shaderProgram.uniform_4x4("model", 1, GL_FALSE, glm::value_ptr(ball.get_model()));
		////shaderProgram.uniform_3f("color", 1, glm::value_ptr(ball.color));
		////std::cout << glm::length2(ball.speed) << " " << ball.pos.x << " " << ball.pos.y;
		////Draw the structure
	//}
	GLfloat const timePlot = glfwGetTime() - timeBeforePlot;
	std::cout << "time for plot : " << timePlot << std::endl;

	//float const timeframe = .16666f;
	//GLfloat timeEndFrame = glfwGetTime();
	//float const timeframe = .1f;

	//float timeTaken = timeEndFrame - timeValue;
	
	//std::cout << "\t" << timeTaken << std::endl;


	//if(timeframe - timeTaken > 0) sleep(timeframe - timeTaken);

	return retValue;
}
