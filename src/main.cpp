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
#include "keyInterface.hpp"
#include "pidController.hpp"
#include "lift.hpp"

#define TPS 120
#define MSPT (1000/TPS)


using namespace vex;

competition Competition;
brain Brain;
controller Controller = controller();

motor motorL1 = motor(11, false);
motor motorL2 = motor(12, false);
motor motorR1 = motor(13, true);
motor motorR2 = motor(14, true);

motor_group motorsL = motor_group(motorL1, motorL2);
motor_group motorsR = motor_group(motorR1, motorR2);

motorsL.setStopping(hold);
motorsR.setStopping(hold);

PIDController::Parameters liftParams{1, 0, 1, 100};
PIDController liftPID = PIDController(liftParams);
//PID, motor index, rotation index, initPosition:
Lift lift(liftPID, 19, 20, 0);



void pre_auton(void)
{

}




void autonomous(void)
{

}




const unsigned short throttleThreshold = 5;
void userDrive()
{
  int throttle = Controller.Axis2.position();
  int rotation = Controller.Axis4.position();

  if (std::abs(throttle) > throttleThreshold || std::abs(rotation) > throttleThreshold) {
    int leftThrottle = throttle + rotation;
    int rightThrottle = throttle - rotation;
    
    motorsL.setVelocity(leftThrottle, percent);
    motorsR.setVelocity(rightThrottle, percent);

    motorsL.spin(forward);
    motorsR.spin(forward);
  } else {
    motorsL.stop();
    motorsR.stop();
  }
}


void usercontrol(void)
{
  //Init User Interface
  KeyInterface userInterface;
  userInterface.pushKeyBind([]() {lift.incTarget();}, std::vector<Key> {Key::R1});
  userInterface.pushKeyBind([]() {lift.decTarget();}, std::vector<Key> {Key::R2});

  while (true)
  {

    userDrive();
    userInterface.pollInput(Controller);
    lift.step();

    wait(MSPT, msec);
  }
}




int main()
{

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
