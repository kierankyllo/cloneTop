#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};


float MemoryUtilization() {   
  vector<int> memVector;
  string line, ignore, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    //for the first four lines in the file stream the file 
    for (int i = 0; i < 3; i++){
      //get a line from the filestream
      getline(stream, line);
      //convert the line to a stringstream
      std::istringstream linestream(line);
      //ignore the first and then push the second symbol on the line to a vector of integers and stoi() to integer
      linestream >> ignore >> value;
      memVector.push_back(stoi(value));
    }
  }
  //calculate the memory utilization float and return it
  float memUtil = (memVector[0] - memVector[1])/memVector[0];
  
  for(auto i : memVector){
    cout << i << "/n";
  }

return memUtil;
}

int TotalProcesses() { 

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

long UpTime() { 
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

string ElapsedTime(long seconds) { 
    long hh, mm, ss, dd;

    dd = seconds / 86400;
    hh = (seconds % 86400)/3600;
    mm = ((seconds % 86400)% 3600)/60;
    ss = (((seconds % 86400) % 3600) % 60) / 1;

string dhms = std::to_string(dd)+":"+std::to_string(hh)+":"+std::to_string(mm)+":"+std::to_string(ss);

return dhms; 
}

vector<long> CpuUtilization() { 
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

long ActiveJiffies() { 

vector<long> cpuCurrValues = CpuUtilization();

  unsigned long long int currTotal {0};
  //sum the first 7 values of the vector into total current sum 
  for (int i = 0 ; i < 8; i++){
      currTotal += cpuCurrValues[i];
  }

return currTotal;  
}

// TODO: Read and return the number of idle jiffies for the system
long IdleJiffies() { 

vector<long> cpuCurrValues = CpuUtilization();
  long currIdle {0};
  //sum selected values into current idle sum 
  currIdle = cpuCurrValues[3] + cpuCurrValues[4];

return currIdle; 
}

void printVector(vector<long> v){
  for (auto i : v){
    cout << i << " ";
  }
}


int main(){
    
  //printVector(CpuUtilization());
  double total = ActiveJiffies();
  double idle = IdleJiffies();
  double util = (total - idle)/total;
  std::cout << total << "\n";
  std::cout << idle << "\n";
  std::cout << util << "\n";
return 0;
}