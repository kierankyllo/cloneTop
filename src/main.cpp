#include "ncurses_display.h"
#include "system.h"
//testing
//#include "linux_parser.h"
//#include <iostream>

int main() {

  System system;
  NCursesDisplay::Display(system);
  
  //testing
  //td::cout << LinuxParser::ActiveJiffies() << " " << LinuxParser::IdleJiffies(); 
}