// CMSC 341 - Spring 2020 - Project 2
#ifndef _RMQLIST_H
#define _RMQLIST_H

#include <iostream>
#include <stdexcept>
#include <cmath>

using std::swap;
using std::ostream;
using std::cout;
using std::endl;
using std::sqrt;
using std::range_error;
using std::invalid_argument;

// Macro for a two-argument min function
#define MIN(a, b)  ((a) < (b) ? (a) : (b))

// forward declarations
template <class K, class V> class RMQList;
template <class K, class V> class Node;
template <class K, class V> ostream& operator<<(ostream &sout, const Node<K,V> &x);

// *********************************************
// Node - node class for the RMQList linked list
//   key and value are templated (types K and V)
// *********************************************

template <class K, class V>
class Node {
  friend RMQList<K, V>;
public:
  Node(K key = K(), V value = V(), Node<K, V> *next = nullptr) {
    _key = key;
    _value = value;
    _next = next;
  }
  friend ostream& operator<< <K,V>(ostream &sout, const Node<K,V> &x);
private:
  K _key;
  V _value;
  Node *_next;
};

// Overloaded insertion operator for Node
template <class K, class V>
ostream& operator<<(ostream &sout, const Node<K,V> &x) {
  sout << "Key: " << x._key << ", Value: " << x._value;
  return sout;
}

// *******************************************************
// RMQList - list container (linked list) with RMQ support
//   key and value are templated (types K and V)
// *******************************************************

template <class K, class V>
class RMQList {
public:
  // Create an empty RMQList object
  RMQList();
  
  // Destructor, Copy Constructor, Assignment Operator
  ~RMQList();
  RMQList(const RMQList<K,V> &rhs);
  const RMQList<K,V>& operator=(const RMQList<K,V> &rhs);
  
  // In-line function. Returns the size (number of elements).
  int size() const { return _listSize; }

  // In-line function. Returns true if the list is empty; false
  // otherwise.
  bool empty() const { return _head == nullptr; }
  
  // Insert an element into the list; list must be kept in increasing
  // order by key; duplicate keys are not allowed, so return false if
  // there is already an entry with the specified key, true otherwise.
  // Should check if key > largest current key and, if so, append to
  // the end of the list without iteration.
  bool insert(const K& key, const V& value);

  // Remove an element from the list; return false if no element
  // exists with the specified key value, true otherwise
  bool remove(const K& key);

  // Update value for the element with given key; return false if
  // there is no element with the given key, true otherwise
  bool update(const K& key, const V& value);
  
  // RMQ Query for k1 to k2.  Throws exceptions (see documentation).
  V query(const K& k1, const K& k2);

  // Dump the list entries
  void dumpList() const;
  
  // Dump the RMQ info and table.  What gets dumped depends on which
  // RMQ method is used.
  void dumpTable() const;

  // Clear the data data strucures
  void clear();
  
 private:
  Node<K,V> *_head;
  Node<K,V> *_tail;
  int _listSize;

  // **********************************
  // Private variables for RMQ go here!
  // **********************************

  const int KEY_DNE = -1; //number to represent that the key you're searching
  const int SINGLE_BLOCK_SIZE = 3; //a single block size for block decomposition
  V* _mins = nullptr; //all minimums stored
  int _blockSize;
  int _blockAmount;
  
  // *******************************************
  // Declarations for private functions go here!
  // *******************************************

  void calcMins(); //precomputation of minimums
  int keyExistsAt(const K& key); //find if the key you're searching for
  // already exists in the list
  void deleteList(Node<K,V>* currentNode); //recursive deletion of the entire list
};


template <class K, class V>
RMQList<K,V>::RMQList() {
  this->_head = nullptr;
  this->_tail = _head;
  this->_listSize = 0;
}

template <class K, class V>
RMQList<K,V>::~RMQList() {
  deleteList(_head);
  _head = nullptr;
  _tail = nullptr;
  if(_mins != nullptr){
    delete[] _mins;
    _mins = nullptr;
  }
}

template <class K, class V>
RMQList<K,V>::RMQList(const RMQList<K,V> &rhs) {
  
  this->_listSize = rhs._listSize;  
  Node<K,V>* temp = rhs._head;
  Node<K,V>* tempNew = new Node<K,V>(temp->_key, temp->_value);
  this->_head = tempNew;
  while(temp->_next != nullptr){
    temp = temp->_next;
    tempNew->_next = new Node<K,V>(temp->_key, temp->_value);
    tempNew = tempNew->_next;
  }
  if(_head->_next != nullptr){    
    this->_tail = tempNew;
  }else{
    this->_tail = _head;
  }
  temp = nullptr;

  calcMins();
}

template <class K, class V>
const RMQList<K,V>& RMQList<K,V>::operator=(const RMQList<K,V> &rhs) {

  if(this != &rhs){
    
    this->clear();
    //head is already initialized to new node, head and tail point to correct place already
    this->_listSize = rhs._listSize;
    Node<K,V>* temp = rhs._head;
    this->_head->_key = temp->_key;
    this->_head->_value = temp->_value;
    Node<K,V>* tempNew = _head;
    while(temp->_next != nullptr){
      temp = temp->_next;
      tempNew->_next = new Node<K,V>(temp->_key, temp->_value);
      tempNew = tempNew->_next;
    }
    if(_head->_next != nullptr){
      this->_tail = tempNew;
    }else{
      this->_tail = _head;
    }
    temp = nullptr;

    calcMins();
  }

  return *this;
}

template <class K, class V>
bool RMQList<K,V>::insert(const K& key, const V& value) {
  //creates a new node to be inserted
  Node<K,V>* newNode = new Node<K,V>(key, value);
  //if the list is empty, add a new head
  if(_listSize == 0){
    _head = newNode;
    _tail = _head;
    _listSize++;
    return true;
  }
  //if the new node is a head node, add it to the head
  if(_head->_key > key){
    newNode->_next = _head;
    _head = newNode;
    _listSize++;
    calcMins();
    return true;
  }
  //if the new node is a tail node, add it to the tail
  if(_tail->_key < key){
    _tail->_next = newNode;
    _tail = newNode;
    _listSize++;
    calcMins();
    return true;
  }
  //find if the key already exists in the list
  int index = keyExistsAt(key);
  //if the key does exist, you cannot add this node
  if(index != KEY_DNE){
    delete newNode;
    newNode = nullptr;
    return false;
  }
  //if the key does NOT exist, you can add this node
  Node<K,V>* curr = _head;
  Node<K,V>* prev = nullptr;
  while(curr->_key < key){
    prev = curr;
    curr = curr->_next;
  }
  newNode->_next = curr;
  prev->_next = newNode;
  curr = nullptr;
  prev = nullptr;
  _listSize++;
  calcMins();
  return true;
}

template <class K, class V>
bool RMQList<K,V>::remove(const K& key) {
  // finds index where key exists
  int index = keyExistsAt(key);
  // if the key does not exist (or index -1), we're done.
  // there's nothing to delete
  if(index == KEY_DNE){
    return false;
  }
  // if the tail node is the one we need to delete, delete it
  if(_tail->_key == key){ // REVISIT, SEEMS INEFFICIENT
    Node<K,V>* temp = _head;
    while(temp->_next != _tail){
      temp = temp->_next;      
    }
    delete _tail;
    _tail = temp;
    _tail->_next = nullptr;
    _listSize--;
    calcMins();
    return true;
  }
  // if the head node is the one we need to delete, delete it
  if(_head->_key == key){
    Node<K,V>* toBeDeleted = _head;
    _head = _head->_next;
    delete toBeDeleted;
    toBeDeleted = nullptr;
    _listSize--;
    calcMins();
    return true;
  }
  // if the node is anywhere else in the list, delete it and
  // reconnect the list
  Node<K,V>* temp = _head;
  for(int i = 0; i < index - 1; i++){
    temp = temp->_next;
  }
  Node<K,V>* connectFront = temp;
  temp = temp->_next;
  connectFront->_next = temp->_next;
  delete temp;
  temp = nullptr;
  _listSize--;
  calcMins();
  return true;
}

template <class K, class V>
bool RMQList<K,V>::update(const K& key, const V& value) {
  int index = keyExistsAt(key);
  if(index == KEY_DNE){
    return false;
  }
  Node<K,V>* temp = _head;
  for(int i = 0; i < index; i++){
    temp = temp->_next;
  }
  temp->_value = value;
  temp = nullptr;
  calcMins();
  return true;
}

template <class K, class V>
V RMQList<K,V>::query(const K& k1, const K& k2) {
  
  if(empty()){
    throw range_error("RMQList::query() - Attempted to query on an empty list.");
  }
  if( keyExistsAt(k1) == KEY_DNE || keyExistsAt(k2) == KEY_DNE){
    throw invalid_argument("RMQList::query() - Passed key(s) do not exist in the list.");
  }

  //if the keys are equal, return its value, there's no need for pre computation
  if(k1 == k2){
    Node<K,V>* temp = _head;
    while(temp->_key != k1){
      temp = temp->_next;
    }
    return temp->_value;
  }
  
  Node<K,V>* temp = _head; //master list holder
  Node<K,V>* tempDifference = _head; //list holder
  // to find difference between first and second key
  int track; //tracks the index
  //iterate to the first key
  for(track = 0; temp->_key != k1; track++){
    temp = temp->_next;
    tempDifference = tempDifference->_next;
  }
  //find the range between key 1 and key 2
  int difference = 0;
  while(tempDifference->_key != k2){
    tempDifference = tempDifference->_next;
    difference++;
  }
  difference++;
  //get the portion of values before a precomputed block
  V queryResult = temp->_value;
  while(track % _blockSize > 0 && (difference > 0)){
    if(temp->_value < queryResult){
      queryResult = temp->_value;
    }
    difference--;
    track++;
    temp = temp->_next;
  }
  //get the minumums already calculated
  while(track + _blockSize <= difference){ //keep going until there's less than
    // a block size left to iterate through
    if(_mins[track/_blockSize] < queryResult){ //if that precomputed value is less
      queryResult = _mins[track/_blockSize]; //set it to the result
    }
    difference -= _blockSize;
    track += _blockSize;
    for(int i = 0; i < _blockSize; i++){
      temp = temp->_next;
    }
  }
  //get the rest of the mimimums not in blocks
  while(difference > 0){
    if(temp->_value < queryResult){
      queryResult = temp->_value;
    }
    difference--;
    temp = temp->_next;
  }
  
  temp = nullptr;
  tempDifference = nullptr;
  return queryResult;
}

template <class K, class V>
void RMQList<K,V>::dumpList() const {
  Node<K,V>* temp = _head;
  while(temp != nullptr){
    cout << *temp << endl;
    temp = temp->_next;
  }
  temp = nullptr;
}

template <class K, class V>
void RMQList<K,V>::dumpTable() const {  
  cout << "Table dump:" << endl;
  cout << "  Amount of blocks: " << _blockAmount << endl;
  cout << "  Single block size: " << _blockSize << endl;
  cout << endl;
  for(int i = 0; i < _blockSize; i++){
    cout << "  Minimum of block " << i + 1 << ": " << _mins[i] << endl;
  }
}

template <class K, class V>
void RMQList<K,V>::clear() {
  if(_listSize == 1){
    delete _head;
    _head = nullptr;
    _tail = nullptr;
  }else{
    deleteList(_head);
  }
  this->_head = nullptr;
  this->_tail = _head;
  this->_listSize = 0;
  if(_mins != nullptr){
    delete[] _mins;
    _mins = nullptr;
  }
}

//private helper functions created by me
template <class K, class V>
void RMQList<K,V>::deleteList(Node<K,V>* currentNode){  
  if(currentNode != _tail){
    deleteList(currentNode->_next);
  }
  delete currentNode;
  currentNode = nullptr;
}

template <class K, class V>
int RMQList<K,V>::keyExistsAt(const K& key){
  Node<K,V>* temp = _head;
  int index = 0;
  while(temp != nullptr){
    if(temp->_key == key){
      temp = nullptr;
      return index;
    }
    index++;
    temp = temp->_next;
  }
  temp = nullptr;
  return KEY_DNE;
}

// the precomputation function
template <class K, class V>
void RMQList<K,V>::calcMins() {
  if(empty()){
    throw range_error("calcMins() - Attempted to calculate minimums on an empty list.");
  }
  
  if(_mins != nullptr){
    delete[] _mins;
  }

  int blockIndex = -1;
  this->_blockSize = int(sqrt(float(_listSize)));
  if(float(sqrt(_listSize)) > _blockSize){
    this->_blockSize++;
  }
  this->_mins = new V[_blockSize];
  
  _blockAmount = 0;
  for(int i = 0; i < _listSize; i++){
    if(i % _blockSize == 0){
      _blockAmount++;
    }
  }

  Node<K,V>* temp = _head;
  for(int i = 0; i < _listSize; i++){
    if(i % _blockAmount == 0){
      blockIndex++;
      _mins[blockIndex] = temp->_value;
    }
    if(temp->_value < _mins[blockIndex]){
      _mins[blockIndex] = temp->_value;
    }
    temp = temp->_next;
  }
  temp = nullptr;
}


#endif
