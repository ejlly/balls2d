#pragma once

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstring>

#include <unistd.h> //why ?

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

//Balls
#include "ball_list.hpp"
#include "keys.hpp"
#include "shader_progs.hpp"
#include "engine.hpp"

class Engine {
    private:
        int nb_points_circle;


        std::vector<glm::vec3> positions, colors;

        GLfloat getAngle();

        GLfloat lastTime, addTime, lastAddTime;
        GLint respec_tree_time;
    
    public:
        Engine(int _nb_points_circle=30);

        std::vector<GLfloat> genVertices();
        std::vector<GLuint> genIndices();
        
        std::vector<glm::vec3> getPositions();
        std::vector<glm::mat4> getModels();
        std::vector<glm::vec3> getColors();

        int getNbPointsCircle() const;

        int update();
};