#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

extern int CENTER_GRAV;
extern float GLOBAL_SIZE;
extern glm::vec3 G;
extern GLfloat g;
extern GLfloat EPS;
extern GLfloat rebounce_factor;

void loadConstants(const std::string& iniFilePath);