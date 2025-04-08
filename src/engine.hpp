#pragma once

#include <iostream>
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Balls
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