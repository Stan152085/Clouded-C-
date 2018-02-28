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

  math::Vec3 Position(Controller type);
  math::Quat Rotation(Controller type);
  math::Vec3 Velocity(Controller type);
  math::Vec3 AngularVelocity(Controller type);

  bool IsButtonPressed(Controller type, vr::EVRButtonId button_id);
  bool IsButtonReleased(Controller type, vr::EVRButtonId button_id);
  bool IsButtonTouched(Controller type, vr::EVRButtonId button_id);


  Input( vr::IVRSystem* vr_input );
  void Poll();
  
private:
  vr::IVRSystem* vr_system_;
  struct InputData* pImpl;
};
