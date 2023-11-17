#ifndef COLLISION_TREE_H
#define COLLISION_TREE_H

#include <vector>
#include "aabb.hpp"
#include "balls.hpp"

// GLM
#include <array>
#include <memory>
#include <glm/glm.hpp>


class Ball;

class Pair_Ball{
	public:

	Ball *fst, *snd;

	Pair_Ball();
	Pair_Ball(Ball *a, Ball *b);
	Pair_Ball(const Pair_Ball& p);
};

class Node{
	public:

	Node* pere;
	std::array<Node*, 2> fils;
	AABB box;
	Ball* ball;
	bool childrenCrossed;

	Node();

	bool isLeaf() const;
	void setChildren(Node& n0, Node& n1);
	void setValue(Ball *b);
	void updateAABB(float eps);
	Node* getBrother() const;
};

class LooseTree{
	public:
		Node* m_root;
		std::vector<Pair_Ball> m_collisions;
		float m_eps;
		std::vector<Node*> m_invalidNodes;

		LooseTree();
		~LooseTree();

		bool isLeaf(const Node& node) const;
		void update();

		void add(Ball *b);
		void remove(Ball &b);

		void computePairs();

		void print();

	private:
		void insertNode(Node* node, Node** parent);

		void removeNode(Node* node);

		void updateInvalids(Node* nodes);

		void clearChildrenCrossFlagHelper(Node *node);
		void crossChildren(Node *node);
		void computePairsHelper(Node *n0, Node *n1);

		void print_aux(Node *node, int cur_size);
};



#endif
