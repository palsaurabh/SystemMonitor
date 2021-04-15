#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp


 void setIdleJiffies(long jiffies);
 void setActiveJiffies(long jiffies);
 void getIdleJiffies(long jiffies);
 void getActiveJiffies(long jiffies);
  // TODO: Declare any necessary private members
 private:
 long idleJiffies;
 long activeJiffies;
};

#endif