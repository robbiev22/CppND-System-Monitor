#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hours;
  int minutes;
  string seconds_str;
  string minutes_str;
  string hours_str;
  
  hours = (seconds / 60) / 60; 
  minutes = (seconds / 60) % 60;
  seconds = seconds % 60;
  
  hours_str = (hours < 10) ? "0" + std::to_string(hours) : std::to_string(hours);
  minutes_str = (minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes);
  seconds_str = (seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds); 
  
  return hours_str + ":" + minutes_str + ":" + seconds_str;
}