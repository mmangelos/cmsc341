/* 
** Test for RMQList
** Mitchell Angelos [CV55655]
*/

#include <iostream>
#include "rmqlist.h"
#include <ctime>
#include <cstdlib>
using namespace std;

int main(){

  srand(time(NULL));
  
  //BASIC TESTS
  //constructor
  RMQList<int, int> rmq_one;
  rmq_one.insert(1, 2);
  rmq_one.insert(1, 3);
  rmq_one.insert(3, 4);
  rmq_one.insert(2, 3);
  rmq_one.insert(5, 12);
  rmq_one.dumpList();

  //copy constructor
  cout << endl;
  cout << "Copy constructor:" << endl;
  RMQList<int, int> rmq_one_cc(rmq_one);
  rmq_one_cc.remove(3); //test for deep copy
  rmq_one_cc.dumpList();
  cout << "Test for deep copy, print original:" << endl;
  rmq_one.dumpList();

  //overloaded assignment operator
  cout << endl;
  cout << "Overloaded assignment operator:" << endl;
  //also tests for clear()
  RMQList<int, int> rmq_one_ao = rmq_one;
  rmq_one_ao = rmq_one_ao; //protect against self assignment test
  rmq_one_ao.remove(1); //test for deep copy and removal at head
  rmq_one_ao.remove(5); //test for removal at tail
  rmq_one_ao.dumpList();
  cout << "Test for deep copy, print original:" << endl;
  rmq_one.dumpList();

  //update
  cout << endl;
  cout << "Updated head, tail and a middle node" << endl
       << "Key 1 -> 5, key 3 -> 10, key 5 -> 15" << endl;
  rmq_one.update(1, 5);
  rmq_one.update(3, 10);
  rmq_one.update(5, 15);
  rmq_one.dumpList();

  /* small list test
  ** for list of size 100
  */
  const int SMALL_LIST_SIZE = 103;

  RMQList<int, int> smallList;
  for(int i = 0; i < SMALL_LIST_SIZE; i++){
    smallList.insert(i+1, int(rand() % SMALL_LIST_SIZE + 1));
  }

  cout << endl;
  cout << "Small list dump:" << endl;
  smallList.dumpList();
  cout << "Query (1,5): " << smallList.query(1, 5) << endl;
  cout << "Query (1, 20): " << smallList.query(1, 20) << endl;
  cout << "Query (34, 76): " << smallList.query(34, 76) << endl;
  cout << "Query (79, 100): " << smallList.query(79, 100) << endl;
  cout << "Query (3, 97): " << smallList.query(3, 97) << endl;
  smallList.dumpTable();

  //copy constructor query
  RMQList<int, int> copyQuery(smallList);
  cout << "Query (1,5) [SHOULD BE SAME AS ABOVE] = " << copyQuery.query(1,5) << endl;

  RMQList<int, int> aaQuery = smallList;
  cout << "Query (79, 100) [SHOULD BE SAME AS ABOVE] = " << aaQuery.query(79, 100) << endl;

  /* large list test
  ** for list of size 1000
  */

  const int LARGE_LIST_SIZE = 1000;
  
  RMQList<int, int> largeList;
  for(int i = 0; i < LARGE_LIST_SIZE; i++){
    if(rand() % 2 == 0){
      largeList.insert(i, int(rand() % LARGE_LIST_SIZE + 1));
    }else{
      largeList.insert(-i, int(rand() % LARGE_LIST_SIZE + 1));
    }
  }

  cout << endl;
  //cout << "Large list dump:" << endl;
  //largeList.dumpList();
  //cout << "Query (4, 112): " << largeList.query(4, 112) << endl;
  //cout << "Query (104, 674): " << largeList.query(104, 674) << endl;
  

  /* massive list test
  ** for list of size 10000
  */

  const int HUGE_LIST_SIZE = 10000;  

  RMQList<int, int> hugeList;
  for(int i = 0; i < HUGE_LIST_SIZE; i++){
    if(rand() % 2 == 0){
      hugeList.insert(i, int(rand() % HUGE_LIST_SIZE + 1));
    }else{
      hugeList.insert(-i, int(rand() % HUGE_LIST_SIZE + 1));
    }
  }

  cout << endl;
  //cout << "Massive list dump:" << endl;
  //hugeList.dumpList();
  //cout << "Query (3, 5665): " << hugeList.query(3, 5565) << endl;
  //cout << "Query (1004, 9997): " << hugeList.query(1004, 9997) << endl;
  

  return 0;
}
