#include "stdafx.h"
#include "Input.h"
#include "OpenVr/openvr.h"

Input::Input( vr::IVRSystem* vr_input )
  :
  vr_system_(vr_input),
  universe_origin_( vr::TrackingUniverseOrigin::TrackingUniverseStanding )
{
}

void Input::Poll()
{
  if ( vr_system_->PollNextEventWithPose( universe_origin_, &event_, 0, &pose_ ) )
  {
    
  }
}
