#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include <fstream>

std::ofstream Log;

int main() {
  Log.open("./log.txt", std::ios::out|std::ios::app);

  System system;
  // std::cout<<"Memory Utilisation:"<<system.MemoryUtilization()<<'\n';
  NCursesDisplay::Display(system);
}