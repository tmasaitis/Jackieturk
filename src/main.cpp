/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       tmasaitis                                                 */
/*    Created:      9/10/2026, 2:47:15 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

// Disable features not supported or needed on VEX bare-metal RTOS
//#define TOML_ENABLE_UNWINDOWS 0
//#define TOML_EXCEPTIONS 0 // Optional: disables C++ exceptions if your project uses -fno-exceptions

//#include "toml.hpp"

#include <cmath>
#include "keyInterface.hpp"
#include "pidController.hpp"
#include "lift.hpp"
#include "claw.hpp"
#include <string>

#define TPS 120
#define MSPT (1000/TPS)


using namespace vex;

competition Competition;
brain Brain;
controller Controller = controller();

motor motorL1 = motor(0, ratio18_1, true);
motor motorL2 = motor(19, ratio18_1, false);
motor motorR1 = motor(9, ratio18_1, true);
motor motorR2 = motor(8, ratio18_1, false);

motor_group motorsL = motor_group(motorL1, motorL2);
motor_group motorsR = motor_group(motorR1, motorR2);



PIDController::Parameters liftParams{1, 0, 1, 100};
PIDController liftPID = PIDController(liftParams);
//PID, motor index, rotation index, initPosition:
Lift * lift = nullptr;
/*
PIDController::Parameters clawParams{1, 0, 1, 50};
PIDController clawPID = PIDController(clawParams);
//PID, motor index, rotation index, initPosition:
Claw claw(clawPID, 9, 10, 0);
*/


void pre_auton(void)
{
  lift = new Lift(liftPID, 20, 18, 0);
}




void autonomous(void)
{

}




const unsigned short throttleThreshold = 5;
bool spinning = false;
bool lastSpinning = false;

void userDrive()
{
  int throttle = Controller.Axis3.position();
  int rotation = Controller.Axis1.position();

  if (std::abs(throttle) > throttleThreshold || std::abs(rotation) > throttleThreshold) {
    int leftThrottle = throttle + rotation;
    int rightThrottle = throttle - rotation;
    
    motorsL.spin(forward, leftThrottle, percent);
    motorsR.spin(forward, rightThrottle, percent);

  } else {
    motorsL.stop();
    motorsR.stop();
  }

  lastSpinning = spinning;
}




void usercontrol(void)
{
  
  
  //Init User Interface
  KeyInterface userInterface;
  userInterface.pushKeyBind([]() {lift->incTarget();}, std::vector<Key> {Key::R1});
  userInterface.pushKeyBind([]() {lift->decTarget();}, std::vector<Key> {Key::R2});

  //userInterface.pushKeyBind([]() {claw.setHigh();}, std::vector<Key> {Key::Up});
  //userInterface.pushKeyBind([]() {claw.setLow();}, std::vector<Key> {Key::Down});

  while (true)
  {
    userDrive();
    userInterface.pollInput(Controller);
    //lift.step();
    lift->liftMotor.spin(forward, 20, percent);
    //claw.step();
    Brain.Screen.clearLine(1);
    Brain.Screen.setCursor(1,1);
    Brain.Screen.print( "%lf", lift->pidController.controlLog);
    wait(MSPT, msec);
  }
}




int main()
{
  motorsL.setStopping(hold);
  motorsR.setStopping(hold);


  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
