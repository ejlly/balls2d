#ifndef MY_BALLS_LIST_HPP
#define MY_BALLS_LIST_HPP

#include "balls.hpp"

class BallList{
	public:

		Ball list[NB_BALL];
		LooseTree tree;
		int nbBall;

		BallList();

		Ball* get_list();

		void addBall(int k);

		void updateCollisions(float dt, int nbSubs);

		void static threaded_collision(std::vector<Ball*>& balls);

		void subUpdate(float sub_dt);

		void update(float dt);
};

#endif
