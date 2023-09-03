#include "processor.h"
#include "linux_parser.h"
#include <chrono>
#include <thread>

#include <iostream>
#include <string>

// DONE: Return the aggregate CPU utilization
/*
https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
*/
float Processor::Utilization() { 
 
  float cpu_utilization;
  
  long prev_idle = LinuxParser::IdleJiffies();
  long prev_total = LinuxParser::Jiffies();
    
  std::this_thread::sleep_for(std::chrono::duration<double>(0.5));
  
  long idle = LinuxParser::IdleJiffies();
  long total = LinuxParser::Jiffies();
  
  float total_delta = total - prev_total;
  float idle_delta = idle - prev_idle;
  
  cpu_utilization = (total_delta - idle_delta) / total_delta;

  return cpu_utilization;

  
  
}