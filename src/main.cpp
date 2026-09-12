/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       200533725                                                 */
/*    Created:      9/10/2026, 2:47:15 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <cmath>
#define FPS 120
#define MSPF 1000/FPS


using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;
controller Controller = controller();

motorLeft1 = motor(11, false);
motorLeft2 = motor(12, false);
motorRight1 = motor(13, true);
motorRight2 = motor(14, true);


void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}


void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

const unsigned short throttleThreshold = 5;

void usercontrol(void) {

  while (true) {

    //Drive
    int throttle = Controller.Axis2.position();
    int rotation = Controller.Axis4.position();

    if (std::abs(throttle) > throttleThreshold || std::abs(rotation) > throttleThreshold) {
      int leftThrottle = throttle + rotation;
      int rightThrottle = throttle - rotation.

      motorLeft1.setVelocity(leftThrottle, percent);
      motorLeft2.setVelocity(leftThrottle, percent);
      motorRight1.setVelocity(rightThrottle, percent);
      motorRight2.setVelocity(rightThrottle, percent);
    }

    wait(SPF, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
