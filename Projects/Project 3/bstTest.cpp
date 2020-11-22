/* bstTest.cpp
** Mitchell Angelos [CV55655]
** CMSC 341 - Project 3
** Professor Wilson
*/

#include <iostream>
#include <string>
#include "BST.cpp"

using std::cout;
using std::endl;
using std::string;

int main() {

  //testing overloaded constructor for a BST
  BST bstOne(imbalfn);
  //testing some insertions
  bstOne.insert("A", 5);
  bstOne.insert("B", 3);
  bstOne.insert("C", 7);
  bstOne.insert("D", 1);
  bstOne.insert("E", 6);
  bstOne.insert("F", 4);
  bstOne.insert("G", 0);
  bstOne.insert("H", -4);
  bstOne.insert("I", -2);
  bstOne.insert("J", -3);
  //size function test, SHOULD BE 10
  cout << "bstOne size: " << bstOne.size() << endl;
  //height function test
  cout << "bstOne height: " << bstOne.height() << endl;
  //verbose dump test
  bstOne.dump(true);
  
  //testing copy constructor
  cout << "Copy constructor test" << endl;
  BST bstTwo(bstOne);
  bstTwo.insert("K", 42); //check for deep copy
  cout << "bstTwo dump to check for deep copy: ";
  bstTwo.dump(false);
  cout << endl;
  cout << "bstOne dump to make sure it's not a shallow copy: ";
  bstOne.dump(false);
  cout << endl;
  
  //testing overloaded assignment operator
  bstOne = bstOne; //self assignment test
  cout << "Overloaded assignment operator test" << endl;
  BST bstThree = bstOne;
  bstThree.insert("L", 312);
  cout << "bstTree dump to check for deep copy: ";
  bstThree.dump(false);
  cout << endl;
  cout << "bstOne dump to make sure it's not a shallow copy: ";
  bstOne.dump(false);
  cout << endl;


  return 0;
}
