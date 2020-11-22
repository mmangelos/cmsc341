// CMSC 341 - Spring 2020 - Project 3

#include <vector>
#include "Scanner.h"

// Balance function for use with BSTs.  This implements the
// height-balance property for AVL trees.  Returns true if
// height-balance property is violated.
bool Scanner::heightImbalance(int leftHeight, int rightHeight, int leftSize, int rightSize) {
  return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}

// SNode constructor.  bounds contains the upper and lower bounds on
// the weights for this BST.
Scanner::SNode::SNode(pair<int, int> bounds) {
  _bounds = bounds;
  _root = new BST(heightImbalance);
  _left = _right = nullptr;
}

Scanner::SNode::~SNode() {
  delete _root;
  delete _left;
  delete _right;
  this->_root = nullptr;
  this->_left = nullptr;
  this->_right = nullptr;
}

// Initialize a Scanner object to process an image with 'lines' lines
// of text, each of which is 'range' characters long.  The ASCII
// characters corresponding to the character indices are passed in
// 'chars'.
Scanner::Scanner(int lines, int range, vector<string> chars) {
  pair<int, int> r(0, range);
  this->_forest = new SNode(r);
  this->_lines = lines;
  this->_chars = chars;
}

Scanner::~Scanner() {
  this->deleteSplay(_forest);
}

Scanner::Scanner(const Scanner& rhs) {
  this->_lines = rhs._lines;
  this->_chars = rhs._chars;
  this->_forest = new SNode(rhs._forest->_bounds);
}

Scanner& Scanner::operator=(const Scanner& rhs) {

  if(this != &rhs){
    deleteSplay(_forest);
    this->_lines = rhs._lines;
    this->_chars = rhs._chars;
    this->_forest = buildTree(rhs._forest);
    if(_forest == nullptr){
      pair<int, int> r(0, 0);
      _forest = new SNode(r);
    }
  }
  return *this;
}

// Read the data files and insert into the datastructure.  File in
// 'ascii' contains the scrambled character indices; file 'weights'
// contains the corresponding weights.
bool Scanner::loadFiles(string ascii, string weights) {

  std::ifstream inputA(ascii);
  if(inputA.is_open() == false){
    return false;
  }
  std::ifstream inputW(weights);
  if(inputW.is_open() == false){
    inputA.close();
    return false;
  }

  string a; //temp string for loading ascii values from file
  string w; //temp string for loading weight values from file
  while(getline(inputA, a, ',') && getline(inputW, w, ',')){
    insert(stoi(w), stoi(a));
  }
  
  inputA.close();
  inputW.close();
  return true;
}

// Insert a node; splay the node that is inserted.
bool Scanner::insert(int weight, int ch) {
  if(_forest == nullptr){
    pair<int, int> r(ch, weight);
    _forest->_bounds = r;
    return true;
  }
  return true;
}

void Scanner::dump() const {
  dumpHelper(_forest);
}

void Scanner::inorder() const {
  scannerInorder(_forest);
}

void Scanner::scannerInorder(SNode* currentNode) const{ //implement parantheses for tree structure
  if(currentNode == nullptr){
    return;
  }
  scannerInorder(currentNode->_left);
  cout << "Bounds - " << _forest->_bounds.first
       << ", " << _forest->_bounds.second << endl;
  scannerInorder(currentNode->_right);  
}

void Scanner::dumpHelper(SNode* currentNode) const {
  if(currentNode == nullptr){
    return;
  }
  dumpHelper(currentNode->_left);
  currentNode->_root->dump();
  dumpHelper(currentNode->_right);
}

void Scanner::deleteSplay(SNode* currentNode){
  if(currentNode != nullptr){
    deleteSplay(currentNode->_left);
    deleteSplay(currentNode->_right);
    delete currentNode;
    currentNode = nullptr;
  }
}

Scanner::SNode* Scanner::buildTree(SNode* currentNode) {
  if(currentNode == nullptr){
    return nullptr;
  }
  SNode* newNode = new SNode(currentNode->_bounds);
  newNode->_root = currentNode->_root;
  newNode->_left = buildTree(currentNode->_left);
  newNode->_right = buildTree(currentNode->_right);
  return newNode;
}
