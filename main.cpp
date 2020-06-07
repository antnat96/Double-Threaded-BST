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

	cout << "Inserting my key value pairs..." << endl;

	// Data
	const int intArr[13] = { 77,70,75,66,79,68,67,69,90,85,83,87,65 };
	string strArr[13] = { "seventy-seven","seventy","seventy-five","sixty-six",
		"seventy-nine","sixty-eight","sixty-seven","sixty-nine","ninety",
		"eighty-five","eighty-three","eighty-seven","sixty-five" };

	for (int i = 0; i < 2; i++) {
		myBST.insert(intArr[i], strArr[i]);
	}

	cout << "Inserted!" << endl;

	cout << "Size of my BST is " << myBST.size() << "\n" << endl;

	//cout << "Regular print:" << endl;

	//myBST.print();

	//cout << "\n" << "Clearing my BST..." << endl;

	//myBST.clear();

	//cout << "Cleared!" << endl;

	//cout << "Size of my BST is " << myBST.size() << endl;

	return 0;
}