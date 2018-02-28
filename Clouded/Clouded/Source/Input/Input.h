#pragma once
#include <OpenVr/openvr.h>

namespace vr
{
  class IVRSystem;
}



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
  
private:
  vr::IVRSystem* vr_system_;
  struct InputData* pImpl;
};
