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
  const unsigned int maxVelocity = 50;
  int velocity = 0;

  

  PIDController &pidController;
  double targetPosition{};
  vex::motor liftMotor;
  vex::rotation liftSensor;
  bool enabled = false;

  

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
    if(enabled)
      velocity = maxVelocity;
      //targetPosition = clamp( targetPosition + sensitivity);
  }

  void decTarget()
  {
    if(enabled)
      velocity = -maxVelocity;
      //targetPosition = clamp( targetPosition - sensitivity);
  }

  void enable(bool state) {
    enabled = state;
  }

  void step()
  {
    double position = liftMotor.position(vex::degrees);
    
    if(velocity == 0 || position >= upperbound || position <= lowerBound) { 
      double error = targetPosition - position;
      double control = pidController.step(error);
      liftMotor.spin(vex::forward, control, vex::percent);
    } else {
      liftMotor.spin(vex::forward, velocity, vex::percent);
      targetPosition = position;
    }
  }

};

#endif
