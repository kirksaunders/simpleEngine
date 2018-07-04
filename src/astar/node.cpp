#include "astar/node.hpp"

using namespace Math3D;

Node::Node() {
	parent = NULL;
	h = 0.0;
	g = 0.0;
}

Node::Node(const Vector4 pos) {
	parent = NULL;
	position = pos;
	h = 0.0;
	g = 0.0;
}

Node::Node(const Vector4 pos, Node* par, const Node* end) {
	parent = par;
	position = pos;
	h = end->getDistanceTo(position);
	g = par->getPathDistance(position);
}

void Node::setPosition(const Vector4 pos) {
	position = pos;
}

Vector4 Node::getPosition() const {
	return position;
}

double Node::getDistanceTo(const Vector4 pos) const {
	return (position - pos).magnitude();
}

double Node::getPathDistance(const Vector4 pos) const {
	return g + (position - pos).magnitude();
}

void Node::updateH(const Node* end) {
	h = end->getDistanceTo(position);
}

void Node::updateParent(Node* par) {
	if (parent == NULL) {
		parent = par;
		g = par->getPathDistance(position);
	} else {
		double newG = par->getPathDistance(position);
		if (newG < g) {
			parent = par;
			g = newG;
		}
	}
}

Node* Node::getParent() const {
	return parent;
}

double Node::getCost() const {
	return (h + g);
}

bool Node::operator<(const Node& other) const {
	return (g + h) < (other.g + other.h);
}
