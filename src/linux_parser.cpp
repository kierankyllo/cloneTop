#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


//Exception handling for conversion from string to long
long LinuxParser::SafeStol(const string& input){

  try {
    return std::stol(input);
  } 
  catch (const std::invalid_argument& arg) {
  return 0;
  }
}

//Exception handler for conversion from string to int
int LinuxParser::SafeStoi(const string& input){

  try {
    return std::stoi(input);
  } 
  catch (const std::invalid_argument& arg) {
  return 0;
  }
}


//Return the OS pretty name string
string LinuxParser::OperatingSystem() {

  string line, key, value;
  //Open a well formed filestream and replace unwanted chars
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      //find the key and return the value
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }

return value;
}


//Return the kernel version from system files
string LinuxParser::Kernel() {

  string line, os, version, kernel;
  //Open a well formed filestream
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    //locate kernel version in file and return it
    linestream >> os >> version >> kernel;
  }

return kernel;
}


//Return a vector of process ids
vector<int> LinuxParser::Pids() {

  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = SafeStoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);

return pids;
}


//Return a float of the memory utilization
float LinuxParser::MemoryUtilization() { 

  vector<double> memVector;
  string line, ignore, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    //for the first four lines in the file stream the file 
    for (int i = 0; i < 3; i++){
      //get a line from the filestream
      std::getline(stream, line);
      //convert the line to a stringstream
      std::istringstream linestream(line);
      //ignore the first and then push the second symbol on the line to a vector of integers and SafeStoi() to integer
      linestream >> ignore >> value;
      memVector.push_back(SafeStoi(value));
    }
  }
  //calculate the memory utilization float and return it
  float memUtil = (memVector[0] - memVector[1])/memVector[0];

return memUtil;
}


//Return the system uptime
long LinuxParser::UpTime() { 

  string line, uptime;
  //open a well formed filestream
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    //locate the uptime value and return the casted long value 
    linestream >> uptime;
    return SafeStol(uptime); 
  }

return 0;
}


//Return the sum of values of indices 0-7 of the CpuUtilization vector
long LinuxParser::ActiveJiffies() { 

  //fetch CpuUtilization vector
  vector<long> cpuCurrValues = CpuUtilization();
  long currActive = 0;
  //sum the first 7 values of the vector into total current sum 
  for (int i = 0 ; i <= kSteal_; i++){
    currActive += cpuCurrValues[i];
  }

return currActive;  
}

//Return the sum of indices 3 and 4
long LinuxParser::IdleJiffies() {

  //fetch CpuUtilization vector
  vector<long> cpuCurrValues = LinuxParser::CpuUtilization();
  //sum selected values into current idle sum 
  long currIdle = cpuCurrValues[kIdle_] + cpuCurrValues[kIOwait_];

return currIdle; 
}


//Return a vector of CpuUtilization figures 
vector<long> LinuxParser::CpuUtilization() {

  vector<long> cpuUtil;
  string line, ignore, figures;
  //open a well formed file stream
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> ignore;
    //construct a vector of indices 0-7 values from the system data
    for (auto i = 0; i <= kSteal_; i++){
      linestream >> figures;
      cpuUtil.push_back(SafeStol(figures));
    }

  }

return cpuUtil;  
}


//Return an integer value of the total running processes
int LinuxParser::TotalProcesses() { 

  string line, key, value, ignore;
  //define the search term
  key = "processes";
  //open input file stream and if it opens well formed, stream it into a linestream line by line while there are lines
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream iss(line);
      //if the linestream contains the search term return the second token on that line as an integer
      if (line.find(key, 0) != string::npos){
        iss >> ignore >> value;
        return SafeStoi(value);
      }
    }
  }

return 0;
}


//Return an integer of total running processes
int LinuxParser::RunningProcesses() { 

  string line, key, value, ignore;
  //define the search term
  key = "procs_running";
  //open input file stream and if it opens well formed, stream it into a linestream line by line while there are lines
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream iss(line);
      //if the linestream contains the search term return the second token on that line as an integer
      if (line.find(key, 0) != string::npos){
        iss >> ignore >> value;
        return SafeStoi(value);
      }
    }
  }

return 0;
}


//Returns the formatted command string
string LinuxParser::Command(const int& pid) { 
 
  string cmdLineString;
  //open a well formed file stream
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    //get the first line and return a formatted string of command chars
    std::getline(stream, cmdLineString);
  return "  " + cmdLineString;
  }

//else return default value 
return "~";
}


//Returns a string of the VmSize in MB
string LinuxParser::Ram(const int& pid) { 

  string line, key, value, ignore = "";
  //define the search term
  key = "VmSize:";
  //open input file stream and if it opens well formed, stream it into a linestream line by line while there are lines
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  unsigned long vmSize = 0;
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream iss(line);
      //if the linestream contains the search term return the second token on that line
      if (line.find(key, 0) != string::npos){
        iss >> ignore >> value;
        vmSize = SafeStol(value);
        break;
      }
    }
  return to_string(vmSize /1024);
  }

//else return default value  
return "~";
}


//Return the UID of a given process
string LinuxParser::Uid(const int& pid) {
  
  string line, key, value, ignore = "";
  //define the search term
  key = "Uid:";
  //open input file stream and if it opens well formed, stream it into a linestream line by line while there are lines
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  string uidString;
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream iss(line);
      //if the linestream contains the search term return the second token on that line
      if (line.find(key, 0) != string::npos){
        iss >> ignore >> value;
        uidString = value;
        break;
      }
    }
  return uidString;
  }

//else return default value
return "~";
}


//Return a string of the process user of a given process
string LinuxParser::User(const int& pid) {

  string uid = Uid(pid);
  string line, key, userString, ignore;
  //open a well formed file stream
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      //format the getline
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream iss(line);
      //locate the userstring for the given key
      while (iss >> userString >> ignore >> key) {
        if (key == uid) {
          return userString;
        }
      }
    }
  }

return userString;
}


//Return the uptime of a given pid
long LinuxParser::UpTime(const int& pid) {

  //fetch process utilization vector
  vector<string> procUtilVector = getProcUtilVector(pid);
  //get clockHz 
  double clockHz = sysconf(_SC_CLK_TCK);
  //get long of clockTicks
  long clockTicks = SafeStol(procUtilVector[kStarttime_]);

//calculate and return seconds of uptime
return clockTicks/clockHz;
}


//Return a vector of the processor utilization figures
vector<string> LinuxParser::getProcUtilVector(const int &pid){

  vector<string> procValues;
  string line, value;
  //open a well formed file stream
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream iss(line);
      //push the first 0-21 indices into a vector
      for (int i = 0; i <= kStarttime_; i++) {
        iss >> value;
        procValues.push_back(value);
      }
    }
  }

return procValues;
}


//Return a float of the processor utilization percentage
float LinuxParser::procUtilization(const int& pid) { 

  //get a vector of the first 22 values from /proc/{PID}/stat
  vector<string> procUtilVector = getProcUtilVector(pid);
  //totalTime = v[13]+v[14]
  double totalTime = SafeStol(procUtilVector[kUtime_]) + SafeStol(procUtilVector[kStime_]); 
  double clockHz = sysconf(_SC_CLK_TCK);
  //seconds = UpTime(pid) - (v[21] / ClockHz)
  double seconds = UpTime() - (SafeStol(procUtilVector[kStarttime_])/clockHz);
  //usage = 100* (total_time / ClockHz)/seconds
  float usage = (totalTime / clockHz)/seconds; 

return usage;
}