#include <iostream>
#include "Graph.h"

using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;

// Constructor - DO NOT MODIFY
Graph::Graph(int n) {
  if (n <= 0)
    throw invalid_argument("Graph::Graph(): number of vertices must be positive");

  _rows = new EntryList*[n];
  for (int i = 0; i < n; i++) {
    _rows[i] = new EntryList();
  }
  _numVert = n;
  _numEdge = 0;
}

Graph::Graph(const Graph& G) {

  this->_numVert = G._numVert;
  this->_numEdge = G._numEdge;
  
  this->_rows = new EntryList*[_numVert];
  for(int i = 0; i < _numVert; i++){
    _rows[i] = new EntryList(*G._rows[i]);
  }
  
}

const Graph& Graph::operator=(const Graph& rhs) {

  if(this != &rhs){

    for(int i = 0; i < _numVert; i++){
      delete _rows[i];
      _rows[i] = nullptr;
    }
    delete[] _rows;
    _rows = nullptr;
    
    this->_numVert = rhs._numVert;
    this->_numEdge = rhs._numEdge;

    this->_rows = new EntryList*[_numVert];
    for(int i = 0; i < _numVert; i++){
      _rows[i] = new EntryList(*rhs._rows[i]);
    }
  }
  return *this;
}

Graph::~Graph() {
  for(int i = 0; i < _numVert; i++){
    delete _rows[i];
    _rows[i] = nullptr;
  }
  delete[] _rows;
  _rows = nullptr;
}

// Number of vertices - DO NOT MODIFY
int Graph::numVert() const {
  return _numVert;
}

// Number of edges - DO NOT MODIFY
int Graph::numEdge() const {
  return _numEdge;
}

void Graph::addEdge(int u, int v, weight_t x) {
  if((u >= _numVert) || (v >= _numVert) ||
     (u < 0) || (v < 0)){
    throw invalid_argument("Graph::addEdge() attempted to add edge with an invalid vertex.");
  }
  EntryList::Entry e(v, x);
  _rows[u]->insert(e);
  _numEdge++;
}

bool Graph::removeEdge(int u, int v) {
  if((u >= _numVert) || (v >= _numVert) ||
     (u < 0) | (v < 0)){
    throw invalid_argument("Graph::removeEdge() - attempted to remove from edge that is not valid.");
  }
  EntryList::Entry removed;
  if(_rows[u]->remove(v, removed)){
    _numEdge--;
    return true;
  }
  return false;
}


// Dump the graph - DO NOT MODIFY
void Graph::dump() const {

  cout << "Dump of graph (numVert = " << _numVert
       << ", numEdge = " << _numEdge << ")" << endl;

  for (int i = 0; i < _numVert; i++) {
    cout << "Row " << i << ": \n";
    _rows[i]->dump();
  }
}


Graph::EgIterator::EgIterator(Graph *Gptr, bool enditr) {
  if(Gptr != nullptr){
    this->_Gptr = Gptr;
  }else{
    this->_Gptr = new Graph(Gptr->numVert());
  }
  if(enditr){
    _itr = Gptr->_rows[_row]->end();
  }else{
    _row = 0;
   _itr = Gptr->_rows[_row]->begin();
  }  
}
  
tuple<int,int,weight_t> Graph::EgIterator::operator*() {
  tuple<int, int, weight_t> entryTuple(_row, (*_itr).getVertex(),
				       (*_itr).getWeight());
  return entryTuple;
}

bool Graph::EgIterator::operator!=(const EgIterator& rhs) {
  if(this != &rhs){
    return true;
  }
  return false;
}

void Graph::EgIterator::operator++(int dummy) {
  /*  using graph, initialize to first row, iterate through that
  ** row using EL iterator, then keep doing that until no more left
   */
  for(int i = 0; i < _Gptr->numEdge(); i++){
    Graph::NbIterator nbit(_Gptr, i, false);
    for(nbit = _Gptr->nbBegin(i); nbit != _Gptr->nbEnd(i); nbit++){
      
    }
  }
}

Graph::EgIterator Graph::egBegin() {
  return EgIterator(this, false);
}
 
Graph::EgIterator Graph::egEnd(){
  return EgIterator(this, true);
}
 
Graph::NbIterator::NbIterator(Graph *Gptr, int v, bool enditr) {
  if((v < 0) || (v >= Gptr->numVert())){
    throw invalid_argument("Graph::NbIterator::NbIterator: invalid vertex");
  }
  if(Gptr == nullptr){
    throw invalid_argument("Graph::NbIterator::NbIterator: Host graph points to a null pointer.");
  }  
  this->_Gptr = Gptr; 
  this->_row = v;
  if(enditr){
    _itr = Gptr->_rows[_row]->end();
  }else{
    _itr = Gptr->_rows[_row]->begin();
  }
}

bool Graph::NbIterator::operator!=(const NbIterator& rhs) {
  if(this != &rhs){
    return true;
  }
  return false;
}

void Graph::NbIterator::operator++(int dummy) {
  if(_itr == _Gptr->_rows[_row]->end()){
    *this = _Gptr->nbEnd(_row);
  }else{
    _itr++;
  }
}

pair<int,weight_t> Graph::NbIterator::operator*() {
  pair<int, weight_t> newPair((*_itr).getVertex(),
			      (*_itr).getWeight());
  return newPair;
}

Graph::NbIterator Graph::nbBegin(int v) {
  return NbIterator(this, v, false);
}

Graph::NbIterator Graph::nbEnd(int v) {
  return NbIterator(this, v, true);
}
