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
  long sysUpTimeSecs;
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
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{


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
  std::string value;
  std::ifstream processStatusFile(kProcDirectory + to_string(pid) + kStatusFilename); 
  if(processStatusFile.is_open())
  {
    while(std::getline(processStatusFile, line) && key != "VmSize:")
    {
      std::istringstream linestream(line);
      linestream>>key>>value;
    }
  }

  // return value;
  // std::cout<<"2nd stoi"<<'\n';

  return to_string((stoi(value)/1024)/1024);
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
  // return to_string((stoi(value)/1024)/1024);

}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{
  std::string line;
  std::string key;
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
  std::ifstream processStatusFile(kProcDirectory + to_string(pid) + kStatFilename);
  if (processStatusFile.is_open()) 
  {
    std::getline(processStatusFile, line);
    std::istringstream stream(line);
    int count = 0;
    while(count < 22 && stream >> temp)
    {
      count++;
    }
  }
  return std::strtol(temp.c_str(), NULL, 10);

}
