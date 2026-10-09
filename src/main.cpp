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
#include "edge.hpp"

#define TPS 120
#define MSPT (1000/TPS)


using namespace vex;

competition Competition;
brain Brain;
controller Controller = controller();

motor intake = motor(17, ratio36_1, false);
short intakeSpeed = 0;
const unsigned short maxIntakeSpeed = 75;

motor motorL1 = motor(0, ratio18_1, true);
motor motorL2 = motor(19, ratio18_1, false);
motor motorR1 = motor(9, ratio18_1, true);
motor motorR2 = motor(8, ratio18_1, false);

motor_group motorsL = motor_group(motorL1, motorL2);
motor_group motorsR = motor_group(motorR1, motorR2);



PIDController::Parameters liftParams{0.25, 0, 2, 100};
PIDController liftPID = PIDController(liftParams);
//PID, motor index, rotation index, initPosition:
Lift * lift = nullptr;

PIDController::Parameters clawParams{0.5, 0, 1, 30};
PIDController clawPID = PIDController(clawParams);
//PID, motor index, rotation index, initPosition:
Claw * claw = nullptr;


pneumatics clawCylinder(Brain.ThreeWirePort.A);


void pre_auton(void)
{
  lift = new Lift(liftPID, 18, 16, 0);
  claw = new Claw(clawPID, 7, 0);
  intake.spin(forward);
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
  int rotation = Controller.Axis1.position() * 0.75;

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

double linIntNorm(double x, double lower, double upper) {
  if(x <= lower)
    return 0;
  else if(x >= upper)
    return 1;

  return (x - lower) / (upper - lower);
}


bool unfoldSequence = false;
bool foldSequence = false;
bool unfolded = false;
bool foldButton = false;
bool clawCleared = false;
Edge foldEdge;

void fold() {
  
  if(!unfolded)
    unfoldSequence = true;
  else if(foldEdge.rising()) {
    clawCylinder.close();
    claw->setClear();
    clawCleared = true;
  }


}

void sequenceStep() {

  if(foldEdge.falling() && clawCleared)
  {
    foldSequence = true;
    clawCleared = false;
  }

  if(unfoldSequence) {
    clawCylinder.open();
    double normPosition = linIntNorm(lift->liftMotor.position(degrees), 0, 4000);
    lift->targetPosition = 4000;

    claw->targetPosition = normPosition * claw->upperBound;

    if(normPosition > 0.9) {
      claw->setHigh();
      unfoldSequence = false;
      unfolded = true;
      lift->enable(true);
    }
  }

  if(foldSequence) {
    clawCylinder.close();
    lift->targetPosition = 0;
    if(lift->liftMotor.position(degrees) < 1000)
    {
      claw->setLow();
      foldSequence = false;
      unfolded = false;
      lift->enable(false);
    }
  }
  if(unfolded && lift->liftMotor.position(degrees) < 100 && !foldSequence && !clawCleared && !unfoldSequence && unfolded)
    claw->setMid();
  else if (!foldSequence && !clawCleared && !unfoldSequence && unfolded)
    claw->setHigh();
}


void usercontrol(void)
{
  
  
  //Init User Interface here something son son son
  KeyInterface userInterface;
  userInterface.pushKeyBind([]() {lift->incTarget();}, std::vector<Key> {Key::R1});
  userInterface.pushKeyBind([]() {lift->decTarget();}, std::vector<Key> {Key::R2});

  userInterface.pushKeyBind([]() {intake.setVelocity(maxIntakeSpeed, percent);}, std::vector<Key> {Key::L1});
  userInterface.pushKeyBind([]() {intake.setVelocity(-maxIntakeSpeed, percent);}, std::vector<Key> {Key::L2});

  userInterface.pushKeyBind([]() {clawCylinder.open();}, std::vector<Key> {Key::Up});
  userInterface.pushKeyBind([]() {clawCylinder.close();}, std::vector<Key> {Key::Down});

  userInterface.pushKeyBind([]() {fold(); foldButton = true;}, std::vector<Key> {Key::A});

  while (true)
  {
    intake.setVelocity(0, percent);
    userInterface.pollInput(Controller);
    
    userDrive();
    lift->step();
    claw->step();
    Brain.Screen.clearLine(1);
    Brain.Screen.setCursor(1,1);
    Brain.Screen.print( "%lf", lift->liftMotor.position(degrees));
    //Brain.Screen.print( "%lf", lift->pidController.controlLog);
    //Brain.Screen.print( "%lf", claw->clawMotor.position(degrees));
    
    sequenceStep();

    foldEdge.step(foldButton);
    foldButton = false;
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
