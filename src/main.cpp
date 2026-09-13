/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       tmasaitis                                                 */
/*    Created:      9/10/2026, 2:47:15 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <cmath>
#define TPS 120
#define MSPT 1000/TPS


using namespace vex;

competition Competition;
brain Brain;
controller Controller = controller();

motorL1 = motor(11, false);
motorL2 = motor(12, false);
motorR1 = motor(13, true);
motorR2 = motor(14, true);

motorsL = motor_group(motorL1, motorL2);
motorsR = motor_group(motorR1, motorR2);


void pre_auton(void) {

}


void autonomous(void) {

}

const unsigned short throttleThreshold = 5;

void usercontrol(void) {

  while (true) {

    //Drive
    int throttle = Controller.Axis2.position();
    int rotation = Controller.Axis4.position();

    if (std::abs(throttle) > throttleThreshold || std::abs(rotation) > throttleThreshold) {
      int leftThrottle = throttle + rotation;
      int rightThrottle = throttle - rotation;

      motorsL.setVelocity(leftThrottle, percent);
      motorsR.setVelocity(rightThrottle, percent);
    }

    wait(MSPT, msec);
  }
}




int main() {

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
