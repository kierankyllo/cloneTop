#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
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


string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
return kernel;
}

//BONUS: implement with std::filesystem
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
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


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
      //ignore the first and then push the second symbol on the line to a vector of integers and stoi() to integer
      linestream >> ignore >> value;
      memVector.push_back(stoi(value));
    }
  }
  //calculate the memory utilization float and return it
  float memUtil = (memVector[0] - memVector[1])/memVector[0];
return memUtil;
}


long LinuxParser::UpTime() { 
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return stol(uptime); 
  }
return 0;
}


long LinuxParser::Jiffies() { 
vector<long> cpuCurrValues = LinuxParser::CpuUtilization();
  long currJiff {0};
  //sum the first 7 values of the vector into total current sum 
  for (int i = 0 ; i < 10; i++){
      currJiff += cpuCurrValues[i];
  }
return currJiff;  
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }


long LinuxParser::ActiveJiffies() { 
vector<long> cpuCurrValues = CpuUtilization();
  long currActive {0};
  //sum the first 7 values of the vector into total current sum 
  for (int i = 0 ; i < 8; i++){
      currActive += cpuCurrValues[i];
  }
return currActive;  
}


long LinuxParser::IdleJiffies() { 
vector<long> cpuCurrValues = LinuxParser::CpuUtilization();
  long currIdle {0};
  //sum selected values into current idle sum 
  currIdle = cpuCurrValues[3] + cpuCurrValues[4];
return currIdle; 
}


vector<long> LinuxParser::CpuUtilization() { 
  vector<long> cpuUtil;
  string line, ignore, figures;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> ignore;
    for (auto i = 0; i< 10; i++){
      linestream >> figures;
      cpuUtil.push_back(stoll(figures));
    }

  }
return cpuUtil;  
}


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
        return std::stoi(value);
      }
    }
  }
return 0;
}


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
        return std::stoi(value);
      }
    }
  }
return 0;
}


string LinuxParser::Command(int pid) { 
  string cmdLineString;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmdLineString);
  return cmdLineString;
  }
return "~";
}


string LinuxParser::Ram(int pid) { 
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
        vmSize = stol(value);
        break;
      }
    }
  return to_string(vmSize /1024);
  }
return "~";
}

string LinuxParser::Uid(int pid) {
  
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
return "~";
}


string LinuxParser::User(int pid) {

  string uid = Uid(pid);
  string line, key, userString, ignore;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> userString >> ignore >> key) {
        if (key == uid) {
          return userString;
        }
      }
    }
  }
  return userString;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
