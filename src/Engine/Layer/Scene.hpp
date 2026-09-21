// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/TileMap/TileMap.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
// | -------------------------------
#include <cstdint>
#include <memory>
#include <vector>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Base class of every scene of the game.
   *
   * A scene is a self-contained part of the game (a menu, a level, a demo)
   * with its own lifecycle: it is initialized, receives inputs and updates
   * every frame, is rendered, and is destroyed when it is no longer needed.
   * A game derives from Scene and overrides the hooks it needs. Only
   * Render() is mandatory.
   *
   * A Scene can also hold tile maps, kept sorted by layer, and a camera.
   *
   * Scenes are stored and switched by the ScenesManager, using the ID
   * returned by GetSceneID().
   *
   * Usage:
   * @code
   * class MenuScene : public ENG::Scene
   * {
   *   public:
   *     void Render(ENG::Batcher& b) override { }
   * };
   * @endcode
   */
  class Scene
  {
    public:
      /// @brief Virtual destructor, so derived scenes are destroyed correctly.
      virtual ~Scene() = default;

      /**
       * @brief Called when the scene is initialized.
       *
       * Override it to create the objects and load the resources of the scene.
       */
      virtual void Init(void){};

      /**
       * @brief Called when the scene is destroyed.
       *
       * Override it to release what the scene created.
       */
      virtual void Destroy(void){};

      /**
       * @brief Tells whether the scene is running.
       * @return The value of the isRunning flag.
       */
      virtual bool IsRunning(void)  {return isRunning;}

      /**
       * @brief Called to process the inputs of the scene.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      virtual void Inputs(float dt){(void)dt;};

      /**
       * @brief Called once per frame to update the scene.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      virtual void Update(float dt){(void)dt;};

      /**
       * @brief Called at a fixed time step to update the scene.
       *
       * Meant for logic that needs a constant time step, such as physics.
       *
       * @param[in] dt Duration of the fixed time step, in seconds.
       */
      virtual void UpdateFixed(float dt){(void)dt;};

      /**
       * @brief Draws the scene.
       *
       * Every scene must implement it.
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      virtual void Render(ENG::Batcher& b) = 0;

      /**
       * @brief Returns the ID of the scene.
       * @return The identifier of the scene.
       */
      uint8_t GetSceneID(void) const
      {
        return this->sceneID;
      }

    protected:
      /**
       * @brief Adds a tile map to the scene.
       *
       * Keeps the tile maps sorted by TileMap::GetLayer(), in ascending order.
       *
       * @param[in] map Tile map to add.
       */
      void AddTileMap(TileMap map);

      /**
       * @brief Draws the tile maps that belong to a range of layers.
       *
       * Draws the tile maps whose layer is in [minLayer, maxLayer), that is,
       * including @p minLayer and excluding @p maxLayer. This lets a scene
       * interleave tile map draws with its ObjectPool render queue by layer
       * group.
       *
       * @param[in,out] b        Batcher that collects the draw data of this frame.
       * @param[in]     minLayer First layer to draw (included).
       * @param[in]     maxLayer Layer where the range ends (excluded).
       */
      void RenderTileMaps(Batcher& b, uint8_t minLayer, uint8_t maxLayer) const;

      /// @brief Whether the scene is running. Returned by IsRunning(). Defaults to false.
      bool isRunning = false;

      /// @brief Whether the scene has been initialized. Defaults to false.
      bool isInit = false;

      /// @brief Identifier of the scene. Returned by GetSceneID().
      uint8_t sceneID;

      /// @brief Camera of the scene. Owned by the scene.
      std::unique_ptr<Camera2D> camera;

      /// @brief Tile maps of the scene, sorted by layer in ascending order.
      std::vector<TileMap> tileMaps;
  };
}
