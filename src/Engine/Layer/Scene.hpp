// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
// | -------------------------------
#include <memory>
#include <string>
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
   * Scene is only the interface plus the data every scene has: its objects
   * (m_pool), its main camera (m_camera) and its ID. Everything else is
   * composed by the scene that needs it:
   * - Run the objects: ENG::UpdateObjects / DispatchPointer / RenderObjects
   *   on m_pool (Engine/Object/PoolSystems.hpp).
   * - A map: an ENG::SceneMap member (Engine/Map/SceneMap.hpp).
   *
   * Scenes are stored and switched by the ScenesManager, using the ID
   * returned by GetSceneID(). The batcher camera is cleared on every scene
   * change; Init() activates m_camera (or another) if the scene uses one.
   *
   * Usage:
   * @code
   * class MenuScene : public ENG::Scene
   * {
   *   public:
   *     void Inputs(float dt) override        { ENG::DispatchPointer(m_pool); }
   *     void Update(float dt) override        { ENG::UpdateObjects(m_pool, dt); }
   *     void Render(ENG::Batcher& b) override { ENG::RenderObjects(m_pool, b); }
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
       * Override it to release what the scene created (usually m_pool.Clear()).
       * It may be called twice (see ScenesManager::DestroyScene()).
       */
      virtual void Destroy(void){};

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
      std::string GetSceneID(void) const
      {
        return this->m_sceneId;
      }

    protected:
      /// @brief Objects of the scene. Owned by the scene.
      ObjectPool m_pool;

      /// @brief Main camera of the scene. Owned by the scene. nullptr if it has none.
      std::unique_ptr<Camera2D> m_camera;

      /// @brief Identifier of the scene. Returned by GetSceneID().
      std::string m_sceneId;

      /// @brief Whether the scene has been initialized. Defaults to false.
      bool m_isInit = false;
  };
}
