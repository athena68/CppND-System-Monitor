#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp

 private:
    int cpuid_;
    long int m_idletime = 0;
    long int m_totaltime = 0;

};

#endif