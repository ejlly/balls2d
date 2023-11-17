#ifndef AABB_PERSONNEL_H
#define AABB_PERSONNEL_H

#include <glm/glm.hpp>

#define DEBUG 0

class AABB{
	public:
		float m_top;
		float m_right;
		float m_bottom;
		float m_left;

		AABB();
		AABB(float top, float right, float bottom, float left);
		AABB(glm::vec3 top_right, glm::vec3 bottom_left);

		float volume();

		bool contains(const AABB& box);
		bool intersects(const AABB& box);
		AABB unionize(AABB& box);
};

#endif
