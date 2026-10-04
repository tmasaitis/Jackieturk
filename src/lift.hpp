#ifndef LIFT
#define LIFT

#include "vex.h"
#include "pidController.hpp"
#include <algorithm>

struct Lift
{
  const double sensitivity = 50;
  const double upperBound = 16000;
  const double lowerBound = 0;

  

  PIDController &pidController;
  double targetPosition{};
  vex::motor liftMotor;
  vex::rotation liftSensor;

  

  Lift(PIDController &pidController, int motorIndex, int rotationIndex, double initPosition = 0): pidController{pidController}, targetPosition{initPosition}, liftMotor(motorIndex, vex::ratio6_1, true), liftSensor(rotationIndex)
  {
    //liftMotor = vex::motor(motorIndex);
    //liftSensor = vex::rotation(rotationIndex);
    //liftSensor.setPosition(initPosition, vex::degrees);
    liftMotor.setStopping(vex::hold);
  }


  double clamp(double x)
  {
    return std::max( std::min(x, upperBound), lowerBound );
  }


  void incTarget()
  {
    targetPosition = clamp( targetPosition + sensitivity);
  }

  void decTarget()
  {
    targetPosition = clamp( targetPosition - sensitivity);
  }

  void step()
  {
    //double position = liftSensor.position(vex::degrees);
    double position = liftMotor.position(vex::degrees);
    double error = targetPosition - position;
    double control = pidController.step(error);

    liftMotor.spin(vex::forward, control, vex::percent);
  }

};

#endif