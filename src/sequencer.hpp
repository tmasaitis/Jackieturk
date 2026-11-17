#ifndef SEQUENCER
#define SEQUENCER


class Sequencer {
  
  bool triggered{};
  double x{};

  double time;
  double dt;
  bool reset;

  double clamp(double x)
  {
    return std::max( std::min(x, 1), 0 );
  }
  

  Sequencer(double time, double dt, bool reset): reset{reset} {}

  void trigger() {
    if(!triggered)
    {
      x = 0;
      triggered = true;
    }
  }

  void step()
  {
    if(x < 1)
      x += dt / time
    else if(x >= 1 && reset)
    {
      x = 0;
      triggered = false;
    }
    else
      triggered = false;

    x = clamp(x);
  }
}


#endif