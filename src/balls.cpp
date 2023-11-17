#include "balls.hpp"

#include <math.h>
#include <thread>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>

#define CENTER_GRAV 0
#define GLOBAL_SIZE .02f
#if CENTER_GRAV
#define TMP_DT (5e-3f)
#else
#define TMP_DT (5e-3f)
#endif


Ball::Ball(){
	pos = glm::vec3(0.0f);
	pos_last = glm::vec3(0.0f);
	speed = glm::vec3(0.0f);
	accel = glm::vec3(0.0f);
	radius = GLOBAL_SIZE;
	color = glm::vec3(1.0f, 0.f, 0.f);

	respective_node = nullptr;
	updateBox();
}

Ball::Ball(glm::vec3 pos, glm::vec3 speed){
	this->pos = pos;
	pos_last = pos - TMP_DT*speed;
	this->speed = speed;
	accel = glm::vec3(0.0f);
	radius = GLOBAL_SIZE;
	color = glm::vec3(1.0f, 0.f, 0.f);

	respective_node = nullptr;
	updateBox();
}

Ball::Ball(glm::vec3 pos, glm::vec3 speed, glm::vec3 color){
	this->pos = pos;
	pos_last = pos - TMP_DT*speed;
	this->speed = speed;
	accel = glm::vec3(0.0f);
	radius = GLOBAL_SIZE;
	this->color = color;

	respective_node = nullptr;
	updateBox();
}

void Ball::updateBox(){
	box = AABB(pos.y + radius, pos.x + radius, pos.y - radius, pos.x - radius);
}

glm::vec3 Ball::gravity(){
#if CENTER_GRAV
	glm::vec3 const force = - pos; 
	float const dist2 = glm::length2(force);

	//if(dist2 < 1e-3f) return glm::vec3(0.f);

	return 2.f*force/(glm::length2(force) + 1e-7f);
#else
	if((pos.x < -1.f + radius) || (pos.y < -1.f + radius) || (pos.x > 1.f - radius) || (pos.y > 1.f - radius))
		return glm::vec3(0.f);

	return G;
#endif
}

void Ball::checkBounds(){
#if !CENTER_GRAV
	if(pos.x < -1.f + radius){
		speed.x = -speed.x;
		speed = rebounce_factor * speed;
		pos.x = -1.f + radius;
	}
	if(pos.y < -1.f + radius){
		speed.y = -speed.y;
		speed = rebounce_factor * speed;
		pos.y = -1.f + radius;
	}
	if(pos.x > 1.f - radius){
		speed.x = -speed.x;
		speed = rebounce_factor * speed;
		pos.x = 1.f - radius;
	}
	if(pos.y > 1.f - radius){
		speed.y = -speed.y;
		speed = rebounce_factor * speed;
		pos.y = 1.f - radius;
	}
#endif
	//std::cout << speed.x << " " << speed.y << " -> " << glm::length2(speed) + pos.y*g << std::endl;
	
}

bool Ball::collides(Ball &b){
	glm::vec3 const centerVector = pos - b.pos;
	GLfloat const sumRadius = radius + b.radius;
	GLfloat const dist2 = glm::length2(centerVector);

	return dist2 < sumRadius*sumRadius;
}

bool Ball::coll_update(Ball &b){
	//std::cout << pos.x << std::endl;
	//std::cout << b.pos.x << std::endl;

	GLfloat const sumRadius = radius + b.radius;
	glm::vec3 const centerVector = pos - b.pos;
	GLfloat const dist2 = glm::length2(centerVector);

	if(dist2 < sumRadius*sumRadius){
		GLfloat const dist = sqrt(dist2);
		pos = pos + .75f*centerVector*(sumRadius - dist)/(2.f*dist);
		b.pos = b.pos - .75f*centerVector*(sumRadius - dist)/(2.f*dist);
		return true;
	}
	return false;
}

void Ball::integrate(float dt){
	glm::vec3 const disp = pos - pos_last;
	pos_last = pos;
	pos = pos + (disp + accel * (dt*dt*.5f))*.999f;
	glm::vec3 new_accel = this->gravity();
	//speed = speed + (accel + new_accel)*(dt*.5f);
	accel = new_accel;
	
	//speed = .98f * speed;
	updateBox();
}

glm::mat4 Ball::get_model(){

	glm::mat4 model(1.0f);
	
	glm::mat4 resize(1.0f);

	resize[0][0] = radius;
	resize[1][1] = radius;
	//resize[2][2] = radius;

	model = glm::translate(model, pos);

	model = model * resize;

	return model;
}

