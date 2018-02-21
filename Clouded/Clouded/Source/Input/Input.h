#pragma once
namespace vr
{
  class IVRSystem;
}
class Input
{
public:
  Input( vr::IVRSystem* vr_input );

  void Poll();

private:
  vr::IVRSystem* vr_system_;

  vr::ETrackingUniverseOrigin universe_origin_;
  vr::VREvent_t event_;
  vr::TrackedDevicePose_t pose_;
};