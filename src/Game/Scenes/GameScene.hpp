/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/GUI/Label.hpp"
#include "Engine/Layer/Scene.hpp"
#include "Engine/Map/MapData.hpp"
#include "Engine/Map/SceneMap.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include "Game/Dialog/DialogRunner.hpp"
/// | ------------------------------------ |
#include <string>
#include <vector>
/// | ------------------------------------ |

namespace APP
{
  /**
   * @brief Game scene, where everything is tested: a player on the map.
   *
   * Map pipeline: Tiled -> IMP::TiledLoader -> MapData -> SceneMap::Load(GameLayerRules())
   * -> TileMap (draw) + CollisionGrid (m_map.GetCollision()).
   *
   * Controls:
   * - WASD / arrows: move the player. It collides with the solid cells.
   * - G: show / hide the collision cells.
   * - E: start the test dialog / advance it (the player does not move while it is open).
   * - Backspace: back to the main menu.
   */
  class GameScene : public ENG::Scene
  {
    public:
      GameScene(const std::string& id);

      // | ====================================================== |
      // | Scene interface                                        |
      // | ====================================================== |
      void Init(void)               override;
      void Destroy(void)            override;
      void Inputs(float dt)         override;
      void Update(float dt)         override;
      void UpdateFixed(float dt)    override;
      void Render(ENG::Batcher& b)  override;

    private:
      // | ====================================================== |
      // | Own methods of the scene                               |
      // | ====================================================== |
      /// @brief Loads the atlas and the map into the scene.
      bool LoadLevel();

      /// @brief Places the player on the spawn point of the map (see FindSpawnPoint()).
      void SpawnPlayer();

      /// @brief Creates the dialog box: a hidden Label in the pool (Screen space, UI layer).
      /// @return Its ObjectID, or NULL_ID if the font is missing.
      ENG::ObjectID CreateDialogBox();

    private:
      /// @brief The level: tile layers + collision grid.
      ENG::SceneMap                   m_map;
      std::vector<ENG::MapObjectData> m_objects;
      ENG::ObjectID                   m_playerId      = ENG::NULL_ID;
      ENG::Vector2                    m_moveDir{0.0f, 0.0f};
      bool                            m_showCollision = false;

      /// @brief Runs the conversations; its box is a Label in m_pool.
      DialogRunner                    m_dialog;
  };
}
