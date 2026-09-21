// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Layer/Scene.hpp"
// | -------------------------------
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
// | -------------------------------

namespace ENG
{
  /// @brief Numeric identifier of a scene. It can hold values from 0 to 255.
  using SceneID = uint8_t;
  /**
   * @brief Identifiers of the scenes of the game.
   */
  enum : SceneID
  {
    SCENE_MENU = 0,     ///< Main menu scene.
    SCENE_LEVEL_2 = 3,  ///< Second level scene.
    SCENE_DEMO = 100,   ///< Demo scene.
  };
  /**
   * @brief Stores the scenes of the game and controls which one is active.
   *
   * ScenesManager owns every Scene it receives, keeps them indexed by
   * SceneID and tracks the current scene. Scene changes are deferred: a
   * change is first requested with PeddingScene() and applied later with
   * ChangeScene().
   */
  class ScenesManager
  {
    public:
      /// @brief Default constructor.
      ScenesManager() = default;
      /// @brief Default destructor.
      ~ScenesManager() = default;
      /**
       * @brief Adds a scene to the manager.
       *
       * The manager takes ownership of the scene and stores it under its ID.
       *
       * @param[in] _ Scene to store. Ownership is transferred to the manager.
       */
      void AddScene(std::unique_ptr<Scene> _);
      /**
       * @brief Returns the current scene.
       *
       * @return Pointer to the scene that is currently active, or nullptr if
       *         there is none. The scene is owned by the manager.
       */
      Scene* GetCurrent();
      /**
       * @brief Applies the pending scene change.
       *
       * If a change was requested with PeddingScene(), the pending scene
       * becomes the current scene. Otherwise it does nothing.
       */
      void ChangeScene(void);
      /**
       * @brief Requests a change to another scene.
       *
       * The change is not applied immediately: it is applied the next time
       * ChangeScene() is called.
       *
       * @param[in] idPedding ID of the scene to change to.
       */
      void PeddingScene(SceneID idPedding);
      /**
       * @brief Destroys a scene.
       *
       * @param[in] id ID of the scene to destroy.
       */
      void DestroyScene(SceneID id);
      /**
       * @brief Destroys every scene stored in the manager.
       */
      void Clear(){m_scenesMap.clear();}
    private:
      /// @brief Current scene. nullptr if there is none. Owned by m_scenesMap.
      Scene* m_s_current = nullptr;
      /// @brief ID of the scene requested by PeddingScene().
      SceneID m_pedding = 1;
      /// @brief Whether a scene change is waiting to be applied by ChangeScene().
      bool m_arePedding = false;
      /// @brief Scenes owned by the manager, indexed by ID.
      std::map<SceneID, std::unique_ptr<Scene>, std::less<>> m_scenesMap;
  };
}
