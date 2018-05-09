#pragma once
class Keyboard
{
  public:
  static constexpr int num_keys = 256;
  bool states[num_keys];
};