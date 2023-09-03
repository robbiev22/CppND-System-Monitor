
#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <cctype>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string kernel;
  kernel = FindValueByKey<string>(filterKernel, kVersionFilename);
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memTotal; 
  float memFree;
  memTotal = FindValueByKey<int>(filterMemoryTotal,  kMeminfoFilename);
  memFree = FindValueByKey<int>(filterMemoryFree,  kMeminfoFilename);
  return  1 - (memTotal - memFree) / memTotal;
}


// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  stream.close();
  return uptime; 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {  
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::string line, value;
  std::vector<std::string> data;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      std::istream_iterator<std::string> begin(linestream), end;
      std::vector<std::string> values(begin, end);
      return (std::stof(values[CPUStatesProcess::kUtime_]) + std::stof(values[CPUStatesProcess::kStime_]) +
              std::stof(values[CPUStatesProcess::kCUtime_]) + std::stof(values[CPUStatesProcess::kCStime_])) /
             sysconf(_SC_CLK_TCK);
    }
  }
  stream.close();
  return 0;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = LinuxParser::CpuUtilization();
  long totalJiffies;
  
  totalJiffies = std::stol(jiffies[CPUStates::kUser_]) + 
                  std::stol(jiffies[CPUStates::kNice_])  +
                  std::stol(jiffies[CPUStates::kSystem_]) + 
                  std::stol(jiffies[CPUStates::kIRQ_]) + 
                  std::stol(jiffies[CPUStates::kSoftIRQ_]) + 
                  std::stol(jiffies[CPUStates::kSteal_]);
  
   return totalJiffies;;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies =  LinuxParser::CpuUtilization();
  long totalJiffies;
  totalJiffies = std::stol(jiffies[CPUStates::kIdle_]) + std::stol(jiffies[CPUStates::kIOwait_]);
 return totalJiffies; 
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> jiffies;
  string key;
  string value;
  string line;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    if (key == filterCPU){
      while (linestream >> value){
        jiffies.push_back(value);
      }
    }
  }
  stream.close();
  return jiffies;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {  
  int processes;
  processes = FindValueByKey<int>(filterProcesses, kStatFilename);
  return processes; 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {  
  int runningProcesses;
  runningProcesses = FindValueByKey<int>(filterRunningProcesses, kStatFilename);
  return runningProcesses;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  command = FindValueByPosition<string>(extractCommand, std::to_string(pid) + kCmdlineFilename);
  return command;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string ram;
  ram = FindValueByKey<string>(filterRAM, std::to_string(pid) + kStatusFilename);
  if (ram.empty()){
    return 0;
  }
  else {
    return std::to_string(stol(ram) / 1024);
  }
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string uid;
  uid = FindValueByKey<string>(filterUID, std::to_string(pid) + kStatusFilename);
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  
  string value;
  string key;
  string username;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      linestream >> value >> key;
      if (key == Uid(pid)){
        username = value;
        stream.close();
        return username;
      }
    }
  }
  stream.close();
  return "";
}

// DONE: Read and return the uptime of a process 
long LinuxParser::UpTime(int pid) { 
  string uptime;
  uptime = FindValueByPosition<std::string>(extractUpTimeProcess, std::to_string(pid) + kStatFilename);
  return std::stol(uptime) / sysconf(_SC_CLK_TCK);
}
/*
COMMENT: I don't understand why below code would give me wrong values for uptime.
long uptime;
uptime = FindValueByPosition<long>(extractUpTimeProcess, std::to_string(pid) + kStatFilename);
return uptime / sysconf(_SC_CLK_TCK);
*/ 