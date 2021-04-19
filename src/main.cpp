#include "ncurses_display.h"
#include "system.h"
#include <iostream>

int main() {
  System system;
  // std::cout<<"Memory Utilisation:"<<system.MemoryUtilization()<<'\n';
  NCursesDisplay::Display(system);
}