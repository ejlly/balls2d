#ifndef MY_CONSTANTS_BALLS_HPP
#define MY_CONSTANTS_BALLS_HPP

// GFLW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>

#define NB_BALL 2000
#define CENTER_GRAV 0
#define GLOBAL_SIZE (.02f)

glm::vec3 const G = glm::vec3(0.f, -1.f, 0.f);
GLfloat const g = 1.f;
GLfloat const EPS = 1e-7f;
GLfloat const rebounce_factor = .9f;


#endif
