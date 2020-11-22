// CMSC 341 - Spring 2020 - Project 3

#include "BST.h"

// Constructor for the BNode nested class
BST::BNode::BNode(string ch, int rank, int height, int size) {
  _data = ch;
  _key = rank;
  _height = height;
  _size = size;
  _right = nullptr;
  _left = nullptr;
}

// Constructor for the BST class; requires a balance function
BST::BST(balfn_t imbalanced) {
  _root = nullptr;
  _imbalanced = imbalanced;
}

BST::~BST(){
  if(_root != nullptr){
    deleteTree(_root); //check for memory leak if head not deleted
  }
}

BST::BST(const BST& rhs) {
  this->_root = buildTree(rhs._root);
  this->_imbalanced = rhs._imbalanced;
}

BST& BST::operator=(const BST& rhs){

  if(this != &rhs){
    if(_root != nullptr){
      deleteTree(_root);
    }
    this->_root = buildTree(rhs._root);
    this->_imbalanced = rhs._imbalanced;
  }
  return *this;
}

// Insert a (character, rank) pair
bool BST::insert(string ch, int rank) {
  if(_root == nullptr){
    _root = new BNode(ch, rank, 0, 1);
    return true;
  }else if(recursiveInsert(ch, rank, _root) == false){
    return false;
  }else{
    if(_imbalanced && _root->_size > 2){
      BNode* newTree = getBalancedTree(_root);
      deleteTree(_root);
      _root = newTree;
      _root->_size = updateSizes(_root);
      updateHeightHelper(_root);
    }
    return true;
  }
}

bool BST::recursiveInsert(string ch, int rank, BNode*& compareNode){
  if(compareNode == nullptr){
    compareNode = new BNode(ch, rank, 1);
    return true;
  }else if(compareNode->_key > rank){
    if(recursiveInsert(ch, rank, compareNode->_left)){
      compareNode->_size++;
      return true;
    }
    return false;
  }else if(compareNode->_key < rank){
    if(recursiveInsert(ch, rank, compareNode->_right)){
      compareNode->_size++;
      return true;
    }
    return false;
  }
  return false;
}

int BST::size() const {
  return ( _root == nullptr ? 0 : _root->_size );
}

int BST::height() const {
  return ( _root == nullptr ? -1 : _root->_height );
}

void BST::dump(bool verbose) const {
  this->inorderDump(_root, verbose);
}

void BST::inorderDump(BNode* currentNode, bool verbose) const {
  if(currentNode == nullptr){
    return;
  }
  inorderDump(currentNode->_left, verbose);
  if(verbose){
    cout << "Data: " << currentNode->_data
	 << ", size: " << currentNode->_size
	 << ", height: " << currentNode->_height << endl;
  }else{
    cout << currentNode->_data << " ";
  }
  inorderDump(currentNode->_right, verbose);
}

void BST::deleteTree(BNode* currentNode){

  if(currentNode != nullptr){
    deleteTree(currentNode->_left);
    deleteTree(currentNode->_right);
    delete currentNode;
    currentNode = nullptr;
  }
}

BST::BNode* BST::buildTree(BNode* copyNode){
  if(copyNode == nullptr){
    return nullptr;
  }
  BNode* pasteNode = new BNode(copyNode->_data, copyNode->_key,
			       copyNode->_height, copyNode->_size);
  pasteNode->_left = buildTree(copyNode->_left);
  pasteNode->_right = buildTree(copyNode->_right);
  return pasteNode;
}

BST::BNode* BST::getBalancedTree(BNode* startNode){
  int keys[startNode->_size];
  string data[startNode->_size];
  decompose(startNode, keys, data);
  balanceKeys(keys, data, startNode->_size); 
  return backToBST(0, startNode->_size - 1, keys, data);
}

void BST::decompose(BNode* currentNode, int keys[], string data[]){
  if(currentNode == nullptr){
    return;
  }
  decompose(currentNode->_left, keys, data);
  int i = 0;
  while(data[i] != ""){
    i++;
  }
  keys[i] = currentNode->_key;
  data[i] = currentNode->_data;
  decompose(currentNode->_right, keys, data);
}

void BST::balanceKeys(int keys[], string data[], int length){
  for(int i = 0; i < length - 1; i++){
    for(int j = 0; j < length - i - 1; j++){
      if(keys[j] > keys[j+1]){
	int tempKey = keys[j];
	keys[j] = keys[j+1];
	keys[j+1] = tempKey;
	string tempData = data[j];
	data[j] = data[j+1];
	data[j+1] = tempData;
      }
    }
  }
}

BST::BNode* BST::backToBST(int lowerBound, int upperBound, int keys[], string data[]){
  if(lowerBound > upperBound){
    return nullptr;
  }
  int middle = (lowerBound + upperBound) / 2;
  if( (upperBound + lowerBound) % 2 != 0){
    middle++;
  }
  BNode* newRoot = new BNode(data[middle], keys[middle], 0, 0);
  newRoot->_left = backToBST(lowerBound, middle - 1, keys, data);
  newRoot->_right = backToBST(middle + 1, upperBound, keys, data);
  return newRoot;
}

int BST::updateSizes(BNode* currentNode){
  if(currentNode == nullptr){
    return 0;
  }
  currentNode->_size = updateSizes(currentNode->_left) + updateSizes(currentNode->_right) + 1;
  return currentNode->_size;
}

int BST::updateHeights(BNode* currentNode, int height){
  if(currentNode == nullptr){
    return height - 1;
  }
  int left = updateHeights(currentNode->_left, height + 1);
  int right = updateHeights(currentNode->_right, height + 1);
  if(left > right){
    return left;
  }
  return right;
}


void BST::updateHeightHelper(BNode* currentNode){
  if(currentNode == nullptr){
    return;
  }
  currentNode->_height = updateHeights(currentNode, 0);
  updateHeightHelper(currentNode->_left);
  updateHeightHelper(currentNode->_right);
}

// A sample balance function.  This implements the height-balanced
// property from AVL trees.

bool imbalfn(int leftHeight, int rightHeight, int leftSize, int rightSize) {
  return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}

/*
// A sample main to exercise BST separately.  This is *NOT* a thorough
// test program

int main() {
  
   BST bst( imbalfn );

   bst.insert("A", 5);
   bst.insert("-A", 3);
   bst.insert("-A2", 2);
   bst.insert("-B", 4);
   bst.insert("B", 10);
   bst.insert("C", 15);
   bst.insert("D", 20);
   bst.insert("E", 30);
   bst.insert("F", 40);
   bst.insert("G", 55);
   bst.insert("H", 60);
   bst.insert("I", 65);
   bst.insert("J", 70);
   bst.insert("K", 75);
   bst.insert("L", 80);

   bst.dump();
   bst.dump(true);

   cout << "size = " << bst.size() << endl;
   cout << "height = " << bst.height() << endl;
   
   return 0;
}
*/
