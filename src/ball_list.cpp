#include "ball_list.hpp"

#include <math.h>
#include <thread>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>

BallList::BallList(){
	nbBall = 1;
	list[0] = Ball(glm::vec3(-.7f, .0f, 0.f), glm::vec3(0.f, .3f, 0.f));
	tree.add(&(list[0]));
	//list[0] = Ball(glm::vec3(-.9f, .8f, 0.f), glm::vec3(0.f, 0.f, 0.f));
}

void BallList::addBall(int k){
	if(nbBall<NB_BALL-k){
		float const h = fmod(nbBall, 360L);
		float const s = .8;
		float const l = .7;

		float const c = (1 - abs(2*l - 1)) * s;
		float const x = c*(1 - abs(fmod(h/60.f, 2L) - 1));
		float const m = l-c/2;

		glm::vec3 color;


		if(h <= 60)        color = glm::vec3(c+m,  x+m,  m  );
		else if (h <= 120) color = glm::vec3(x+m,  c+m,  m  );
		else if (h <= 180) color = glm::vec3(m  ,  c+m,  x+m);
		else if (h <= 240) color = glm::vec3(m  ,  x+m,  c+m);
		else if (h <= 300) color = glm::vec3(x+m,  m  ,  c+m);
		else               color = glm::vec3(c+m,  m  ,  x+m);

		//color = glm::vec3(1.f);

#if CENTER_GRAV
		list[nbBall] = Ball(glm::vec3(-.6f, .0f, 0.f), glm::vec3(0.f, .02f, 0.f), color);
		tree.add(&(list[nbBall]));
		nbBall++;
#else
		for(int i(0); i<k; i++){
			//std::cout << nbBall << std::endl;
			list[nbBall] = Ball(glm::vec3(-.6f, i*.1f, 0.f), glm::vec3(2.f, .5f, 0.f), color);
			tree.add(&(list[nbBall]));
			//tree.update();
#if DEBUG
			tree.print();
			std::cout << "*****************************************\n";
			std::cout << "-----------------------------------------\n";
#endif
			nbBall++;
		}
#endif
		//TODO : add update tree
	}
}

Ball* BallList::get_list(){
	return list;
}

void BallList::updateCollisions(float dt, int nbSubs){
	float const sub_dt = dt/nbSubs;
	for(int i(0); i<nbSubs; i++){
		this->subUpdate(sub_dt);
	}
	tree.update();
}

void BallList::threaded_collision(std::vector<Ball*>& balls){
	
	int size_balls = balls.size();

	for(int i(0); i<size_balls; i++){
		for(int j(i+1); j<size_balls; j++){
			Ball& ball1 = *(balls[i]);
			Ball& ball2 = *(balls[j]);

			glm::vec3 const centerVector = ball1.pos - ball2.pos;
			GLfloat const sumRadius = ball1.radius + ball2.radius;
			GLfloat const dist2 = glm::length2(centerVector);



			if(dist2 < sumRadius*sumRadius){
				/*
				tmp_displacement[i] = ball1.pos + centerVector*sumRadius/(2.f*dist);
				tmp_displacement[j] = ball2.pos - centerVector*sumRadius/(2.f*dist);
				*/
				GLfloat const dist = sqrt(dist2);
				ball1.pos = ball1.pos + .75f*centerVector*(sumRadius - dist)/(2.f*dist);
				ball2.pos = ball2.pos - .75f*centerVector*(sumRadius - dist)/(2.f*dist);
			}
		}
	}
}

int find_index(float x){
	return 0;
}

void BallList::subUpdate(float sub_dt){
	for(int i(0); i<nbBall; i++){
		Ball& ball = list[i];
		ball.integrate(sub_dt);
		ball.checkBounds();
	}

/*
	int size_balls = NB_BALL;

	for(int i(0); i<size_balls; i++){
		for(int j(i+1); j<size_balls; j++){
			Ball& ball1 = list[i];
			Ball& ball2 = list[j];

			glm::vec3 const centerVector = ball1.pos - ball2.pos;
			GLfloat const sumRadius = ball1.radius + ball2.radius;
			GLfloat const dist2 = glm::length2(centerVector);



			if(dist2 < sumRadius*sumRadius){
				GLfloat const dist = sqrt(dist2);
				ball1.pos = ball1.pos + .75f*centerVector*(sumRadius - dist)/(2.f*dist);
				ball2.pos = ball2.pos - .75f*centerVector*(sumRadius - dist)/(2.f*dist);
			}
		}
	}
*/

	tree.computePairs();
	tree.update();

}

void BallList::update(float dt){
	this->updateCollisions(dt, 8);
}
