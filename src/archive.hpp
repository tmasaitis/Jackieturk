#ifndef ARCHIVE
#define ARCHIVE
#include "vex.h"
#include <vector>
#include <string>

class Archive {
  const std::string file;
  
  Archive(vex::brain Brain, std::string file): file{file} {}

};


#endif
//Unfinished