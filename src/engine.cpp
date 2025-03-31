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


GLfloat Engine::getAngle(){
	return 2 * (glm::pi<GLfloat>()) / nb_points_circle;
}

Engine::Engine(int _nb_points_circle) {
	nb_points_circle = _nb_points_circle;

	lastTime = glfwGetTime();
	addTime = 9e-2;
	nbBall = 2500;
	lastAddTime = lastTime;
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

std::vector<glm::vec3> Engine::getPositions() {
	positions.resize(maListe.nbBall, glm::vec3(0));
	for(int i(0); i < maListe.nbBall; i++){
		Ball& ball = maListe.list[i];
		positions[i] = ball.pos;
	}
	return positions;
}

std::vector<glm::mat4> Engine::getModels() {
	std::vector<glm::mat4> models(maListe.nbBall, glm::mat4(1));
	for(int i(0); i < maListe.nbBall; i++){
		Ball& ball = maListe.list[i];
		models[i] = ball.get_model();
	}
	return models;
}

std::vector<glm::vec3> Engine::getColors() {
	colors.resize(maListe.nbBall, glm::vec3(0));
	for(int i(0); i < maListe.nbBall; i++){
		Ball& ball = maListe.list[i];
		colors[i] = ball.color;
	}
	return colors;
}

int Engine::getNbPointsCircle() const {
	return nb_points_circle;
}

int Engine::getNbBalls() const {
	return maListe.nbBall;
}

void Engine::addBall(int _nbBall) {
	maListe.addBall(_nbBall);
}

void Engine::update() {
	GLfloat timeValue = glfwGetTime();
	//GLfloat dt = timeValue - lastTime;
	//lastTime = timeValue;


	if(addTime > 3 && maListe.nbBall < nbBall){
		maListe.addBall(10);
		addTime = 0;
	}
	addTime++;

	std::cout << "nb ball : " << maListe.nbBall << std::endl;

	if(respec_tree_time > 100){
		maListe.tree.update();
		respec_tree_time = 0;
	}
	respec_tree_time++;	

	GLfloat const timeBeforeUpdate = glfwGetTime();
	//maListe.update(2e-2f);
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
}
