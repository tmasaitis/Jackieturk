
#ifndef KEYINTERFACE

#define KEYINTERFACE
#include "vex.h"
#include <vector>
#include <functional>
//#include <array>

enum class Key {
  A, B, X, Y,
  Down, Up, Left, Right,
  L1, L2, R1, R2
};

class KeyInterface {
public:

private:
  std::vector< std::function<void()> > keyBindRoutines;
  std::vector< std::vector<Key> > keyBinds;
  
  std::vector< std::function<void()> > shadowedKeyBindRoutines;
  std::vector< std::vector<Key> > shadowedKeyBinds;
  
public:

  struct KeyState {
    bool A = false;
    bool B = false;
    bool X = false;
    bool Y = false;
    bool Down = false;
    bool Up = false;
    bool Left = false;
    bool Right = false;
    bool L1 = false;
    bool L2 = false;
    bool R1 = false;
    bool R2 = false;

    bool isIn(const KeyState &other) const
    {
      bool states1[] = {A, B, X, Y, Down, Up, Left, Right, L1, L2, R1, R2};
      bool states2[] = {other.A, other.B, other.X, other.Y, other.Down, other.Up, other.Left, other.Right, other.L1, other.L2, other.R1, other.R2};
      int length = sizeof(states1) / sizeof(bool);

      for(int i = 0; i < length; i++)
      {
        if (states1[i] && !states2[i])
          return false;
      }
      
      return true;
    }


    KeyState subtract(const KeyState &other) const {
      KeyState keyState{};

      bool * returnStates[] = {&keyState.A, &keyState.B, &keyState.X, &keyState.Y, &keyState.Down, &keyState.Up, &keyState.Left, &keyState.Right, &keyState.L1, &keyState.L2, &keyState.R1, &keyState.R2};
      bool states1[] = {A, B, X, Y, Down, Up, Left,Right, L1, L2, R1, R2};
      bool states2[] = {other.A, other.B, other.X, other.Y, other.Down, other.Up, other.Left, other.Right, other.L1, other.L2, other.R1, other.R2};
      int length = sizeof(states1) / sizeof(bool);
      
      for(int i = 0; i < length; i++)
      {
        *returnStates[i] = states1[i] && !states2[i];
      }

      return keyState;
    }
  };
    



  KeyState keysToKeyState(const std::vector<Key> &keys) const {
      KeyState keyState{};
      for (Key key : keys)
      {
        switch (key)
        {
          case Key::A: keyState.A = true; break;
          case Key::B: keyState.B = true; break;
          case Key::X: keyState.X = true; break;
          case Key::Y: keyState.Y = true; break;
          case Key::Down: keyState.Down = true; break;
          case Key::Up: keyState.Up = true; break;
          case Key::Left: keyState.Left = true; break;
          case Key::Right: keyState.Right = true; break;
          case Key::L1: keyState.L1 = true; break;
          case Key::L2: keyState.L2 = true; break;
          case Key::R1: keyState.R1 = true; break;
          case Key::R2: keyState.R2 = true; break;
        }
      }
      return keyState;
  }
  



  void pushKeyBind( std::function<void()> keyBindRoutine, std::vector<Key> keyBind, bool shadow = false)
  {
    if(shadow) {
      shadowedKeyBindRoutines.push_back(keyBindRoutine);
      shadowedKeyBinds.push_back(keyBind);
    } else {
      keyBindRoutines.push_back(keyBindRoutine);
      keyBinds.push_back(keyBind);
    }
  }


  KeyState getKeyState(vex::controller &Controller) {
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


  void pollInput(vex::controller &Controller)
  {
    KeyState keyState = getKeyState(Controller);
    
    int shadowedKeyBindsSize = shadowedKeyBinds.size();
    KeyState keyBind;
    
    for(int i = 0; i < shadowedKeyBindsSize; i++)
    {
      keyBind = keysToKeyState(shadowedKeyBinds[i]);
      if(keyBind.isIn(keyState))
      {
        shadowedKeyBindRoutines[i]();
        keyState = keyState.subtract(keyBind);
      }
    }


    int keyBindsSize = keyBinds.size();

    for(int i = 0; i < keyBindsSize; i++)
    {
      keyBind = keysToKeyState(keyBinds[i]);
      if(keyBind.isIn(keyState))
        keyBindRoutines[i]();
    }
  }
};


#endif
