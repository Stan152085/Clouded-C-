#pragma once
#include "Core/Transform.h"
namespace resources
{
   class AssetManager;
   class Model;
}
class HexagonGrid;
class HexagonTile;
class D3D11Renderer;
class IObject
{
public:
   enum Type
   {
      kBuilding,
      kCity,
      kFire,
      kForest,
      kTree,
   };
public:
   static void Initialize( resources::AssetManager& asset_manager );
   static IObject* Create( Type type );
   
   IObject( const char* model );
   virtual bool Update( HexagonGrid* grid, HexagonTile* tile ) = 0;
   virtual void Draw( D3D11Renderer& gfx, Vec3& position ) = 0;

protected:
   Transform transform;
   std::shared_ptr<resources::Model> model_;
   static resources::AssetManager* asset_manager_;
};