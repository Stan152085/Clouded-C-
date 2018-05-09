#pragma once
class HexagonGrid;
class HexagonTile;
class ITileState
{
public:
  enum State
  {
    Water,
    Swamp,
    Grass,
    Savannah,
    Desert,
    Mountain
  };
  ITileState(float wetness);
  virtual ITileState* Update(HexagonGrid* grid, HexagonTile* tile ) = 0;
  virtual float CalculateWaterTransition(ITileState* other)=0;
  float wetness();
  void set_wetness(float wetness);
  State Type();
protected:
  float wetness_;
};
