/* Project 4 Test file
** CMSC 341 - Professor Wilson
** Mitchell Angelos (CV55655)
*/

#include <iostream>
#include <string>
#include "jqueue.h"

int samplePriority1(const job_t& job);
int samplePriority2(const job_t& job);

int main() {

  // create a JQueue object
  JQueue jq1(samplePriority1);

  // create and add 5 jobs
  job_t job1("First job", 20, 5, 11, 32, 2048, 1600);
  job_t job2("Second job",40, 2, 15, 64, 1024, 1200);
  job_t job3("Third job", 30, 4, 25, 128, 512, 2400);
  job_t job4("Fourth job",50, 1, 10, 240, 16, 5400);
  job_t job5("Fifth job", 10, 3, 5, 254, 64, 120);

  jq1.insertJob(job1);
  jq1.insertJob(job2);
  jq1.insertJob(job3);
  jq1.insertJob(job4);
  jq1.insertJob(job5);

  // print the queue
  std::cout << std::endl;
  std::cout << "Queue size: " << jq1.numJobs() << std::endl;
  jq1.printJobQueue();
  jq1.dump();
  std::cout << std::endl;

  // test setting new priority function and rebuilding the heap
  std::cout << "\nNew priority function queue:\n";
  jq1.setPriorityFn(samplePriority2);
  jq1.printJobQueue();
  jq1.dump();
  std::cout << std::endl;

  // test overloaded copy constructor
  JQueue jqCC(jq1);

  // test getting the next job and extracting from the queue
  job_t copyTop = jqCC.getNextJob();
  std::cout << "Extracted job: " << copyTop << std::endl;
  std::cout << "Regular queue size: " << jq1.numJobs() << std::endl;
  std::cout << "Copy Constructor queue size: " << jqCC.numJobs() << std::endl;
  std::cout << "Copy constructor print job queue: " << std::endl;
  jqCC.printJobQueue();
  jqCC.dump();
  std::cout << std::endl;

  // test overloaded assignment operator
  JQueue jqOAO(jqCC.getPriorityFn());
  jqOAO.insertJob(job1);
  jqOAO.insertJob(job2);
  std::cout << "Testing overloaded assignment operator deleting existing nodes in heap" << std::endl;
  std::cout << "Should see no memory leaks." << std::endl;
  jqOAO = jq1;

  // take a job out
  job_t copyTopOAO = jqOAO.getNextJob();
  std::cout << "Extracted job: " << copyTopOAO << std::endl;
  std::cout << "Regular queue size: " << jq1.numJobs() << std::endl;
  std::cout << "Overloaded assignment operator queue size: " << jqOAO.numJobs() << std::endl;
  std::cout << "OAO print job queue: " << std::endl;
  jqOAO.printJobQueue();
  jqOAO.dump();
  std::cout << std::endl;
  std::cout << std::endl;

  // make a new heap to merge with another
  JQueue toMerge(samplePriority2);
  
  job_t job6("Sixth job", 80, 40, 76, 192, 8192, 100000);
  job_t job7("Seventh job", 90, 78, 22, 233, 4096, 15000);
  job_t job8("Eighth job", 70, 11, 67, 111, 16384, 127611);
  job_t job9("Ninth job", 45, 12, 13, 90, 32768, 172799);

  toMerge.insertJob(job6);
  toMerge.insertJob(job7);
  toMerge.insertJob(job8);
  toMerge.insertJob(job9);

  // merge the two queues together, print and dump the new queues
  std::cout << "Merge toMerge with jqOAO: " << std::endl;
  toMerge.mergeWithQueue(jqOAO);
  toMerge.printJobQueue();
  toMerge.dump();
  std::cout << std::endl;

  // extract all jobs in said queue, make sure it remains empty
  std::cout << "Taking out all jobs in the queue." << std::endl;
  while(toMerge.numJobs() > 0){
    std::cout << toMerge.getNextJob() << std::endl;
  }
  std::cout << "Number of jobs: " << toMerge.numJobs() << std::endl;
  
  return 0;
}

int samplePriority1(const job_t &job) {
  float pri;

  // 100 (regular user) + 100 (regular group) + job priority
  pri = 200 + job._priority;

  // If special user (7 or 23) add 200
  if (job._user == 7 || job._user == 23) pri += 200;

  // If special group (0 or 11) add 100
  if (job._group == 0 || job._group == 11) pri += 100;

  // Subtract weighted proc, mem, and time
  //   1.0 * proc, 0.01 * mem, (1/3600) * time
  pri = pri - job._proc - 0.01*job._mem - (1.0/3600.0)*job._time;

  return int(pri);
}

int samplePriority2(const job_t &job) {
  return int(job._time);
}
