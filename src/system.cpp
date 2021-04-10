#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


System::System()
{
    setOSName();
    setKernelVersion();
} 

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

void System::setOSName()
{
    OSName_ = LinuxParser::OperatingSystem();
}

void System::setKernelVersion()
{
    KernelVersion_ = LinuxParser::Kernel();
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes()
{
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for(int p : pids)
    {
        Process proc;
        proc.Pid(p);
        proc.setCommand(p);

        processes_.push_back(proc);
    }
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return KernelVersion_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return OSName_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses();}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }