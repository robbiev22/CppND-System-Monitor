#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

// constructor
Process::Process(int pid) : pid_(pid) 

{
  pid_  = pid;
  ram_ = Process::Ram();
}


// DONE: Return this process's ID
int Process::Pid() const { return Process::pid_; }

// DONE: Return this process's CPU utilization
/* source: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599*/
float Process::CpuUtilization() {

     try {
            cpu_usage = (float)LinuxParser::ActiveJiffies(pid_) /
            (float)(LinuxParser::UpTime() - LinuxParser::UpTime(pid_));
         } catch (...) {
         cpu_usage = 0.0;}
  return cpu_usage;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { 
  ram_ = LinuxParser::Ram(pid_);
  return ram_;}

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_);}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return std::stol(ram_) > std::stol(a.ram_); }