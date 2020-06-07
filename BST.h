// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

// This file includes all of the pieces of the BST implementation

// Include the node implementation
#include "BSTNode.h"

// Include the dictionary ADT
#include "dictionary.h"
#include <string>

#ifndef BST_H
#define BST_H

using namespace std;

// Binary Search Tree implementation for the Dictionary ADT
template <typename Key, typename E>
class BST : public Dictionary<Key,E> {
private:
  BSTNode<Key,E>* root;   // Root of the BST
  int nodecount;         // Number of nodes in the BST
  int lowestKey;
  int highestKey;

  // Private "helper" functions
  void clearhelp(BSTNode<Key, E>*);
  BSTNode<Key,E>* inserthelp(BSTNode<Key, E>*, const Key&, const E&);
  BSTNode<Key,E>* deletemin(BSTNode<Key, E>*);
  BSTNode<Key,E>* getmin(BSTNode<Key, E>*);
  BSTNode<Key,E>* removehelp(BSTNode<Key, E>*, const Key&);
  E* findhelp(BSTNode<Key, E>*, const Key&) const;
  void printhelp(BSTNode<Key, E>*, int) const;
  void vist(BSTNode<Key, E>*) const;
  void simpleThread(BSTNode<Key, E>*);
  void standardThread(BSTNode<Key, E>*, const Key&);
  BSTNode<Key, E>* findPredecessor(BSTNode<Key, E>*, BSTNode<Key, E>*, BSTNode<Key, E>*);
  BSTNode<Key, E>* findSuccessor(BSTNode<Key, E>*, BSTNode<Key, E>*, BSTNode<Key, E>*);
  BSTNode<Key, E>* getNode(BSTNode<Key, E>*, const Key&);
  BSTNode<Key, E>* getParent(BSTNode<Key, E>*, const Key&);
  BSTNode<Key, E>* getLowestParent(BSTNode<Key, E>*, const Key&);
  BSTNode<Key, E>* getHighestParent(BSTNode<Key, E>*, const Key&);
  void printInorderHelp(BSTNode<Key, E>*) const;
  void printReverseHelp(BSTNode<Key, E>*) const;
  BSTNode<Key, E>* leftMostChildInRightSubtree(BSTNode<Key, E>*) const;

public:
	BST() {  // Constructor
		root = NULL; 
		nodecount = 0; 
	} 
  
	//Note from Prof Sipantzi -- I've commented out the destructor
	//since you would have to change clearhelp() to make it work with
	//doubly-threaded trees and that is not part of the assignment.
	//~BST() { clearhelp(root); }            // Destructor

	void clear() { // Reinitialize tree
		clearhelp(root); 
		root = NULL; 
		nodecount = 0;
	}

	// Insert a record into the tree.
	// k Key value of the record.
	// e The record to insert.
	void insert(const Key& k, const E& e) {
		// Insert the new node in the appropriate spot
		root = inserthelp(root, k, e);

		// Increment the number of nodes
		nodecount++;

		// Set the global lowest key variable
		if (nodecount == 1) lowestKey = k;
		if (lowestKey > k) {
			lowestKey = k;
		}

		// Set the global highest key variable
		if (nodecount == 1) highestKey = k;
		if (highestKey < k) {
			highestKey = k;
		}

		// Assign threads
		if (nodecount == 1) {
			simpleThread(root);
		}
		else if (nodecount > 1) {
			standardThread(root, k);
		}
	}

	// Remove a record from the tree.
	// k Key value of record to remove.
	// Return: The record removed, or NULL if there is none.
	E* remove(const Key& k) {
		E* temp = findhelp(root, k);   // First find it
		if (temp != NULL) {
			root = removehelp(root, k);
			nodecount--;
		}
		return temp;
	}
	// Remove and return the root node from the dictionary.
	// Return: The record removed, null if tree is empty.
	E* removeAny() {  // Delete min value
		if (root != NULL) {
			E* temp = new E;
			*temp = root->element();
			root = removehelp(root, root->key());
			nodecount--;
			return temp;
		}
		else return NULL;
	}

	// Return Record with key value k, NULL if none exist.
	// k: The key value to find. */
	// Return some record matching "k".
	// Return true if such exists, false otherwise. If
	// multiple records match "k", return an arbitrary one.
	E* find(const Key& k) const { 
		return findhelp(root, k); 
	}

	// Return the number of records in the dictionary.
	int size() { 
		return nodecount; 
	}

	// Print the contents of the BST
	void print() const {
		if (root == NULL) cout << "The BST is empty.\n";
		else {
			cout << "Printing with printhelp()..." << endl;
			printhelp(root, 0);
		}
	}

	// Print the contents of the BST
	void printInorder() const {
		if (root == NULL) cout << "The BST is empty.\n";
		else {
			printInorderHelp(root);
			cout << "\n" << endl;
		}
	}

	// Print the contents of the BST
	void printReverse() const {
		if (root == NULL) cout << "The BST is empty.\n";
		else {
			cout << "Printing inorder/reverse..." << endl;
			printReverseHelp(root);
		}
	}
};

// Visit -- prints out root
template <typename Key, typename E>
void BST<Key, E>::vist(BSTNode<Key,E>* r) const {
    cout << "Node - " << r->element() << endl;
}

// Clean up BST by releasing space back free store
template <typename Key, typename E>
void BST<Key, E>::clearhelp(BSTNode<Key, E>* root) {
  if (root == NULL) return;
  clearhelp(root->left());
  clearhelp(root->right());
  delete root;
}

// Get the minimum value
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::getmin(BSTNode<Key, E>* rt) {
  if (rt->left() == NULL)
    return rt;
  else return getmin(rt->left());
}

// Delete the minimum value from the BST, returning the revised BST
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::deletemin(BSTNode<Key, E>* rt) {
  if (rt->left() == NULL) // Found min
    return rt->right();
  else {                      // Continue left
    rt->setLeft(deletemin(rt->left()));
    return rt;
  }
}

// Remove a node with key value k
// Return: The tree with the node removed
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::removehelp(BSTNode<Key, E>* rt, const Key& k) {
  if (rt == NULL) return NULL;    // k is not in tree
  else if (k < rt->key())
    rt->setLeft(removehelp(rt->left(), k));
  else if (k > rt->key())
    rt->setRight(removehelp(rt->right(), k));
  else {                            // Found: remove it
    BSTNode<Key, E>* temp = rt;
    if (rt->left() == NULL) {     // Only a right child
      rt = rt->right();         //  so point to right
      delete temp;
    }
    else if (rt->right() == NULL) { // Only a left child
      rt = rt->left();          //  so point to left
      delete temp;
    }
    else {                    // Both children are non-empty
      BSTNode<Key, E>* temp = getmin(rt->right());
      rt->setElement(temp->element());
      rt->setKey(temp->key());
      rt->setRight(deletemin(rt->right()));
      delete temp;
    }
  }
  return rt;
}

// Find a node with the given key value
template <typename Key, typename E>
E* BST<Key, E>::findhelp(BSTNode<Key, E>* root, const Key& k) const {
  if (root == NULL) return NULL;          // Empty tree
  if (k < root->key())
    return findhelp(root->left(), k);   // Check left
  else if (k > root->key())
    return findhelp(root->right(), k);  // Check right
  else {
      E* temp = new E;
      *temp = root->element();
      return temp;  // Found it
  }
}

// Print out a BST
template <typename Key, typename E>
void BST<Key, E>::printhelp(BSTNode<Key, E>* root, int level) const {
  if (root == NULL) return;           // Empty tree
  if (root->lcThreadStatus() != true) { // Don't follow a thread
	  printhelp(root->left(), level + 1);   // Do left subtree
  }
  for (int i=0; i<level; i++)         // Indent to level
    cout << "  ";
  cout << root->key() << "\n";        // Print node value
  if (root->rcThreadStatus() != true) { // Don't follow a thread
	  printhelp(root->right(), level + 1);  // Do right subtree
  }
}

// Insert a node into the BST, returning the updated tree
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::inserthelp(BSTNode<Key, E>* root, const Key& k, const E& it) {
	if (root == NULL) {
		return new BSTNode<Key, E>(k, it, false, NULL, false, NULL);
	}
	if (k < root->key()) { // If the new node key is less than the root key, left turn
		if (root->lcThreadStatus() == true) { // If the "node" there isn't real because the thread status is true
			root->lcIsThreaded(false); // Set the thread status of the parent to false because that's where we will insert the new node
			BSTNode<Key, E>* temp = new BSTNode<Key, E>(k, it, false, NULL, false, NULL); // Insert the new node without any threads
			root->setLeft(temp); // Set it as the left node of the current root (the new node's parent)
		}
		else { // There's actually a regular node there, keep traversing
			root->setLeft(inserthelp(root->left(), k, it));
		}
	}
	else { // If the new node key is greater than the root key, right turn
		if (root->rcThreadStatus() == true) { // If the "node" there isn't real because the thread status is true
			root->rcIsThreaded(false); // Set the thread status of the parent to false because that's where we will insert the new node
			BSTNode<Key, E>* temp = new BSTNode<Key, E>(k, it, false, NULL, false, NULL); // Insert the new node without any threads
			root->setRight(temp); // Set it as the right node of the current root (the new node's parent)
		}
		else {// There's actually a regular node there, keep traversing
			root->setRight(inserthelp(root->right(), k, it));
		}
	}
	return root; // Return tree with node inserted
}

// Print inorder
template <typename Key, typename E>
void BST<Key, E>::printInorderHelp(BSTNode<Key, E>* root) const {
	if (root == NULL) return;           // Empty tree

	// Find where to begin by looking for the node with the lowest key
	BSTNode<Key, E>* startingPlace = NULL;
	while (startingPlace == NULL) {
		if (root->lcThreadStatus() != true) {
			root = root->left();
		}
		else {
			startingPlace = root;
		}
	}
	BSTNode<Key, E>* curr = startingPlace;

	while (curr != NULL) { // While current node is a node
		cout << curr->element() << endl; // Print its info
		if (curr->rcThreadStatus() == true && curr->right() != NULL) {
			curr = curr->right();
		}
		else {
			curr = leftMostChildInRightSubtree(curr->right());
		}
	}
	return;
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::leftMostChildInRightSubtree(BSTNode<Key, E>* root) const {
	if (root == NULL) return NULL;
	while (root->left() != NULL && root->lcThreadStatus() == false) {
		root = root->left();
	}
	return root;
}

// Print Reverse
template <typename Key, typename E>
void BST<Key, E>::printReverseHelp(BSTNode<Key, E>* root) const {
	if (root == NULL) return;           // Empty tree

	// Find where to begin by looking for the node with the lowest key
	BSTNode<Key, E>* startingPlace = NULL;
	while (startingPlace == NULL) {
		if (root->rcThreadStatus() != true) {
			root = root->right();
		}
		else {
			startingPlace = root;
		}
	}
	BSTNode<Key, E>* curr = startingPlace;

	while (curr != NULL) { // While current node is a node
		cout << curr->element() << endl; // Print its info
		if (curr->rcThreadStatus() == true && curr->right() != NULL) {
			curr = curr->right();
		}
		else {
			curr = leftMostChildInRightSubtree(curr->right());
		}
	}
	return;
}

// Assign the successor and set the context variable
template <typename Key, typename E>
void BST<Key, E>::simpleThread(BSTNode <Key, E>* root) {
	root->setLeft(root); // Set left thread to root
	root->lcIsThreaded(true); // Threaded back to itself
	root->setRight(root); // Set right thread to root
	root->rcIsThreaded(true); // Threaded back to itself
	return;
}

// Assign the successor and set the context variable
template <typename Key, typename E>
void BST<Key, E>::standardThread(BSTNode <Key, E>* root, const Key& k) {
	// Get pointers to the new node and its parent
	BSTNode<Key, E>* temp = getNode(root, k);
	BSTNode<Key, E>* par;
	if (lowestKey != k && highestKey != k) {
		par = getParent(root, k);
	}
	else if (lowestKey == k) {
		par = getLowestParent(root, k);
	}
	else if (highestKey == k) {
		par = getHighestParent(root, k);
	}

	// If the node is the left-most node (the first node printed when printing in order)
	if (lowestKey == k) {
		temp->lcIsThreaded(true); 
		temp->setLeft(NULL); // Thread left most node's left child to itself
		temp->rcIsThreaded(true); 
		temp->setRight(findSuccessor(root, temp, NULL)); // Thread left most node's right child to its inorder successor
	}
	// If the node is the right-most node (the last node printed when printing in order)
	if (highestKey == k) {
		temp->lcIsThreaded(true);
		temp->setLeft(findPredecessor(root, temp, NULL)); // Thread right most node's left child to its inorder successor
		temp->rcIsThreaded(true);
		temp->setRight(NULL); // Thread right most node's right child to itself
	}
	// If the node is not the left or right most node
	if (lowestKey != k && highestKey != k) {
		// Find the predecessor
		BSTNode<Key, E>* pred = findPredecessor(root, temp, NULL);
		// Assign the predecessor to the left thread
		temp->setLeft(pred);
		temp->lcIsThreaded(true);
		// Find the successor
		BSTNode<Key, E>* succ = findSuccessor(root, temp, NULL);
		// Assign the successor to the left thread
		temp->setRight(succ);
		temp->rcIsThreaded(true);
	}

	// Adjust the parent
	if (par->left() == temp) { // If the new node was inserted as the left child of the parent
		par->lcIsThreaded(false);
	}
	else if (par->right() == temp) { // If the new node was inserted as the right child of the parent
		par->rcIsThreaded(false);
	}
	return;
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::getNode(BSTNode<Key, E>* root, const Key& k) {
	if (root == NULL) return NULL;
	if (root->key() == k) {
		return root;
	}
	if (root->key() > k) {
		root = root->left();
		getNode(root, k);
	}
	else if (root->key() < k) {
		root = root->right();
		getNode(root, k);
	}
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::getParent(BSTNode<Key, E>* root, const Key& k) {
	if (root == NULL) return NULL;
	if (root->left() != NULL) {
		if (root->left()->key() == k) {
			return root;
		}
	}
	if (root->right() != NULL) {
		if (root->right()->key() == k) {
			return root;
		}
	}
	if (root->key() > k) {
		root = root->left();
		getParent(root, k);
	}
	else if (root->key() < k) {
		root = root->right();
		getParent(root, k);
	}
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::getLowestParent(BSTNode<Key, E>* root, const Key& k) {
	if (root == NULL) return NULL;
	if (root->left() != NULL) {
		if (root->left()->key() == k) {
			return root;
		}
		root = root->left();
		getLowestParent(root, k);
	}
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::getHighestParent(BSTNode<Key, E>* root, const Key& k) {
	if (root == NULL) return NULL;
	if (root->right() != NULL) {
		if (root->right()->key() == k) {
			return root;
		}
		root = root->right();
		getHighestParent(root, k);
	}
}

// Find the inorder predecessor
// Citation: https://www.geeksforgeeks.org/inorder-predecessor-successor-given-key-bst/
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::findPredecessor(BSTNode <Key, E>* root, BSTNode<Key, E>* nodeInQuestion, BSTNode<Key, E>* predecessor = NULL) {
	if (root == NULL) return NULL;
	while (root != NULL) {
		if (nodeInQuestion->key() > root->key()) {
			predecessor = root;
			root = root->right();
		}
		else if (nodeInQuestion->key() < root->key()) {
			root = root->left();
		}
		else {
			break;
		}
	}
	return predecessor;
}

// Find the inorder successor
// Citation: https://www.geeksforgeeks.org/inorder-predecessor-successor-given-key-bst/
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::findSuccessor(BSTNode <Key, E>* root, BSTNode<Key, E>* nodeInQuestion, BSTNode<Key, E>* successor = NULL) {
	if (root == NULL) return NULL;
	while (root != NULL) {
		if (nodeInQuestion->key() < root->key()) {
			successor = root;
			root = root->left();
		}
		else if (nodeInQuestion->key() > root->key()) {
			root = root->right();
		}
		else {
			break;
		}
	}
	return successor;
}

#endif