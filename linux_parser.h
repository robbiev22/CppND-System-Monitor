#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <map>

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

// Keywords
const std::string filterProcesses{"processes"};
const std::string filterRunningProcesses{"procs_running"};
const std::string filterMemoryFree{"MemFree:"};
const std::string filterMemoryTotal{"MemTotal:"};
const std::string filterRAM{"VmRSS:"}; // I replaced VMSize with VmRSS
const std::string filterUID{"Uid:"};
const std::string filterCPU{"cpu"};
const std::string filterKernel{"version"};
  
// Positions
const int extractUpTimeProcess = 21;
const int extractCommand = 0;

// Templates

template <typename T>
T FindValueByKey(std::string const &keyFilter, std::string const &filename){
  std::string key;
  std::string line;
  T value;
    
  std::ifstream stream(kProcDirectory + filename);
  
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == keyFilter){
          linestream >> value;
          stream.close();
          return value;
        }
      }
    }
  }
  stream.close();
  return value;
}

template <typename T>
T FindValueByPosition(int const &extractPosition, std::string const &filename){
  std::string line;
  T value;
  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i <= extractPosition; i++){
      linestream >> value;
    }
    return value;
    stream.close();
  }
  return value;
}
  
  
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
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
  
enum CPUStatesProcess {
  kUtime_ = 13,
  kStime_,
  kCUtime_,
  kCStime_
};
  
  
  
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif