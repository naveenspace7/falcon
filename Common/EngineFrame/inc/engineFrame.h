#ifndef ENGFRAME
#define ENGFRAME

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <chrono>
#include <vector>
#include <iostream>

class engineFrame
{
 private:
  int state;
  int id;
  const int Lock = 0;
  std::chrono::high_resolution_clock::time_point start, end, start_old;
  //std::chrono::high_resolution_clock::time_point ;
  std::string engine_name;
  std::vector<int> vec_time;
 protected:
  int*base;
 public:
  engineFrame(std::string,int);
  void compute_start();
  void compute_end();
  void print_duration();
  void setLock(int);
  int getLock();
  ~engineFrame();
  int getState();
  void setState(int);
  int average_time(std::vector<int>);
  //void compute();
  //void on_exit(); // TODO: make the PID -1  
};

int* get_base();

#endif
