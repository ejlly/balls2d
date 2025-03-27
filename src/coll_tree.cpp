#include "coll_tree.hpp"
#include <iostream>
#define DEBUG 1


Node::Node(){
	pere = nullptr;
	fils[0] = nullptr;
	fils[1] = nullptr;
	ball = nullptr;
}

bool Node::isLeaf() const{
	return !static_cast<bool>(fils[0]);
}

void Node::setChildren(Node& n0, Node& n1){
	n0.pere = this;
	n1.pere = this;

	fils[0] = &n0;
	fils[1] = &n1;
}

void Node::setValue(Ball *b){
	ball = b;
}

void Node::updateAABB(float eps){
	if(isLeaf()){
		box.m_top    += eps;
		box.m_right  += eps;
		box.m_bottom -= eps;
		box.m_left   -= eps;
	}
	else
		box = (fils[0]->box).unionize(fils[1]->box);
}

Node* Node::getBrother() const{
	if(this == pere->fils[0]) return pere->fils[1];
	else return pere->fils[0];
}

Pair_Ball::Pair_Ball(){
	fst = nullptr; snd = nullptr;
}

Pair_Ball::Pair_Ball(Ball* a, Ball* b){
	fst = a; snd = b;
}

Pair_Ball::Pair_Ball(const Pair_Ball& p){
	fst = p.fst; snd = p.snd;
}

LooseTree::LooseTree(){
	m_root = nullptr;
	m_eps = GLOBAL_SIZE;
}

LooseTree::~LooseTree(){
	delete m_root;
}

bool LooseTree::isLeaf(const Node& node) const{
	return isLeaf(node);
}

void LooseTree::updateInvalids(Node* node){
#if DEBUG
	std::cout << "ib\n";
	std::cout << "addresse pere : " << node->pere << std::endl;
#endif
	if(node->isLeaf()){
#if DEBUG
		std::cout << "a\n" << "pere bf : " << node->ball->box.m_bottom << std::endl;
#endif
		if(!(node->box.contains(node->ball->box))){
			m_invalidNodes.push_back(node);
#if DEBUG
			std::cout << "b\n" << "pere af : " << node->ball->box.m_bottom << std::endl;
#endif
		}
	}
	else{
#if DEBUG
		std::cout << "du\n";
#endif
		updateInvalids(node->fils[0]);
		updateInvalids(node->fils[1]);
	}
}

void LooseTree::update(){
	if(m_root){
		if(m_root->isLeaf()) m_root->updateAABB(m_eps);
		else{
			m_invalidNodes.clear();
			updateInvalids(m_root);
#if DEBUG
			std::cout << "aa\n";
#endif

			for(Node *node: m_invalidNodes){

				Node *parent = node->pere;
				Node *frere = node->getBrother();
				Node **parentLink = parent->pere ? (parent == parent->pere->fils[0] 
							? &parent->pere->fils[0] 
							: &parent->pere->fils[1])
					: &m_root;

				// replace parent with sibling
				frere->pere = parent->pere ? parent->pere : nullptr; // root has null pere

				*parentLink = frere;
				delete parent;

				// re-insert node
				node->updateAABB(m_eps);
				insertNode(node, &m_root);
			}
			m_invalidNodes.clear();
		}
	}
}

void LooseTree::add(Ball *b){
	if(m_root){
		Node *node = new Node();
		node->setValue(b);
		node->updateAABB(m_eps);
		insertNode(node, &m_root);
	}
	else{
		m_root = new Node();
		m_root->setValue(b);
		m_root->updateAABB(m_eps);
	}
}

void LooseTree::insertNode(Node *node, Node **parent){
	Node *p = *parent;
	if(p->isLeaf()){
		Node *newParent = new Node();
		newParent->pere = p->pere;
		newParent->setChildren(*node, *p);
		*parent = newParent;
	}
	else{
		// parent is branch, compute volume differences 
		// between pre-insert and post-insert
		AABB *aabb0 = &(p->fils[0]->box);
		AABB *aabb1 = &(p->fils[1]->box);
		const float volumeDiff0 = aabb0->unionize(node->box).volume() - aabb0->volume();
		const float volumeDiff1 = aabb1->unionize(node->box).volume() - aabb1->volume();

		// insert to the child that gives less volume increase
		if(volumeDiff0 < volumeDiff1) insertNode(node, &p->fils[0]);
		else insertNode(node, &p->fils[1]);
	}
	// update parent AABB
	// (propagates back up the recursion stack)
	(*parent)->updateAABB(m_eps);
}

void LooseTree::remove(Ball &b){
	Node *node = b.respective_node;

	// remove two-way link
	node->ball = nullptr;
	b.respective_node = nullptr;

	removeNode(node);
}

void LooseTree::removeNode(Node *node){
	// replace parent with sibling, remove parent node
	Node *parent = node->pere;
	if(parent){
		// node is not root
		Node *sibling = node->getBrother();
		if(parent->pere){
			// if there's a grandparent
			// update links
			sibling->pere = parent->pere;
			(parent == parent->pere->fils[0] 
			 ? parent->pere->fils[0] 
			 : parent->pere->fils[1]) = sibling;
			//parent = sibling;
		}
		else{
			// no grandparent
			// make sibling root
			Node *sibling = node->getBrother();
			m_root = sibling;
			sibling->pere = nullptr;
		}
		delete node;
		delete parent;
	}
	else{
		// node is root
		m_root = nullptr;
		delete node;
	}
}

void LooseTree::computePairs(){
	std::cout << "Computing pairs\n";
	m_collisions.clear();
	if(!m_root || m_root->isLeaf())
		return;

	// clear Node::childrenCrossed flags
	clearChildrenCrossFlagHelper(m_root);

	// base recursive call
	computePairsHelper(m_root->fils[0], m_root->fils[1]);
	std::cout << "pairs computed\n";
}

void LooseTree::clearChildrenCrossFlagHelper(Node *node){
	node->childrenCrossed = false;
	if(!node->isLeaf()){
		clearChildrenCrossFlagHelper(node->fils[0]);
		clearChildrenCrossFlagHelper(node->fils[1]);
	}
}

void LooseTree::crossChildren(Node *node){
	if(!node->childrenCrossed){
		computePairsHelper(node->fils[0], node->fils[1]);
		node->childrenCrossed = true;
	}
}

void LooseTree::computePairsHelper(Node *n0, Node *n1){
	if(n0->isLeaf()){
		// 2 leaves, check proxies instead of fat AABBs
		if(n1->isLeaf()){
			/*
			if(n0->ball->collides(*n1->ball))
				m_collisions.push_back(Pair_Ball(n0->ball, n1->ball));
			*/
			n0->ball->coll_update(*(n1->ball));
		}
		// 1 branch / 1 leaf, 2 cross checks
		else{
			crossChildren(n1);
			computePairsHelper(n0, n1->fils[0]);
			computePairsHelper(n0, n1->fils[1]);
		}
	}
	else{
		// 1 branch / 1 leaf, 2 cross checks
		if(n1->isLeaf()){
			crossChildren(n0);
			computePairsHelper(n0->fils[0], n1);
			computePairsHelper(n0->fils[1], n1);
		}
		// 2 branches, 4 cross checks
		else{
			crossChildren(n0);
			crossChildren(n1);
			computePairsHelper(n0->fils[0], n1->fils[0]);
			computePairsHelper(n0->fils[0], n1->fils[1]);
			computePairsHelper(n0->fils[1], n1->fils[0]);
			computePairsHelper(n0->fils[1], n1->fils[1]);
		}
	} // end of if (n0->IsLeaf())
}


void LooseTree::print(){
	std::cout << "tree print :\n";
	print_aux(m_root, 0);
	std::cout << "tree done\n";
}

#define SPACE_COUNT 15

void LooseTree::print_aux(Node* node, int cur_size){
	if(!node) return;

	cur_size += SPACE_COUNT;

	print_aux(node->fils[1], cur_size);
	std::cout << std::endl;

	for(int i(SPACE_COUNT); i<cur_size; i++)
		std::cout << " ";

	std::cout << node << std::endl;
	
	print_aux(node->fils[0], cur_size);
}
