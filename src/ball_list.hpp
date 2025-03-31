#pragma once

#include "balls.hpp"
#include "coll_tree.hpp"
#include <vector>

class BallList{
	public:

		std::vector<Ball> list;
		int nbBall;

		LooseTree tree;

		BallList();

		void addBall(int k);

		void updateCollisions(float dt, int nbSubs);

		void static threaded_collision(std::vector<Ball*>& balls);

		void subUpdate(float sub_dt);

		void update(float dt);
};