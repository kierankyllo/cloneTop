#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
  public:
    Process(const int&);
    int Pid();                               
    std::string User();                      
    std::string Command();                   
    float CpuUtilization();                  
    std::string Ram();                       
    long int UpTime();                       
    bool operator<(Process const& a) const;  

  private:
    int pid_;
    std::string user_;
    std::string command_;
    std::string ram_;
    long uptime_;
    float util_;
    };

#endif