#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
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
  string value{"NA"};
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
  string os, version, kernel{"NA"};
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  std::string line;
  std::string key;
  long memTotal;
  long memUsed;
  std::ifstream memoryStatusFile(kProcDirectory + kMeminfoFilename); 
  if(memoryStatusFile.is_open())
  {
    std::string MemTotal;
    std::string MemFree;
    std::getline(memoryStatusFile, line);
    std::istringstream MemTotalstream(line);
    MemTotalstream>>key>>MemTotal;
    std::getline(memoryStatusFile, line);
    std::istringstream MemFreestream(line);
    MemFreestream>>key>>MemFree;
    memTotal = std::strtol(MemTotal.c_str(), NULL, 10);
    memUsed = (memTotal - std::strtol(MemFree.c_str(), NULL, 10));
  }

  return ((static_cast<float>(memUsed)/memTotal));
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  std::string line;
  long sysUpTimeSecs{0};
  std::ifstream uptimefile(kProcDirectory + kUptimeFilename);
  if(uptimefile.is_open())
  {
    std::getline(uptimefile, line);
    std::istringstream iss(line);
    std::string secs;
    iss>>secs;
    sysUpTimeSecs = static_cast<long>(std::strtof(secs.c_str(), NULL));
  }
  return sysUpTimeSecs;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{
  std::string line;
  std::string scratchPad;
  long totalJiffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
 
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int state = kUser_; state <= kSteal_; state++)
    {
      stream >> scratchPad; 
      totalJiffies += strtol(scratchPad.c_str(), NULL, 10);
    }
  }
 
  return totalJiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  std::string line;
  std::string scratchPad;
  long ActiveJiffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
 
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int state = kUser_; state <= kSteal_; state++)
    {
      stream >> scratchPad; 
      if(state != kIdle_ && state != kIOwait_)
      {
        ActiveJiffies += strtol(scratchPad.c_str(), NULL, 10);
      }
    }
  }
 
  return ActiveJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  std::string line;
  std::string scratchPad;
  long idleJiffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
 
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int state = kUser_; state <= kGuestNice_; state++)
    {
      stream >> scratchPad; 
      if(state >= kIdle_ && state <= kIOwait_)
      {
        idleJiffies += strtol(scratchPad.c_str(), NULL, 10);
      }
    }
  }
 
  return idleJiffies; 
  
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(long currentIdle, long currentActive, long prevIdle, long prevNonIdle) 
{
//Help taken from
//https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  long prevTotal = prevIdle + prevNonIdle;
  long total = currentIdle + currentActive;


  // # differentiate: actual value minus the previous one
  long totald = total - prevTotal;
  long idled = currentIdle - prevIdle;

  if(totald > 0)
  {
    return (float)((float)totald - idled)/totald;
  }
  else 
    return -1.0;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  std::string line;
  std::string key;
  std::string value{"0"};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line) && key != "processes")
    {
      std::istringstream linestream(line);
      linestream>>key>>value;
    }
  }
  // std::cout<<"1st stoi"<<'\n';
  return stoi(value); 

}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
  std::string line;
  std::string key;
  std::string value{"0"};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line) && key != "procs_running")
    {
      std::istringstream linestream(line);
      linestream>>key>>value;
    }
  }
 
  // std::cout<<"2nd stoi"<<'\n';
  return stoi(value); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  std::string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
  std::string line;
  std::string key;
  std::string value{"0"};
  std::ifstream processStatusFile(kProcDirectory + to_string(pid) + kStatusFilename); 
  if(processStatusFile.is_open())
  {
    while(std::getline(processStatusFile, line) && key != "VmSize:")
    {
      std::istringstream linestream(line);
      linestream>>key>>value;
    }
  }

  return to_string((stoi(value)/1024));
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
  std::string line;
  std::string key;
  std::string value;
  std::ifstream processStatusFile(kProcDirectory + to_string(pid) + kStatusFilename);
  if(processStatusFile.is_open())
  {
    while(std::getline(processStatusFile, line) && key != "Uid:")
    {
      std::istringstream linestream(line);
      linestream>>key>>value;
    }
  }

  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{
  std::string line;
  std::string key{"NA"};
  std::string value;
  std::string x;
  std::string uid = LinuxParser::Uid(pid);
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value; 
      if (value == uid) 
      {
        break;      
      }
    }
  }

  return key; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{
  std::string line;
  std::string temp;
  long uptime;
  std::ifstream processStatusFile(kProcDirectory + to_string(pid) + kStatFilename);
  if (processStatusFile.is_open()) 
  {
    std::getline(processStatusFile, line);
    std::istringstream stream(line);
    int count = 0;
    while(count < STARTTIME && stream >> temp)
    {
      count++;
    }
  }

  uptime = LinuxParser::UpTime() - (std::strtol(temp.c_str(), NULL, 10))/ sysconf(_SC_CLK_TCK);

  return uptime;
}

float LinuxParser::processCPUutilisation(int pid)
{
  std::string line;
  std::string temp;
  long utime, stime, cutime, cstime;
  std::ifstream processStatusFile(kProcDirectory + to_string(pid) + kStatFilename);
  if (processStatusFile.is_open()) 
  {
    std::getline(processStatusFile, line);
    std::istringstream stream(line);
    int count = 0;
    while(count < CSTIME && stream >> temp)
    {
      count++;
      if(count == UTIME)
      {
        utime = strtol(temp.c_str(), NULL, 10);
      }
      else if(count == STIME)
      {
        stime = strtol(temp.c_str(), NULL, 10);
      }
      else  if(count == CUTIME)
      {
        cutime = strtol(temp.c_str(), NULL, 10);
      }
      else  if(count == CSTIME)
      {
        cstime = strtol(temp.c_str(), NULL, 10);
      }
    }
  }

  long totaltime = utime + stime + cstime + cutime;
  long time = LinuxParser::UpTime(pid);
  float cpuUsage;
  if(time != 0)
  {
    cpuUsage = ((float)totaltime/sysconf(_SC_CLK_TCK))/time;
  }
  else cpuUsage = 0;

  return cpuUsage;
}

