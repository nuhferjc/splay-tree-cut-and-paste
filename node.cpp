#include "node.h"

Node::Node(string k) {
	key = k;
	left = nullptr;
    right = nullptr;
    parent = nullptr;
    parenttype = 0;
	size = 1;
}

unsigned int Node::update_size() {
	size = ((left == nullptr) ? 0 : left->size) + ((right == nullptr) ? 0 : right->size) + 1;
    return size;
}


string Node::get_key() {
	return key;
}

unsigned int Node::get_size() {
	return size;
}

Node* Node::get_left() {
	return left;
}

Node* Node::get_right() {
	return right;
}

Node* Node::get_parent() {
    return parent;
}

unsigned int Node::get_parenttype() {
    return parenttype;
}

void Node::set_key(string k) {
	key = k;
}

void Node::set_left(Node* l) {
	left = l;
}

void Node::set_right(Node* r) {
	right = r;
}

void Node::set_parent(Node* p, unsigned int pType) {
    parent = p;
    parenttype = pType;
}

void Node::inc_size() {
    size++;
}
