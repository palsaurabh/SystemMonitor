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

// TODO: Return this process's ID
int Process::Pid() { return pid_; }


void Process::Pid(int p)
{
    pid_ = p;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
    return cpu_;
}
void Process::setCpuUtilization() 
{ 
    cpu_ = LinuxParser:: processCPUutilisation(pid_);
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

void Process::setCommand(int pid)
{
    command_.clear();
    command_ = LinuxParser::Command(pid);
}
// TODO: Return this process's memory utilization
string Process::Ram() { return Ram_; }
void Process::setRam() 
{
    Ram_.clear() ;
    Ram_ = LinuxParser::Ram(pid_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }
void Process::setUser(int pid)
{
    user_.clear();
    user_ = LinuxParser::User(pid);
}
// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }
void Process::setUpTime() { uptime_ = LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(const Process& a) const 
{ 
    return a.cpu_ < this->cpu_;
}
