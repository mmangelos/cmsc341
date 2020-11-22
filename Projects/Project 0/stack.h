// CMSC 341 - Spring 2020 - Project 0
// stack.h: a templated, linked list-based stack implementation

// Assignment:
//   (1) Complete the copy constructor
//   (2) Complete the assignment operator
//   (3) Complete the destructor
//   (4) Write a test program (mytest.cpp) to test copy and assignment
//   (5) Verify destructor by running the test program in Valgrind

#ifndef _STACK_H
#define _STACK_H

#include <iostream>
#include <exception>

using std::ostream;
using std::cout;
using std::endl;
using std::range_error;

// Forward declarations
template <class T> class Stack;
template <class T> class Node;
template <class T> ostream& operator<<(ostream&, const Node<T>&);

// Node class for linked list
template <class T>
class Node {

  friend Stack<T>;
  
public:
  Node(T data = T(), Node<T> *next = nullptr) {
    _data = data;
    _next = next;
  }

  friend ostream& operator<< <T>(ostream &sout, const Node<T> &x);
  
private:
  T _data;
  Node *_next;
};

// Overloaded insertion operator.  Must be overloaded for the template
// class T, or this won't work!
template <class T>
ostream& operator<<(ostream &sout, const Node<T> &x) {
  sout << "Data: " << x._data;
  return sout;
}


// Stack class.  Linked-list implementation of a stack. Uses the Node
// class.
template <class T>
class Stack {
public:
  // Constructor
  Stack();

  // Copy constructor, assignment operator, and destructor
  // DO NOT IMPLEMENT HERE.  SEE BELOW.
  Stack(const Stack &rhs);
  const Stack& operator=(const Stack& rhs);
  ~Stack();

  // Stack operations: push(), top(), and pop()
  void push(const T& data);
  const T& top() const;
  void pop();

  // Helpful functions
  bool empty() const;  // Returns 'true' if stack is empty
  void dump() const;   // Dump contents of the linked list

private:
  Node<T> *_head;

  // ***************************************************
  // Any private helper functions must be delared here!
  // ***************************************************

  void deleteList(Node<T>* currentNode) const; // deallocates memory of all nodes and resets the stack
  
};

template <class T>
Stack<T>::Stack() {
  _head = nullptr;
}

template <class T>
Stack<T>::Stack(const Stack<T>& rhs) {

  if(rhs._head != nullptr){ //if the head is not null, then proceed
    
    _head = new Node<T>(rhs._head->_data); //create a deep copy of the head
    Node<T>* temp = _head; //tracks this object's nodes
    Node<T>* tempRhs = rhs._head; //tracks the passed object's nodes
    while(tempRhs->_next != nullptr){ //while there is still stack to copy
      temp->_next = new Node<T>(tempRhs->_next->_data);
      tempRhs = tempRhs->_next;
      temp = temp->_next;
    }    
  }else{ //there is nothing to copy, so make the head a null pointer.
    _head = nullptr;
  }
}

template <class T>
const Stack<T>& Stack<T>::operator=(const Stack<T>& rhs) {

  if(&rhs != this){ //guards against self assignment

    if(_head != nullptr){ //if the stack is NOT empty...
      deleteList(_head); //...then it deallocates memory from current stack.
    }
    if(rhs._head != nullptr){
      
      _head = new Node<T>(rhs._head->_data); //create a deep copy of the head
      Node<T>* temp = _head; //tracks this object's nodes
      Node<T>* tempRhs = rhs._head; //tracks the passed object's nodes
      while(tempRhs->_next != nullptr){ //while there is still stack to copy
	temp->_next = new Node<T>(tempRhs->_next->_data); //create the next node
	tempRhs = tempRhs->_next; //move one node forward in the stack
	temp = temp->_next; //move one node forward in the stack
      }
    }else{
      _head = nullptr;
    }
  }
  /*
  else{
    cout << "You are attempting to do self assignment. Nothing happened." << endl;
  }
  */
  return *this;
}

template <class T>
Stack<T>::~Stack() {

  if(_head != nullptr){ //if the stack is NOT empty, proceed to delete the stack
    deleteList(_head);
  }
}

template <class T>
void Stack<T>::push(const T& data) {
  Node<T> *tmpPtr = new Node<T>(data);
  tmpPtr->_next = _head;
  _head = tmpPtr;
}

template <class T>
const T& Stack<T>::top() const {
  if ( empty() ) {
    throw range_error("Stack<T>::top(): attempt to read empty stack.");
  }

  return _head->_data;
}

template <class T>
void Stack<T>::pop() {
  if ( empty() ) {
    throw range_error("Stack<T>::pop(): attempt to pop from an empty stack.");
  }

  Node<T> *tmpPtr = _head->_next;
  delete _head;
  _head = tmpPtr;
}

template <class T>
bool Stack<T>::empty() const {
  return _head == nullptr;
}

template <class T>
void Stack<T>::dump() const {
  Node<T> *nodePtr = _head;
  while ( nodePtr != nullptr ) {
    cout << nodePtr->_data << endl;
    nodePtr = nodePtr->_next;
  }
}

/* deleteList(Node<T>* currentNode);
** deleteList recursively runs through the stack
** and deallocates all memory.
*/
template <class T>
void Stack<T>::deleteList(Node<T>* currentNode) const {

  if(currentNode->_next != nullptr){
    deleteList(currentNode->_next);
  }
  delete currentNode;
  currentNode = nullptr;
  return;
}

#endif
