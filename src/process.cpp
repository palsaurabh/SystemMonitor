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
    cpu_ = LinuxParser:: processCPUutilisation(pid_);
    return cpu_;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

void Process::setCommand(int pid)
{
    command_ = LinuxParser::Command(pid);
}
// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }
void Process::setUser(int pid)
{
    user_ = LinuxParser::User(pid);
}
// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
    return a.pid_ > this->pid_;
}
