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


  Input( vr::IVRSystem* vr_input );
  void Poll();
  
private:
  vr::IVRSystem* vr_system_;

  vr::ETrackingUniverseOrigin universe_origin_;
  vr::VREvent_t event_;
  vr::TrackedDevicePose_t pose_;
  struct InputData* pImpl;
};
