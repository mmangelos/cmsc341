/*
** mytest.cpp
** Mitchell Angelos (CV55655)
*/
#include <iostream>
#include "stack.h"
using namespace std;

int main(){

  Stack<int> stackOne;

  for(int i = 1; i <= 5; i++){
    stackOne.push(i);
  }
  stackOne.dump();
  cout << endl;

  //tests copy constructor
  Stack<int> stackTwo(stackOne);
  stackTwo.pop(); //pop twice here to ensure that stack two is a deep copy.
  stackTwo.pop();
  stackTwo.dump();
  cout << endl;

  //tests assignment operator
  Stack<int> stackThree = stackOne;
  stackThree.pop(); //pop once to ensure that stack three is a deep copy.
  cout << "Stack one dump" << endl;
  stackOne.dump();
  cout << "Stack three dump" << endl;
  stackThree.dump();
  cout << endl;

  Stack<int> stackFour; //test to make sure all contents are deleted in stack after
  //using assignment operator. this is used with valgrind.
  for(int i = 1; i <= 10; i++){
    stackFour.push(i);
  }
  stackFour = stackOne;
  stackOne.dump();
  cout << endl;

  //test edge cases
  stackThree = stackThree; //tests for self assignment

  Stack<int> stackFive;
  stackFour = stackFive; //tests for empty stack and deleting current items in stack four
  stackFive = stackOne;
  
  return 0;
}
