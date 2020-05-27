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
  bool isLcThreaded;	  // Gives context for the the nature of the left pointer
  BSTNode* lc;            // Pointer to left child OR pointer to the inorder predecessor
  bool isRcThreaded;	  // Gives context for the the nature of the right pointer
  BSTNode* rc;            // Pointer to right child OR pointer to the inorder successor

public:
  // Two constructors -- with and without initial values
  BSTNode() { lc = rc = NULL; }
  BSTNode(Key K, E e, BSTNode* l =NULL, BSTNode* r =NULL)
    { k = K; it = e; lc = l; rc = r; }
  ~BSTNode() {}             // Destructor

  // Functions to set and return the value and key
  E& element() { return it; } // Returns the value
  void setElement(const E& e) { it = e; } // Sets the node's value
  Key& key() { return k; } // Returns the key
  void setKey(const Key& K) { k = K; } // Sets the node's key

  // Functions to set and return the children & context variables
  void lcIsThreaded(bool b) { isLcThreaded = b; } // Sets the boolean indicating the context of the left child node
  bool learnIfLcIsThreaded() { return isLcThreaded; } // Returns true if the left child pointer is threaded
  void rcIsThreaded(bool b) { isRcThreaded = b; } // Sets the boolean indicating the context of the right child node
  bool learnIfRcIsThreaded() { return isRcThreaded; } // Returns true if the right child pointer is threaded
  inline BSTNode* left() const { return lc; } // Returns the left child (OR the inorder predecessor)
  void setLeft(BinNode<E>* b) { lc = (BSTNode*)b; } // Sets the left child (OR the inorder predecessor)
  inline BSTNode* right() const { return rc; } // Returns the right child (OR the inorder successor)
  void setRight(BinNode<E>* b) { rc = (BSTNode*)b; } // Sets the right child (OR the inorder successor)

  // Return true if it is a leaf, false otherwise
  bool isLeaf() { return (lc == NULL) && (rc == NULL); } // Need to edit, should return true if (((lc == NULL) && (rc == NULL)) || (isLcThreaded && isRcThreaded))
};

#endif