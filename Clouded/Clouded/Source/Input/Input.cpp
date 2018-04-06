#include "stdafx.h"
#include "Input.h"
#include "Math\math_defines.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
struct VRControllerData
{
  Vec3 pos;
  Quat rot;
  Vec3 vel;
  Vec3 angular_vel;

  uint64_t button_pressed;
  uint64_t last_button_pressed;
  uint64_t button_touched;
  Vec2 axis[vr::k_unControllerStateAxisCount];
  bool is_connected;
  bool is_valid;
};

struct InputData
{
  VRControllerData controller_data[Input::kNumControlers];
};

Mat44 Input::Pose(Controller type)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  return glm::translate(input_data.controller_data[type].pos) *
         glm::toMat4(input_data.controller_data[type].rot)*
         Mat44(1.0f);
}

Vec3 Input::Position(Controller type)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  return input_data.controller_data[type].pos;
}

Quat Input::Rotation(Controller type)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  return input_data.controller_data[type].rot;
}
Vec3 Input::Velocity(Controller type)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  return input_data.controller_data[type].vel;
}
Vec3 Input::AngularVelocity(Controller type)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  return input_data.controller_data[type].angular_vel;
}

bool Input::IsButtonPressed(Controller type, vr::EVRButtonId button_id)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  return (vr::ButtonMaskFromId(button_id) & input_data.controller_data[type].button_pressed);
}

bool Input::IsButtonReleased(Controller type, vr::EVRButtonId button_id)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  uint64_t mask = vr::ButtonMaskFromId(button_id);
  bool last = (mask & input_data.controller_data[type].last_button_pressed);
  return ((mask & input_data.controller_data[type].button_pressed) == false && last == true);
}

bool Input::IsButtonTouched(Controller type, vr::EVRButtonId button_id)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  return  (vr::ButtonMaskFromId(button_id) & input_data.controller_data[type].button_touched);
}

Vec2 Input::Axis(Controller type, ControllerAxis axis)
{
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  return input_data.controller_data[type].axis[axis];
}

Input::Input( vr::IVRSystem* vr_input )
  :
  vr_system_( vr_input )
{
  pImpl = new InputData();
}

// Get the quaternion representing the rotation
Quat GetRotation( vr::HmdMatrix34_t matrix ) {
  Quat q;

  q.w = sqrtf( fmaxf( 0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2] ) ) / 2;
  q.x = sqrtf( fmaxf( 0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2] ) ) / 2;
  q.y = sqrtf( fmaxf( 0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2] ) ) / 2;
  q.z = sqrtf( fmaxf( 0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2] ) ) / 2;
  q.x = copysignf( q.x, matrix.m[2][1] - matrix.m[1][2] );
  q.y = copysignf( q.y, matrix.m[0][2] - matrix.m[2][0] );
  q.z = copysignf( q.z, matrix.m[1][0] - matrix.m[0][1] );
  return q;
}

// Get the vector representing the position
Vec3 GetPosition( vr::HmdMatrix34_t matrix ) 
{
  return Vec3(matrix.m[0][3], matrix.m[1][3], matrix.m[2][3]);
}
void Input::Poll()
{
  // TODO(Raymi): Swap to keyboard input
  if (vr_system_ == nullptr)
  {
    return;
  }
  InputData& input_data = reinterpret_cast<InputData&>(*pImpl);
  vr::VREvent_t event;
  while ( vr_system_->PollNextEvent( &event, sizeof( event ) ) )
  {
  }
  char buf[1024];
  for ( vr::TrackedDeviceIndex_t device_id = 0; device_id < vr::k_unMaxTrackedDeviceCount; device_id++ )
  {
    if ( vr_system_->IsTrackedDeviceConnected( device_id ) == false )
    {
      continue;
    }

    vr::TrackedDevicePose_t pose;
    vr::VRControllerState_t state;


    if ( vr_system_->IsInputFocusCapturedByAnotherProcess() )
    {
      sprintf_s( buf, sizeof( buf ), "\nInput Focus by Another Process\n" );
      printf_s( buf );
    }

    vr::ETrackingResult result;
    // Get what type of device it is and work with its data
    vr::ETrackedDeviceClass device_class = vr::VRSystem()->GetTrackedDeviceClass( device_id );
    //vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
    //vr::VRCompositor()->WaitGetPoses(poses, vr::k_unMaxTrackedDeviceCount, NULL, 0);
    switch (device_class)
    {
    case vr::TrackedDeviceClass_Invalid:
      sprintf_s( buf, sizeof( buf ), "Invalid tracking result\n" );
      printf_s(buf);
      break;
    case vr::TrackedDeviceClass_HMD:
    {
      vr_system_->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, &pose, 1);

      VRControllerData& head_data = input_data.controller_data[kHead];
      head_data.pos = GetPosition(pose.mDeviceToAbsoluteTracking);
      head_data.rot = GetRotation(pose.mDeviceToAbsoluteTracking);
      head_data.vel = Vec3(pose.vVelocity.v[0], pose.vVelocity.v[1], pose.vVelocity.v[2]);
      head_data.angular_vel = Vec3(pose.vAngularVelocity.v[0], pose.vAngularVelocity.v[1], pose.vAngularVelocity.v[2]);
      head_data.is_connected = pose.bDeviceIsConnected;
      head_data.is_valid = pose.bPoseIsValid;
      result = pose.eTrackingResult;

      vr_system_->GetControllerState(device_id, &state, sizeof(vr::VRControllerState_t));
      head_data.last_button_pressed = head_data.button_pressed;
      head_data.button_pressed = state.ulButtonPressed;
      head_data.button_touched= state.ulButtonTouched;


      switch (result)
      {
      case vr::TrackingResult_Uninitialized:
        sprintf_s(buf, sizeof(buf), "Invalid tracking result\n");
        printf_s(buf);
        break;
      case vr::TrackingResult_Calibrating_InProgress:
        sprintf_s(buf, sizeof(buf), "Calibrating in progress\n");
        printf_s(buf);
        break;
      case vr::TrackingResult_Calibrating_OutOfRange:
        break;
        sprintf_s(buf, sizeof(buf), "Calibrating Out of range\n");
        printf_s(buf);
      case vr::TrackingResult_Running_OK:
        break;
      case vr::TrackingResult_Running_OutOfRange:
        sprintf_s(buf, sizeof(buf), "WARNING: Running Out of Range\n");
        printf_s(buf);
        break;
      default:
        break;
      }


    }
    case vr::TrackedDeviceClass_Controller:
    {
      vr_system_->GetControllerStateWithPose(vr::TrackingUniverseStanding, device_id, &state, sizeof(state), &pose);

      vr::ETrackedControllerRole index = vr_system_->GetControllerRoleForTrackedDeviceIndex(device_id);
      Controller type = kInvalid;
      if (index == vr::ETrackedControllerRole::TrackedControllerRole_LeftHand)
      {
        type = kLeftHand;
      }
      else if (index == vr::ETrackedControllerRole::TrackedControllerRole_RightHand)
      {
        type = kRightHand;
      }
      switch (index)
      {
      case vr::TrackedControllerRole_Invalid:
        //sprintf_s( buf, sizeof( buf ), "Invalid hand\n" );
        break;
      case vr::TrackedControllerRole_LeftHand:
      case vr::TrackedControllerRole_RightHand:
      {
        VRControllerData& controller_data = input_data.controller_data[type];
        controller_data.pos = GetPosition(pose.mDeviceToAbsoluteTracking);
        controller_data.rot = GetRotation(pose.mDeviceToAbsoluteTracking);
        controller_data.vel = Vec3(pose.vVelocity.v[0], pose.vVelocity.v[1], pose.vVelocity.v[2]);
        controller_data.angular_vel = Vec3(pose.vAngularVelocity.v[0], pose.vAngularVelocity.v[1], pose.vAngularVelocity.v[2]);
        controller_data.is_connected = pose.bDeviceIsConnected;
        controller_data.is_valid = pose.bPoseIsValid;
        controller_data.last_button_pressed = controller_data.button_pressed;
        controller_data.button_pressed = state.ulButtonPressed;
        controller_data.button_touched = state.ulButtonTouched;
        for (int i = 0; i < vr::k_unControllerStateAxisCount; ++i)
        {
          controller_data.axis[i] = Vec2(state.rAxis[i].x, state.rAxis[i].y);
        }
        switch (result)
        {
        case vr::TrackingResult_Uninitialized:
          sprintf_s(buf, sizeof(buf), "Invalid tracking result\n");
          printf_s(buf);
          break;
        case vr::TrackingResult_Calibrating_InProgress:
          sprintf_s(buf, sizeof(buf), "Calibrating in progress\n");
          printf_s(buf);
          break;
        case vr::TrackingResult_Calibrating_OutOfRange:
          sprintf_s(buf, sizeof(buf), "Calibrating Out of range\n");
          printf_s(buf);
          break;
        case vr::TrackingResult_Running_OK:
          break;
        case vr::TrackingResult_Running_OutOfRange:
          sprintf_s(buf, sizeof(buf), "WARNING: Running Out of Range\n");
          printf_s(buf);
          break;
        default:
          sprintf_s(buf, sizeof(buf), "Default\n");
          printf_s(buf);
          break;
        }
        break;
      }
      default:
        break;
      }
      break;
    }
    case vr::TrackedDeviceClass_GenericTracker:
      break;
    case vr::TrackedDeviceClass_TrackingReference:
      // incomplete code, only here for switch reference
      //printf_s( buf, sizeof( buf ), "Camera / Base Station" );
      break;
    case vr::TrackedDeviceClass_DisplayRedirect:
      break;
    default:
      break;
    }
  }
}
