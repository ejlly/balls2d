#include "aabb.hpp"
#include <iostream>

float max(float a, float b){
	return (a>b)?a:b;
}

float min(float a, float b){
	return (a<b)?a:b;
}

AABB::AABB(){
	m_top    = .0f;
	m_right  = .0f;
	m_bottom = .0f;
	m_left   = .0f;
}

AABB::AABB(float top, float right, float bottom, float left){
	m_top    = top;
	m_right  = right;
	m_bottom = bottom;
	m_left   = left;
}

AABB::AABB(glm::vec3 top_right, glm::vec3 bottom_left){
	m_top    = top_right.x;
	m_right  = top_right.y;
	m_bottom = bottom_left.x;
	m_left   = bottom_left.y;
}

float AABB::volume(){
	return (m_top - m_bottom)*(m_right - m_left);
}

bool AABB::contains(const AABB& box){
	return m_left <= box.m_left && box.m_right <= m_right &&
		m_top <= box.m_top && box.m_bottom <= m_bottom;
}

bool AABB::intersects(const AABB& box){
	return !(m_left >= box.m_right || m_right <= box.m_left || m_top >= box.m_bottom || m_bottom <= box.m_top);
}

AABB AABB::unionize(AABB& box){
	//std::cout << "turbofeur\n";
	float _top    = max(m_top   , box.m_top);
	float _right  = max(m_right , box.m_right);
	float _bottom = min(m_bottom, box.m_bottom);
	float _left   = min(m_left  , box.m_left);

	return AABB(_top, _right, _bottom, _left);
}
