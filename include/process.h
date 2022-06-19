#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp
  bool operator>(Process const& a) const;
  // Process(int pid) : pid_(pid) {
  //   cpu_usage_ = 0.0;
  // }
  void setPid(const int& pid) { pid_ = pid; }

 private:
  int pid_;
  float cpu_usage_;
};

#endif