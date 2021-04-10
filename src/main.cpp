#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  system.setKernelVersion();
  system.setOSName();
  NCursesDisplay::Display(system);
}