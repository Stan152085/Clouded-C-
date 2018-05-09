#pragma once
class HexagonGrid;
class ITileState
{
public:
  ITileState(float wetness);
  virtual ITileState* Update(HexagonGrid* grid) = 0;
  virtual float CalculateWaterTransition(ITileState* other)=0;
  float wetness();
  void set_wetness(float wetness);
protected:
  float wetness_;
};
