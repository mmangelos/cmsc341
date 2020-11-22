// CMSC 341 - Spring 2020 - Project 4
// JQueue: a job queue using a skew heap and function pointers

#include "jqueue.h"

using std::cout;
using std::endl;
using std::domain_error;

JQueue::JQueue(prifn_t priFn) {
  _heap = nullptr;
  _size = 0;
  priority = priFn;
}

JQueue::~JQueue() {
  deleteHeap(_heap);
}

JQueue::JQueue(const JQueue& rhs) {
  this->_heap = buildHeap(rhs._heap);
  this->_size = rhs.numJobs();
  this->priority = rhs.getPriorityFn();
}

JQueue& JQueue::operator=(const JQueue& rhs) {

  if(this != &rhs){
    deleteHeap(_heap);
    this->_heap = buildHeap(rhs._heap);
    this->_size = rhs.numJobs();
    this->priority = rhs.getPriorityFn();
  }
  return *this;
}

void JQueue::insertJob(const job_t& input) {
  if(_size == 0){
    _heap = new Node(input);
    _size++;
  }else{    
    JQueue temp(priority);
    temp.insertJob(input);
    mergeWithQueue(temp);
  }
}

job_t JQueue::getNextJob() {
  if(_size == 0){
    throw domain_error("Attempted to get next job on an empty JQueue.");
  }
  job_t j = _heap->_job;
  Node* newHeap = recursiveMerge(_heap->_left, _heap->_right);
  delete _heap;
  _heap = newHeap;
  _size--;
  newHeap = nullptr;
  return j;
}

void JQueue::mergeWithQueue(JQueue& rhs) {
  // If the two JQueues have different priority functions,
  // it will throw a domain_error.
  if(priority != rhs.getPriorityFn()){
    throw domain_error("Attempted to merge two JQueues with different priority functions.");
  }
  _size += rhs.numJobs();
  while(rhs.numJobs() > 0){    
    _heap = recursiveMerge(_heap, new Node(rhs.getNextJob()));
  }
}

// recursiveMerge is a helper function to mergeWithQueue.
// Its purpose is to the bulk of the recursive work
// in merging.
Node* JQueue::recursiveMerge(Node* p1, Node* p2){
  // if p1 has nothing to merge, return p2 to merge
  if(p1 == nullptr){
    return p2; // base case 1
  }
  // if p2 has nothing to merge, return p1 to merge
  if(p2 == nullptr){
    return p1; // base case 2
  }
  // swap the nodes if the priority of p1 is lower than p2's.
  if(priority(p1->_job) <= priority(p2->_job)){
    swapNodes(p1, p2); // helper to swap the nodes
  }
  swapSubtrees(p1); // swap the subtrees of p1
  p1->_left = recursiveMerge(p2, p1->_left); // recursive call
  return p1;
}

// swapNodes takes two nodes and swaps the memory locations
// associated with them.
void JQueue::swapNodes(Node*& p1, Node*& p2){
  Node* temp = p1;
  p1 = p2;
  p2 = temp;
  temp = nullptr;
}

// swapSubtrees takes a node and swaps the left subtree to
// the right and vice versa.
void JQueue::swapSubtrees(Node*& toSwap){
  Node* tempRight = toSwap->_right;
  toSwap->_right = toSwap->_left;
  toSwap->_left = tempRight;
  tempRight = nullptr;
}

void JQueue::clear() {
  deleteHeap(_heap);
  _size = 0;
}

int JQueue::numJobs() const {
  return _size;
}

void JQueue::printJobQueue() const {
  printHelper(_heap);
}

prifn_t JQueue::getPriorityFn() const {
  return priority;
}

void JQueue::setPriorityFn(prifn_t priFn) {
  priority = priFn;
  int newSize = _size;
  Node* newHeap = nullptr;
  while(_heap != nullptr){
    newHeap = recursiveMerge(newHeap, new Node(getNextJob()));
  }
  _size = newSize;
  _heap = newHeap;
}

void JQueue::dump() const {
  dumpHelper(_heap);
}

// printHelper uses preorder traversal 
// to print the queue
void JQueue::printHelper(Node* currentNode) const {
  if(currentNode == nullptr){
    return;
  }
  cout << "[" << priority(currentNode->_job)
       << "] " << *currentNode << endl;
  printHelper(currentNode->_left);
  printHelper(currentNode->_right);
}

// dumpHelper uses preorder traversal to print the
// queue with the structure intact.
void JQueue::dumpHelper(Node* currentNode) const {
  if(currentNode == nullptr){
    return;
  }
  cout << "(";
  dumpHelper(currentNode->_left);
  cout << priority(currentNode->_job);
  dumpHelper(currentNode->_right);
  cout << ")";
}

// deleteHeap recursively goes through the entire heap
// and deletes each node to empty the heap entirely.
// This is mainly used for the destructor.
void JQueue::deleteHeap(Node* currentNode){
  if(currentNode != nullptr){
    deleteHeap(currentNode->_left);
    deleteHeap(currentNode->_right);
    delete currentNode;
    currentNode = nullptr;
  }
}

// buildHeap takes another heap and makes a deep copy
// of it.
// This is mainly used for the copy constructor and the
// overloaded assignment operator.
Node* JQueue::buildHeap(Node* currentNode){
  if(currentNode == nullptr){
    return nullptr;
  }
  Node* newNode = new Node(currentNode->_job);
  newNode->_left = buildHeap(currentNode->_left);
  newNode->_right = buildHeap(currentNode->_right);
  return newNode;
}

ostream& operator<<(ostream& sout, const job_t& job) {
  sout << "Job: " << job._name << ", pri: " << job._priority
       << ", u: " << job._user << ", g: " << job._group << ", proc: "
       << job._proc << ", mem: " << job._mem << ", time: " << job._time;
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node._job;
  return sout;
}

