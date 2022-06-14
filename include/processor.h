#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // DONE: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    long int m_idletime = 0;
    long int m_totaltime = 0;

};

#endif