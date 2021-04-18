#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(const Process& a) const ;  // TODO: See src/process.cpp
  void Pid(int);
  void setCommand(int);
  void setUser(int);
  void setCpuUtilization(); 
  void setRam();
  void setUpTime();
  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string command_;
  std::string user_;
  float cpu_;
  std::string Ram_;
  long int uptime_;
};

#endif