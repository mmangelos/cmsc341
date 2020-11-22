#include "EntryList.h"

using std::cout;
using std::endl;
using std::range_error;

// Constructor - DO NOT MODIFY
EntryList::EntryList() {
  _array = new Entry[DEFAULT_SIZE];
  _capacity = DEFAULT_SIZE;
  _size = 0;
}

EntryList::EntryList(const EntryList& rhs) {
  this->_capacity = rhs._capacity;
  this->_size = rhs._size;
  this->_array = new Entry[_capacity];
  if(this->_size != 0){    
    for(int i = 0; i < this->_size; i++){
      Entry newEntry(rhs._array[i].getVertex(),
		     rhs._array[i].getWeight());
      this->_array[i] = newEntry;
    }
  }
}

const EntryList& EntryList::operator=(const EntryList& rhs) {

  if(this != &rhs){

    delete[] _array;
    this->_capacity = rhs._capacity;
    this->_size = rhs._size;
    _array = new Entry[_capacity];
    if(this->_size != 0){
      for(int i = 0; i < this->_size; i++){
	Entry newEntry(rhs._array[i].getVertex(),
		       rhs._array[i].getWeight());
	this->_array[i] = newEntry;
      }
    }
  }
  return *this;
}
  
EntryList::~EntryList() {

  delete[] _array;
  _array = nullptr;
}

//private helper functions
void EntryList::resizeExpand() {
  _capacity *= 2;
  Entry* tempArray = new Entry[_capacity];
  for(int i = 0; i < _size; i++){
    tempArray[i] = _array[i];
  }
  delete[] _array;
  _array = tempArray;
}

/* You would resize and contract only if
** the size of the array is 1/4 of the capacity
*/
void EntryList::resizeContract() {
  if(_capacity / 2 < DEFAULT_SIZE){
    _capacity = DEFAULT_SIZE;
  }else{
    _capacity /= 2;
  }
  Entry* tempArray = new Entry[_capacity];
  for(int i = 0; i < _size; i++){
    tempArray[i] = _array[i];
  }
  delete[] _array;
  _array = tempArray;
}

void EntryList::orderEntryList(){
  for(int i = 0; i < _size; i++){
    for(int j = i; j < _size; j++){
      if(_array[i].getVertex() > _array[j].getVertex()){
	Entry temp = _array[i];
	_array[i] = _array[j];
	_array[j] = temp;
      }
    }
  }
}

bool EntryList::insert(const Entry& e) {
  
  for(int i = 0; i < _size; i++){
    if(_array[i].getVertex() == e.getVertex()){
      return false;
    }
  }
  if(_size == _capacity){
    resizeExpand();    
  }
  _array[_size] = e;
  _size++;
  orderEntryList();
  return true;
}
  
bool EntryList::update(const Entry& e) {
  for(int i = 0; i < _size; i++){
    if(_array[i].getVertex() == e.getVertex()){
      _array[i].setWeight(e.getWeight());
      return true;
    }
  }
  return false;
}

bool EntryList::getEntry(int vertex, Entry &ret) {
  for(int i = 0; i < _size; i++){
    if(_array[i].getVertex() == vertex){
      ret = _array[i];
      return true;
    }
  }
  return false;
}

bool EntryList::remove(int vertex, Entry &ret) {
  for(int i = 0; i < _size; i++){
    if(_array[i].getVertex() == vertex){
      ret = _array[i];
      for(int j = i; j < _size; j++){
	_array[j] = _array[j+1];
      }
      if(_size <= _capacity / 4){
	resizeContract();
      }
      --_size;
      return true;
    }
  }
  return false;
}

EntryList::Entry& EntryList::at(int indx) const {
  if( (indx >= _size) || (indx < 0) ){
    throw range_error("EntryList::at(int) - you are out of bounds.");
  }
  return _array[indx];
}

// dump data structure - DO NOT MODIFY
void EntryList::dump() {
  for (int i = 0; i < _size; i++) {
    cout << "  " << _array[i] << endl;
  }
}

EntryList::Iterator::Iterator(EntryList *EList, int indx) {
  if(EList == nullptr){
    _ELptr = new EntryList();
  }else{
    _ELptr = EList;
  }
  _indx = indx;
}

bool EntryList::Iterator::operator!=(const EntryList::Iterator& rhs) {
  if(this != &rhs){
    return true;
  }
  return false;
}

bool EntryList::Iterator::operator==(const EntryList::Iterator& rhs) {
  if(this == &rhs){
    return true;
  }
  return false;
}

void EntryList::Iterator::operator++(int dummy) {
  if(_indx < _ELptr->size()){
    _indx++;
  }
}

EntryList::Entry EntryList::Iterator::operator*() {
  return _ELptr->at(_indx);
}

EntryList::Iterator EntryList::begin() {
  return Iterator(this, 0);
}

EntryList::Iterator EntryList::end() {
  return Iterator(this, _size - 1);
}

// Insertion operator for Entry objects - DO NOT MODIFY
ostream& operator<<(ostream& sout, const EntryList::Entry& e) {
  sout << e._vertex << ": " << e._weight;
  return sout;
}

// A convenient way to write test code for a single class is to write
// a main() function at the bottom of the .cpp file.  Just be sure to
// comment-out main() once you are done testing!

// Following is example test code.  There is no guarantee that it is
// complete -- you are responsbile for thoroughly testing your code.
// In particular, passing these tests does not mean your
// implementation will pass all grading tests.

/*
int main() {
  EntryList el;
  EntryList::Entry e;
  
  cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
  el.dump();
  cout << endl;
  
  for (int i = 1; i < 13; i++) {
    el.insert( EntryList::Entry((i*5)%13, i) );
  }
  
  cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
  el.dump();
  cout << endl;
  
  for (int i = 1; i < 13; i+=2) {
     el.remove(i, e);
  }
  
  cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
  el.dump();
  cout << endl;
  
  
  for (int i = 2; i < 13; i+=2) {
    el.update( EntryList::Entry(i, 2*i) );
  }
  
  cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
  el.dump();
  cout << endl;
  
  for (int i = 3; i < 13; i*=2) {
    el.remove(i, e);
  }
  
  cout << "size: " << el.size() << ", capacity: " << el.capacity() << endl;
  el.dump();
  cout << endl;
  
  cout << "\nPrint using iterator:\n";
  for (auto itr = el.begin(); itr != el.end(); itr++) {
    cout << *itr << endl;
  }
  
  return 0;
}

*/
