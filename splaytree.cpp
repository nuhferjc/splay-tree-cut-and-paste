#include "splaytree.h"

#include <cassert>
#include <iostream>
using std::cout;
using std::endl;

SplayTree::SplayTree() {
    root = nullptr;
    num_elements = 0;
    deleteable = true;
}

SplayTree::~SplayTree() {
    if (deleteable)
        postorder_delete(root);
}

void SplayTree::postorder_delete(Node* root) {
    if (root == nullptr)
        return;
    postorder_delete(root->get_left());
    postorder_delete(root->get_right());
    delete root;
    root = nullptr;
}

void SplayTree::insert(string k, unsigned int r) {
    assert(r > 0 && r <= num_elements + 1);
    
    Node* splayPtr = nullptr;                  // splayPtr receives a pointer to the node that was inserted, so it may splay it to the root.
    root = insert(root, k, r, splayPtr);
    num_elements++;                            // num_elements is incremented post-insert.
    splay(splayPtr);                           // The node that was inserted is splayed to the root.
}

Node* SplayTree::insert(Node* root, string k, unsigned int r, Node* &splayPtr) {
    if (root == nullptr) {
        splayPtr = new Node(k);
        return splayPtr;
    }
    unsigned int rootSize;
    Node * leftRoot = root->get_left();        // leftRoot is a temporary variable made to reduce memory accesses.
    if (leftRoot == nullptr)
        rootSize = 1;
    else
        rootSize = leftRoot->get_size() + 1;
    if (r <= rootSize) {
        Node* subroot = insert(leftRoot, k, r, splayPtr);   
        root->set_left(subroot);
        subroot->set_parent(root, 1);
        root->inc_size();
    }
    else {
        Node* subroot = insert(root->get_right(), k, r - rootSize, splayPtr);
        root->set_right(subroot);
        subroot->set_parent(root, 2);
        root->inc_size();
    }
    return root;
}

string SplayTree::select(unsigned int r) {
	assert(r > 0 && r <= num_elements);	
    Node* splayPtr = nullptr;                         // splayPtr receives a pointer to the node that was selected, so it may splay it to the root.
    string returnString = select(root, r, splayPtr);
    splay(splayPtr);
    return returnString;
}

string SplayTree::select(Node* root, unsigned int r, Node* &splayPtr) {
    splayPtr = find(root, r);
    return splayPtr->get_key();
}

void SplayTree::modify(unsigned int r, string k) {
	assert(r > 0 && r <= num_elements);
    Node* splayPtr = find(root, r);    
    splayPtr->set_key(k);
    splay(splayPtr);
}

Node* SplayTree::find(Node* root, unsigned int r) {
    unsigned int rootRank;
    if (root->get_left() == nullptr)
        rootRank = 1;
    else
        rootRank = root->get_left()->get_size() + 1;
    if (r == rootRank)
        return root;
    if (r < rootRank)
        return find(root->get_left(), r);
    return find(root->get_right(), r - rootRank);
}

void SplayTree::splay(Node* splayNode) {
    while (splayNode != root) {                        // rotations continue until the Node has reached the root.
        Node* splayParent = splayNode->get_parent();
        Node* splayGrandparent = nullptr;                  // splayGrandparent is only used if the node is 2 rotations or more away from the root.
        if (splayParent != nullptr)                        
            splayGrandparent = splayParent->get_parent();   
        if (splayParent == root) {  // if the node's parent is the root, simply rotate to the root.
            rotate(splayNode, root, true);
        }
        else if (splayNode->get_parenttype() == splayParent->get_parenttype()) {  /* if the node and its parent are both "in line", rotate parent with its grandparent, and then rotate 
                                                                                  the node with its parent. */  
            bool flag = false;                   // this flag indicates to the rotate function whether the rotation will result in a change in the root.
            if (splayGrandparent == root) 
                flag = true;
            rotate(splayParent, splayGrandparent, flag);
            rotate(splayNode, splayParent, flag);
        }
        else {   // otherwise, rotate the node with its parent, and then with its grandparent.
            bool flag = false;
            if (splayGrandparent == root)
                flag = true;
            rotate(splayNode, splayParent, false);
            rotate(splayNode, splayGrandparent, flag);
        }
    }       
}

void SplayTree::rotate(Node* node1, Node* node2, bool flag) {    // node1 is rotated with node2.  Flag indicates whether the rotation will result in node1 becoming the root.
    Node* node1left = node1->get_left();         // node1left, node2left, node1right, and node2parent are variables used to reduce memory accesses.
    Node* node2left = node2->get_left();
    Node* node1right = node1->get_right();
    Node* node2parent = node2->get_parent();
    if (node1 == node2left) {  // If node1 is node2's left child...
        node2->set_left(node1right);
        if (node1right != nullptr)
            node1right->set_parent(node2, 1);  // The 1 indicates that node1right is a left child.
        node1->set_right(node2);
        if (node2parent == nullptr)
            node1->set_parent(node2parent, 0);  // The 0 indicates that node1 has no parent.
        else {
            if (node2->get_parenttype() == 1) {   // If node2 was a left child...
                node2parent->set_left(node1);
                node1->set_parent(node2parent, 1);
            }
            else {    // If node2 was a right child..
                node2parent->set_right(node1);
                node1->set_parent(node2parent, 2);
            }
        }
        node2->set_parent(node1, 2);
    }
    else {  // If node1 is node2's right child...
        node2->set_right(node1left);
        if (node1left != nullptr)
            node1left->set_parent(node2, 2);
        node1->set_left(node2);
        if (node2parent == nullptr)
            node1->set_parent(node2parent, 0);
        else {
            if (node2parent->get_left() == node2) {
                node2parent->set_left(node1);
                node1->set_parent(node2parent, 1);
            }
            else {
                node2parent->set_right(node1);
                node1->set_parent(node2parent, 2);
            }
        }
        node2->set_parent(node1, 1);
    }
    if (flag)
        root = node1;
    node2->update_size();
    node1->update_size();
}

void SplayTree::split(unsigned int r, SplayTree* R) {
	assert(R != nullptr);					// R should be allocated by the user.
	assert(R->root == nullptr);				// The pointer to the right subtree should be nullptr.
	assert(r >= 0 && r <= num_elements);
    if (r == 0) {  // If r is 0, then R will just have all of the elements and the calling tree will be empty.
        R->set_root(root);
        root = nullptr;
        update_size();
        R->update_size();
    }
    else {
        Node* splayPtr = find(root, r);   // splay the node at r to the root, and break off its right subtree and make it R.
	    splay(splayPtr);
        R->set_root(root->get_right());
        R->get_root()->set_parent(nullptr, 0);
        root->set_right(nullptr);
        update_size();
        R->update_size();
    }
}

void SplayTree::join(SplayTree* R) {
	assert(R != nullptr); 				// R should be allocated by the user.
	if (num_elements == 0)              // If there were no elements in the tree, then R just becomes the tree.
        root = R->get_root();
    else {
        Node* splayPtr = find(root, num_elements);   // splay the tree's last element to the root, and then make R's root the tree's right subtree.
        splay(splayPtr);
        root->set_right(R->get_root());
        R->get_root()->set_parent(root, 2);
    }
    update_size();
    R->not_deleteable();  // make R's nodes not deleteable, since they will be deleted by our tree now anyway.
}

void SplayTree::print() {
    print(root);
	cout << "\n";
}

void SplayTree::print(Node* root) {        // Uses an inorder traversal of the tree to print all of its elements in order.
    if (root == nullptr)
        return;
    print(root->get_left());
    std::cout << root->get_key();
    if (root->get_key() != "\n")           // If the key was a new line, no space is printed.  This is done to avoid abnormal spaces at the beginning of lines in output.
        std::cout << " ";
    print(root->get_right());
}

unsigned int SplayTree::size() {
	return num_elements;
}

Node* SplayTree::get_root() {
    return root;
}

void SplayTree::set_root(Node* r) {
    root = r;
}

void SplayTree::update_size() {
    if (root != nullptr)
        num_elements = root->update_size();
    else
        num_elements = 0;
}

void SplayTree::not_deleteable() {
    deleteable = false;
}
