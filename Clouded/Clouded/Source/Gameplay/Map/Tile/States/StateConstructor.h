#pragma once
class ITileState;
class StateConstructor
{
public:
  static void Initialize();
  static ITileState* Create(float wetness);
};