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
  BSTNode<Key, E>* findPredecessor(BSTNode<Key, E>*, BSTNode<Key, E>*);
  BSTNode<Key, E>* findSuccessor(BSTNode<Key, E>*, BSTNode<Key, E>*);
  const Key& getLeftMostNode(BSTNode <Key, E>* root);
  const Key& getRightMostNode(BSTNode <Key, E>* root);
  BSTNode<Key, E>* getNode(BSTNode<Key, E>* root, const Key&);
  //void printInorder(BSTNode<Key, E>*, int) const;
  //void printReverse(BSTNode<Key, E>*, int) const;

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
		cout << "-------Adding " << k << "--------" << endl;

		// Insert the new node in the appropriate spot
		root = inserthelp(root, k, e);

		// Increment the number of nodes
		nodecount++;

		// Assign threads
		if (nodecount == 1) {
			simpleThread(root);
		}
		else if (nodecount > 1) {
			cout << "Calling standardThread" << endl;
			standardThread(root, k);
		}

		cout << "-------Added " << k << "--------\n" << endl;
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
  printhelp(root->left(), level+1);   // Do left subtree
  for (int i=0; i<level; i++)         // Indent to level
    cout << "  ";
  cout << root->key() << "\n";        // Print node value
  printhelp(root->right(), level+1);  // Do right subtree
}

// Insert a node into the BST, returning the updated tree
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::inserthelp(BSTNode<Key, E>* root, const Key& k, const E& it) {
	if (root == NULL) {
		return new BSTNode<Key, E>(k, it, false, NULL, false, NULL);
	}
	if (k < root->key()) { // If the new node key is less than the root key, left turn
		if (root->lcThreadStatus() == true) { // if the left child pointer is threaded, make a new node
			root->lcIsThreaded(false); // // Set the thread status of the new parent's left child to false
			return new BSTNode<Key, E>(k, it, false, NULL, false, NULL);
		}
		else {
			root->setLeft(inserthelp(root->left(), k, it));  // Run down the left subtree to find an empty left child
		}
	}
	else { // If the new node key is greater than the root key, right turn
		if (root->rcThreadStatus() == true) { // if the left child pointer is threaded, make a new node
			root->rcIsThreaded(false); // Set the thread status of the new parent's right child to false
			return new BSTNode<Key, E>(k, it, false, NULL, false, NULL);
		}
		else {
			root->setRight(inserthelp(root->right(), k, it)); // Run down the right subtree
		}
	}
	return root;       // Return tree with node inserted
}

//// Print inorder
//template <typename Key, typename E>
//void BST<Key, E>::printInorder(BSTNode<Key, E>* root, int level) const {
//	if (root == NULL) return;           // Empty tree
//	printInorder(root->left(), level + 1);   // Do left subtree
//	for (int i = 0; i < level; i++)         // Indent to level
//		cout << "  ";
//	cout << root->key() << "\n";        // Print node value
//	printInorder(root->right(), level + 1);  // Do right subtree
//}
//
//// Print Reverse
//template <typename Key, typename E>
//void BST<Key, E>::printReverse(BSTNode<Key, E>* root, int level) const {
//	if (root == NULL) return;           // Empty tree
//	printReverse(root->right(), level + 1);  // Do right subtree
//	for (int i = 0; i < level; i++)         // Indent to level
//		cout << "  ";
//	cout << root->key() << "\n";        // Print node value
//	printReverse(root->left(), level + 1);   // Do left subtree
//}

// Find the left most node
template <typename Key, typename E>
const Key& BST<Key, E>::getLeftMostNode(BSTNode <Key, E>* root) {
	if (root == NULL) return NULL;
	if (root->left() != NULL) {
		getLeftMostNode(root);
	}
	else {
		return root->key();
	}
}

// Find the right most node
template <typename Key, typename E>
const Key& BST<Key, E>::getRightMostNode(BSTNode <Key, E>* root) {
	if (root == NULL) return NULL;
	if (root->right() != NULL) {
		getRightMostNode(root);
	}
	else {
		return root->key();
	}
}

// Assign the successor and set the context variable
template <typename Key, typename E>
void BST<Key, E>::simpleThread(BSTNode <Key, E>* root) {
	root->setLeft(root); // Set left thread to root
	root->lcIsThreaded(true); // Threaded back to itself
	root->setRight(root); // Set right thread to root
	root->rcIsThreaded(true); // Threaded back to itself
	cout << "Root left child is " << root->left()->key() << endl;
	cout << "Root right child is " << root->right()->key() << endl;
	return;

}

// Assign the successor and set the context variable
template <typename Key, typename E>
void BST<Key, E>::standardThread(BSTNode <Key, E>* root, const Key& k) {
	// Get a reference to the new node
	BSTNode<Key, E>* temp = getNode(root, k);

	// If the node isn't the left-most node
	if (getLeftMostNode(root) != k) {
		// Find the predecessor

	}
	if (getRightMostNode(root) != k) {
		// Find the successor

	}
}

template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::getNode(BSTNode<Key, E>* root, const Key& k) {
	if (root == NULL) return NULL;
	if (root->key() == k) {
		cout << "found node " << k << " in getNode" << endl;
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

// Find the inorder predecessor
// Citation: https://www.geeksforgeeks.org/inorder-predecessor-successor-given-key-bst/
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::findPredecessor(BSTNode <Key, E>* root, BSTNode<Key, E>* predecessor = NULL) {
	if (root == NULL) return;
	BSTNode<Key, E>* left = root->left();
	BSTNode<Key, E>* right = root->right();
	if (left != NULL) {
		BSTNode<Key, E>* temp = left;
		while (temp->right() != NULL) {
			temp = temp->right();
		}
		predecessor = temp;
	}
	if (right != NULL) {
		BSTNode<Key, E>* temp2 = right;
		while (temp2->left() != NULL) {
			temp2 = temp2->left();
		}
		successor = temp2;
	}
	if (predecessor != NULL) {
		setThreadedPredecessor(root, predecessor);
	}
	if (successor != NULL) {
		setThreadedSuccessor(root, successor);
	}
	return;
}

// Find the inorder successor
// Citation: https://www.geeksforgeeks.org/inorder-predecessor-successor-given-key-bst/
template <typename Key, typename E>
BSTNode<Key, E>* BST<Key, E>::findSuccessor(BSTNode <Key, E>* root, BSTNode<Key, E>* successor = NULL) {
	if (root == NULL) return;
	BSTNode<Key, E>* left = root->left();
	BSTNode<Key, E>* right = root->right();
	if (left != NULL) {
		BSTNode<Key, E>* temp = left;
		while (temp->right() != NULL) {
			temp = temp->right();
		}
		predecessor = temp;
	}
	if (right != NULL) {
		BSTNode<Key, E>* temp2 = right;
		while (temp2->left() != NULL) {
			temp2 = temp2->left();
		}
		successor = temp2;
	}
	if (predecessor != NULL) {
		setThreadedPredecessor(root, predecessor);
	}
	if (successor != NULL) {
		setThreadedSuccessor(root, successor);
	}
	return;
}

#endif