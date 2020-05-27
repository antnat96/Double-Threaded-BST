// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

// This is the file to include for access to the complete binary node
// template implementation

#include "book.h"
#include "BinNode.h"

#ifndef BSTNODE_H
#define BSTNODE_H

// Simple binary tree node implementation
template <typename Key, typename E>
class BSTNode : public BinNode<E> {
private:
  Key k;                  // The node's key
  E it;                   // The node's value
  bool isLcThreaded;	  // Gives context for the the nature of the left pointer (set to true if it is threaded)
  BSTNode* lc;            // Pointer to left child OR pointer to the inorder predecessor
  bool isRcThreaded;	  // Gives context for the the nature of the right pointer (set to true if it is threaded)
  BSTNode* rc;            // Pointer to right child OR pointer to the inorder successor

public:
  // Two constructors -- with and without initial values
  BSTNode() { lc = rc = NULL; }
  BSTNode(Key K, E e, bool leftContext = false, BSTNode* l =NULL, bool rightContext = false, BSTNode* r =NULL)
  {
	  k = K; 
	  it = e; 
	  isLcThreaded = leftContext; 
	  lc = l; 
	  isRcThreaded = rightContext; 
	  rc = r;
  }
  ~BSTNode() {}             // Destructor

  // Functions to set and return the value and key
  E& element() { return it; } // Returns the value
  void setElement(const E& e) { it = e; } // Sets the node's value
  Key& key() { return k; } // Returns the key
  void setKey(const Key& K) { k = K; } // Sets the node's key

  // Functions to set and return the children & context variables
  void lcIsThreaded(bool b) { // Indicates context of left child - if true, the left child points to an inorder predecessor
	  isLcThreaded = b; 
  } 
  bool lcThreadStatus() { return isLcThreaded; } // Returns true if the left child pointer is threaded
  void rcIsThreaded(bool b) { // Indicates context of right child - if true, the right child points to an inorder successor
	  isRcThreaded = b; 
  } 
  bool rcThreadStatus() { return isRcThreaded; } // Returns true if the right child pointer is threaded
  inline BSTNode* left() const { return lc; } // Returns the left child (OR the inorder predecessor)
  void setLeft(BinNode<E>* b) { lc = (BSTNode*)b; } // Sets the left child (OR the inorder predecessor)
  inline BSTNode* right() const { return rc; } // Returns the right child (OR the inorder successor)
  void setRight(BinNode<E>* b) { rc = (BSTNode*)b; } // Sets the right child (OR the inorder successor)

  // Return true if the node is a leaf, false otherwise
  bool isLeaf() {
	  // If both children are null
	  if ((lc == NULL) && (rc == NULL)) {
		  return true; // Node is obviously a leaf
	  }

	  // If not, count how many children there are
	  int numOfChildren = 0;
	  if (lc != NULL) { numOfChildren++; }
	  if (rc != NULL) { numOfChildren++; }

	  // Count the number of children that are threaded
	  int numOfThreadedChildren = 0;
	  if (isLcThreaded) { numOfThreadedChildren++; }
	  if (isRcThreaded) { numOfThreadedChildren++; }

	  // Check if all children are threaded children
	  if (numOfChildren == numOfThreadedChildren) { return true; } // If they are, the node is a leaf

	  return false; // Otherwise, the node is not a leaf
  }
};

#endif