#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera{
	private:
		GLFWwindow* window;
		glm::vec3 position;
		float hAngle;
		float vAngle;
		float Fov;
		float speed;
		float mouseSpeed;
		glm::mat4 view;
		glm::mat4 proj;

		double lastTime;

	public:
		Camera(GLFWwindow* _window=nullptr);

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();

		void update(int const width, int const height);

};