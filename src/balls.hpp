#ifndef MY_BALLS_HPP
#define MY_BALLS_HPP

#include "constants.hpp"
#include "aabb.hpp"
#include "coll_tree.hpp"

#include <vector>

// GFLW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>

class Node;

class Ball{
	public:
		glm::vec3 pos;
		glm::vec3 pos_last;
		glm::vec3 speed;
		glm::vec3 accel;
		GLfloat radius;
		glm::vec3 color;

		Node* respective_node;
		AABB box;

		Ball();
		Ball(glm::vec3 pos, glm::vec3 speed);
		Ball(glm::vec3 pos, glm::vec3 speed, glm::vec3 color);

		void updateBox();
		
		glm::vec3 gravity();
		void checkBounds();

		bool collides(Ball &b);
		bool coll_update(Ball &b);

		void integrate(float dt);

		glm::mat4 get_model();

};

#endif
