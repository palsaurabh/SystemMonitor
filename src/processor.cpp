#include "processor.h"
#include "linux_parser.h"
#include "iostream"

void Processor::setIdleJiffies(long jiffies)
{
    idleJiffies = jiffies;
}
void Processor::setActiveJiffies(long jiffies)
{
    activeJiffies = jiffies;
}
long Processor::getIdleJiffies()
{
    return idleJiffies;
}
long Processor::getActiveJiffies()
{
    return activeJiffies;
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    float cpuUtil = 0.0;

    long currIdleJiffies = LinuxParser::IdleJiffies();
    long currActiveJiffies = LinuxParser::ActiveJiffies();
    // std::cout<<"currIdleJiffies = "<<currIdleJiffies<<'\n';
    // std::cout<<"currActiveJiffies = "<<currActiveJiffies<<'\n';
    cpuUtil = LinuxParser::CpuUtilization(currIdleJiffies, currActiveJiffies, 
                                        getIdleJiffies(), getActiveJiffies());
    // std::cout<<"CPUUtil = "<<cpuUtil<<'\n';
    setActiveJiffies(currActiveJiffies);
    setIdleJiffies(currIdleJiffies);
    CPUUtil_ = cpuUtil;
    return cpuUtil; 
}