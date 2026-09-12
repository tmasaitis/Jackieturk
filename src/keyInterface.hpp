
#ifndef KEYINTERFACE

#define KEYINTERFACE
#include "vex.h"
#include <vector>
//#include <array>

class KeyInterface {
private:
  std::vector< void(*)() > keyBindRoutines;
  std::vector< std::vector< vex::controller::button > > keyBinds;
  std::vector<bool> shadows;
  
public:

  enum class Key {
    A,
    B,
    X,
    Y,
    Down,
    Up,
    Left,
    Right,
    L1,
    L2,
    R1,
    R2
  };
  
  struct KeyState {
    bool A;
    bool B;
    bool X;
    bool Y;
    bool Down;
    bool Up;
    bool Left;
    bool Right;
    bool L1;
    bool L2;
    bool R1;
    bool R2;
  };


  KeyState controllerToKeyState(std::vector<Key> keys) {
    
    KeyState keyState{};
    int keyvectorSize = keys.size();
    
    for(int i = 0; i < keyvectorSize; i++)
    {
      
      Key key = keys[i];

      switch(key)
      {
      case Key::A:
	keyState.A = true;
	break;
      case Key::B:
	keyState.B = true;
	break;
      case Key::X:
	keyState.X = true;
	break;
      case Key::Y:
	keyState.Y = true;
	break;
      case Key::Down:
	keyState.Down = true;
	break;
      case Key::Up:
        keyState.Up = true;
	break;
      case Key::Left:
	keyState.Left = true;
	break;
      case Key::Right:
	keyState.Right = true;
	break;
      case Key::L1:
	keyState.L1 = true;
	break;
      case Key::L2:
	keyState.L2 = true;
	break;
      case Key::R1:
	keyState.R1 = true;
	break;
      case Key::R2:
	keyState.R2 = true;
	break;
      }
    }

    return keyState;
  }
  
  void pushKeyBind(void (*keyBindRoutine)(), std::vector<vex::controller::button> keyBind, bool shadow) {

    keyBindRoutines.push_back(keyBindRoutine);
    keyBinds.push_back(keyBind);
    shadows.push_back(shadow);
  }
  KeyState getKeyState(vex::controller Controller) {
    KeyState keyState{};
    keyState.A = Controller.ButtonA.pressing();
    keyState.B = Controller.ButtonB.pressing();
    keyState.X = Controller.ButtonX.pressing();
    keyState.Y = Controller.ButtonY.pressing();
    keyState.Down = Controller.ButtonDown.pressing();
    keyState.Up = Controller.ButtonUp.pressing();
    keyState.Left = Controller.ButtonLeft.pressing();
    keyState.Right = Controller.ButtonRight.pressing();
    keyState.L1 = Controller.ButtonL1.pressing();
    keyState.L2 = Controller.ButtonL2.pressing();
    keyState.R1 = Controller.ButtonR1.pressing();
    keyState.R2 = Controller.ButtonR2.pressing();

    return keyState;
  }

  void pollInput(vex::controller Controller)
  {
    KeyState keyState = getKeyState(Controller);
    //Left Off
  }
};


#endif
