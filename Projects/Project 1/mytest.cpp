/*
** mytest.cpp
** Mitchell Angelos (CV55655)
** a242@umbc.edu
** Project 1
** 2/16/20
*/

#include <iostream>
#include "EntryList.h"
#include "Graph.h"

void entryListTests(){
  EntryList e;

  EntryList::Entry ent(0, 0.9);
  e.insert(ent);
  EntryList::Entry entry(3, 3.3);
  e.insert(entry);
  EntryList::Entry entTwo(2, 2.2);
  e.insert(entTwo);
  EntryList::Entry entThree(1, 1.1);
  e.insert(entThree); //add three entries
  EntryList::Entry newTwo(2, 2.4);
  e.update(newTwo); //update
  EntryList::Entry fakeFive(5, 5.5);
  e.update(fakeFive);
  EntryList::Entry removed;
  e.remove(1, removed); //remove one
  //std::cout << removed << std::endl;
  e.dump();

  std::cout << std::endl;
  e.insert(removed);
  EntryList::Entry getEntry;
  e.getEntry(1, getEntry);
  e.dump();
  std::cout << "Get entry (should be vertex 1)- " << getEntry << std::endl;
  std::cout << ".at(2)- " << e.at(2) << std::endl;
  e.at(2).setWeight(2.2);
  std::cout << ".at(2).setWeight(2.2)- " << e.at(2) << std::endl;
  EntryList::Entry alreadyOne(1, 1.2);
  e.insert(alreadyOne);
  EntryList::Entry realFive(5, 5.5);
  e.insert(realFive);
  e.dump();

  //copy constructor
  EntryList copyFrom;
  for(int i = 0; i < 3; i++){
    EntryList::Entry entryCC(i, float(i + (i/10.0)));
    copyFrom.insert(entryCC);
  }
  std::cout << "Copy constructor - copy from:" << std::endl;
  copyFrom.dump();
  std::cout << "Copy constructor - copy to (second entry modified):" << std::endl;
  EntryList copyTo(copyFrom);
  copyTo.at(1).setWeight(80.9);
  copyTo.dump();
  std::cout << "Copy constructor - copy from - check for deep copy:" << std::endl;
  copyFrom.dump();

  //assignment operator
  EntryList copyToAssignment;
  EntryList::Entry abc(1, 2.3);
  copyToAssignment.insert(abc);
  copyToAssignment = copyFrom;
  std::cout << "Assignment operator - copied from copyFrom (second entry modified):" << std::endl;
  copyToAssignment.at(1).setWeight(56.7);
  copyToAssignment.dump();
  std::cout << "Copy from - check if still deep copy:" << std::endl;
  copyFrom.dump();
  copyToAssignment = copyToAssignment; //protect from self assignment

  //all functions returning a bool return the correct value
  EntryList boolList;
  EntryList::Entry a(0, 0.0);
  EntryList::Entry b(0, 1.1);
  EntryList::Entry removedBL;
  //insert new vertex, insert with already existing vertex
  std::cout << std::boolalpha << ".insert(0, 0.0): " << boolList.insert(a) << std::endl;
  std::cout << ".insert(0, 1.1): " << boolList.insert(b) << std::endl;
  //remove vertex 0, attempt to remove vertex 0 that's not in the list
  std::cout << ".remove(0, removedBL): " << boolList.remove(0, removedBL) << std::endl;
  std::cout << ".remove(0, removedBL): " << boolList.remove(0, removedBL) << std::endl;
  boolList.insert(a);
  EntryList::Entry c(1, 1.1);
  boolList.insert(c);
  //update entry 1, update entry 2 which does not exist
  EntryList::Entry d(0, 0.3);
  std::cout << ".update(d) - vertex 0, weight 0.3: " << boolList.update(d) << std::endl;
  EntryList::Entry f(4, 0.0);
  std::cout << ".update(f) - vertex 4, weight 0.0: " << boolList.update(f) << std::endl;
  //get entry, vertex 0, get entry, vertex 4 that does not exist
  std::cout << ".getEntry(0, removedBL): " << boolList.getEntry(0, removedBL) << std::endl;
  std::cout << ".getEntry(4, removedBL): " << boolList.getEntry(4, removedBL) << std::endl;
  
  
  //.at range_error, commented out due to termination when checked
  //std::cout << e.at(20) << std::endl;
  //std::cout << e.at(-1) << std::endl;
  
  //expands correctly
  EntryList expandList;
  for(int i = 0; i < 10; i++){
    EntryList::Entry expandL(i, float(i));
    expandList.insert(expandL);
  }
  std::cout << "Expand list capacity before expansion: " << expandList.capacity() << std::endl;
  EntryList::Entry expandL2(11, 121.0);
  expandList.insert(expandL2);
  std::cout << "Expand list capacity after expansion: " << expandList.capacity() << std::endl;
  
  //contracts correctly
  EntryList contractList;
  for(int i = 0; i < 11; i++){
    EntryList::Entry contractL(i, float(i));
    contractList.insert(contractL);
  }
  std::cout << "Contract list capacity before contraction: " << contractList.capacity() << std::endl;
  for(int i = 0; i < 7; i++){
    contractList.remove(i, removedBL);
  }
  std::cout << "Contract list capacity after contraction: " << contractList.capacity() << std::endl;

  //iterator
  EntryList::Iterator itr;
  std::cout << "Forward iterator:" << std::endl;
  for(itr = e.begin(); itr != e.end(); itr++){
    std::cout << *itr << std::endl;
  } 
  //iterator on empty EntryList
  EntryList dummyList;
  EntryList::Iterator emptyItr;
  for(emptyItr = dummyList.begin(); emptyItr != dummyList.end(); emptyItr++){
    std::cout << "You should not see this." << std::endl;
  }
  //begin() returns first element of non-empty list
  std::cout << "*e.begin() (should be 0: 0.9):  " << *e.begin() << std::endl;
  //begin() != end() unless the EntryList is empty
  std::cout << "e.begin() != e.end(), should be true" << std::endl;
  std::cout << (e.begin() != e.end()) << std::endl; //should be true, not equal
  std::cout << "dummyList.begin() != dummyList.end(), should be false" << std::endl;
  std::cout << (dummyList.begin() != dummyList.end()) << std::endl; //should be false, empty list

}

void graphTests(){

  //constructor throwing invalid_argument if parameter n is < 0
  //Graph g(-1); //commented out due to termination at runtime
  Graph g(4);
  g.addEdge(0, 1, 1.1);
  g.addEdge(1, 2, 2.2);
  g.addEdge(2, 3, 3.3);
  g.removeEdge(1, 2);
  std::cout << "Graph dump:" << std::endl;
  g.dump();

  //throw invalid_argument for invalid vertex
  //g.addEdge(4, 5, 5.5);
  //g.removeEdge(5, 1);

  //copy constructor
  Graph copyTo(g);
  copyTo.removeEdge(0, 1);
  std::cout << "Copy constructor dump, with row 0's entry removed:" << std::endl;
  copyTo.dump();
  std::cout << "Copied from dump, to check if deep copy:" << std::endl;
  g.dump();

  //assignment operator
  Graph empty(4);
  copyTo = empty;
  std::cout << "copyTo dump, used assignment operator, should be empty:" << std::endl;
  copyTo.dump();
  copyTo.addEdge(0, 0, 0.0);
  copyTo.addEdge(1, 1, 1.1);
  copyTo.addEdge(1, 2, 3.4);
  //copyTo.addEdge(4, 4, 4.4); //test for invalid vertex
  copyTo.addEdge(2, 2, 2.2);
  empty = copyTo;
  empty.removeEdge(0, 0);
  std::cout << "Assignment operator on empty, with edge 0 removed, deep copy check:" << std::endl;
  empty.dump();
  empty = empty; //check for self assignment protection

  // edge iterator
  Graph* ptr = new Graph(3);
  ptr->addEdge(0, 0, 0.0);
  ptr->addEdge(1, 1, 1.1);
  ptr->addEdge(1, 2, 3.4);
  ptr->addEdge(1, 0, 4.5);
  ptr->addEdge(0, 2, 699.78);
  std::cout << std::endl;
  Graph::NbIterator nbit(ptr, 1, false);
  for(nbit = ptr->nbBegin(1); nbit != ptr->nbEnd(1); nbit++){
    std::pair<int, weight_t> p = *nbit;
    std::cout << "Pair: " << p.first << "-" << p.second << std::endl;
  }
  std::cout << "NbIterator finished" << std::endl;

}

int main(){

  entryListTests();
  graphTests();
  return 0;
}
