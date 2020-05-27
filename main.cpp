/*
* File:   main.cpp
* Author: Anthony Natale CSIS 215
*
* Created on May 27, 2020
* Implements the double threaded binary tree structure
*/

#include <string>
#include <sstream>
#include "BST.h"

using namespace std;

int main(int argc, char** argv) {

	cout << "Anthony Natale -- CSIS 215 Programming Assignment 2 -- Double Threaded Binary Tree" << "\n" << endl;

	BST<int, string> myBST;

	cout << "Size of my BST is " << myBST.size() << endl;

	const int testInt = 5;
	string testString = "Five";

	myBST.insert(testInt, testString);

	cout << "Size of my BST is " << myBST.size() << endl;

	myBST.clear();

	cout << "Size of my BST is " << myBST.size() << endl;


	return 0;
}