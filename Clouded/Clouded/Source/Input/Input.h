#pragma once
#include <OpenVr/openvr.h>

namespace vr
{
  class IVRSystem;
}

class Keyboard;

class Input
{
public:
  enum Controller
  {
    kHead,
    kLeftHand,
    kRightHand,
    kInvalid,
    kNumControlers = kInvalid
  };

  enum ControllerAxis
  {
    TrackPad,
    Trigger
  };

  enum KeyboardButton
  {
    ENTER = 0x0D,
    ZERO = 0x30,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    SPACE = 0x20,
    SHIFT = 0x10,
    CONTROL,
    ESC = 0x1B,
    LEFT = 0x25,
    UP,
    RIGHT,
    DOWN,
    A = 65,
    B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  };

  Mat44 Pose(Controller type);
  Vec3 Position(Controller type);
  Quat Rotation(Controller type);
  Vec3 Velocity(Controller type);
  Vec3 AngularVelocity(Controller type);

  bool IsButtonPressed(Controller type, vr::EVRButtonId button_id);
  bool IsButtonReleased(Controller type, vr::EVRButtonId button_id);
  bool IsButtonTouched(Controller type, vr::EVRButtonId button_id);

  Vec2 Axis(Controller type, ControllerAxis axis);

  Input( vr::IVRSystem* vr_input );
  void Poll();

  Keyboard& GetKeyboard();
  bool IsKeyPressed(KeyboardButton button);
  
private:
  vr::IVRSystem* vr_system_;
  struct InputData* pImpl;
};
