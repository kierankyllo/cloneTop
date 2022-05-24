#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;


//Return a float of the processor utilization
float Processor::Utilization() { 
    
    //fetch the utilization vector
    vector<long> cpuCurrValues = LinuxParser::CpuUtilization();
    
    //init vars
    double currActive, currIdle, deltaTotal, deltaIdle;
    float currUtil;

    //fetch current active
    currActive = LinuxParser::ActiveJiffies();

    //fetch current idle 
    currIdle = LinuxParser::IdleJiffies();

    //calculate delta values from previously stored private member values
    deltaTotal = currActive - prevTotal;
    deltaIdle = currIdle - prevIdle;

    //update stored private member values with current values
    prevTotal = currActive;
    prevIdle = currIdle;

    //calculate the utilization percentage from the deltas
    currUtil = (deltaTotal - deltaIdle)/deltaTotal;

return currUtil; 
}