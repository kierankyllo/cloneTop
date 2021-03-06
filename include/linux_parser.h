#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
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

//Helpers
long SafeStol(const std::string& input);
int SafeStoi(const std::string& input);

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_ = 1,
  kSystem_ = 2,
  kIdle_ = 3,
  kIOwait_ = 4,
  kIRQ_ = 5,
  kSoftIRQ_ = 6,
  kSteal_ = 7,
  kGuest_,
  kGuestNice_
};

// Process utilization vector data indices
enum ProcessCPUStates {
  kUtime_ = 13,
  kStime_ = 14,
  kCutime_,
  kCstime_,
  kStarttime_ = 21
};

// CPU
std::vector<long> CpuUtilization();
long ActiveJiffies();
long ActiveJiffies(const int& pid);
long IdleJiffies();

// Processes
std::string Command(const int& pid);
std::string Ram(const int& pid);
std::string Uid(const int& pid);
std::string User(const int& pid);
long int UpTime(const int& pid);
std::vector<std::string> getProcUtilVector(const int& pid);
float procUtilization(const int& pid);
};  // namespace LinuxParser

#endif