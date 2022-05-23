#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();  

 private:
 
    long prevTotal {0};
    long prevIdle {0};
};

#endif