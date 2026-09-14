#ifndef CYLINDER
#define CYLINDER

#include "vex.h"

struct Cylinder {
  vex::pneumatics extendValve;
  vex::pneumatics retractValve;

  Cylinder(triport::port &extendPort, vex::triport::port &retractPort): extendValve(extendPort), retractValve(retractPort) {}

  void extend()
  {
    extendValve.open();
    retractValve.close();
  }
  void retract()
  {
    extendValve.close();
    retractValve.open();
  }
};


#endif