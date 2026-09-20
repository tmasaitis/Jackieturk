#ifndef CLAW
#define CLAW

#include "vex.h"
#include "pidController.hpp"
#include <algorithm>

struct Claw
{
  const double upperBound = 90;
  const double lowerBound = 0;


  PIDController &pidController;

  double targetPosition;

  vex::motor clawMotor;
  vex::rotation sensor;

  

  Claw(PIDController &pidController, int motorIndex, int rotationIndex, double initPosition = 0): pidController{pidController}, targetPosition{initPosition}, clawMotor{motorIndex}, sensor{rotationIndex}
  {
    sensor.setPosition(initPosition, vex::degrees);
    clawMotor.setStopping(vex::hold);
  }


  void setHigh()
  {
    targetPosition = upperBound;
  }

  void setLow()
  {
    targetPosition = lowerBound;
  }

  void step()
  {
    double position = sensor.position(vex::degrees);
    double error = targetPosition - position;
    double control = pidController.step(error);

    clawMotor.setVelocity(control, vex::percent);
    clawMotor.spin(vex::forward);
  }

};

#endif