#ifndef CLAW
#define CLAW

#include "vex.h"
#include "pidController.hpp"
#include <algorithm>

struct Claw
{
  const double clearBound = 720;
  const double upperBound = 360;
  const double midBound = 270;
  const double lowerBound = 0;


  PIDController &pidController;

  double targetPosition;

  vex::motor clawMotor;
  //vex::rotation sensor;

  

  Claw(PIDController &pidController, int motorIndex, double initPosition = 0): pidController{pidController}, targetPosition{initPosition}, clawMotor{motorIndex, vex::ratio18_1, false}//, sensor{rotationIndex}
  {
    //sensor.setPosition(initPosition, vex::degrees);
    clawMotor.setStopping(vex::hold);
  }


  void setHigh()
  {
    targetPosition = upperBound;
  }

  void setMid()
  {
    targetPosition = midBound;
  }

  void setLow()
  {
    targetPosition = lowerBound;
  }

  void setClear()
  {
    targetPosition = clearBound;
  }

  void step()
  {
    double position = clawMotor.position(vex::degrees);
    double error = targetPosition - position;
    double control = pidController.step(error);

    clawMotor.setVelocity(control, vex::percent);
    //clawMotor.setPosition(targetPosition, vex::degrees);
    clawMotor.spin(vex::forward);
  }

};

#endif