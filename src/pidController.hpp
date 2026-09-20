#ifndef PIDCONTROLLER
#define PIDCONTROLLER

#include <algorithm>

class PIDController
{
public:

  struct Parameters
  {
    double P;
    double I;
    double D;
    double limit;
  };

private:
  double accumulator = 0;
  double previousError = 0;
  Parameters &parameters;

  double clamp(double x)
  {
    double limit = parameters.limit;
    return std::max( std::min(x, limit), -limit );
  }

public:
  //Test to ensure where this should be enabled at the very beginning, when derivative is correctly computable.
  PIDController(Parameters &parameters): parameters{parameters} {}

  double step(double error)
  {
    accumulator = clamp(accumulator + parameters.I * error);
    double derivative = error - previousError;

    double control = parameters.P * error + accumulator + parameters.D * derivative;
    previousError = error;

    return control;
  }
};


#endif