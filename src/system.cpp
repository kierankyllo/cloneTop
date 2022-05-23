#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a vector of processes sorted by CPU utilization 
vector<Process>& System::Processes() {
    //declare empty vector of process objects
    vector<Process> processVector{};
    //fetch a vector of the current active pricess ids
    vector<int> processIds = LinuxParser::Pids();
    //for each process id construct a process object and push it into the process vector
    for (auto i : processIds) {
        Process temp(i);
        processVector.push_back(temp);
    }
    //sort process objects by their CPU utilization
    std::sort( processVector.begin(), processVector.end());

//update the private member variable and return it 
processes_ = processVector;
return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }