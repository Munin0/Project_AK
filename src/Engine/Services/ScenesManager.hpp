// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Layer/Scene.hpp"
#include "Engine/Services/KeyHash.hpp"
// | -------------------------------
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Stores the scenes of the game and controls which one is active.
   *
   * ScenesManager owns every Scene it receives, keeps them indexed by their
   * scene ID (Scene::GetSceneID()) and tracks the current scene.
   *
   * Scene changes are deferred, so a scene never destroys itself in the
   * middle of its own Update():
   * 1. Any code requests a change with SetPedingScene("<id>").
   * 2. On the next update, the GameLayer applies it with ChangeScene().
   *    Only if it succeeds does the GameLayer destroy the old scene and
   *    initialize the new one:
   * @code
   * if (scenes.HasPedingScene())
   * {
   *   ENG::Scene* old = scenes.GetCurrent();
   *   if (scenes.ChangeScene())          // validates the ID and clears the request
   *   {
   *     old->Destroy();
   *     scenes.GetCurrent()->Init();
   *   }
   * }
   * @endcode
   *
   * Changing scene never deletes a Scene object: it stays in the manager
   * until Clear(), so pointers to scenes stay valid across changes.
   */
  class ScenesManager
  {
    public:
      /// @brief Default constructor.
      ScenesManager() = default;

      /// @brief Default destructor. Destroys every scene it owns.
      ~ScenesManager() = default;

      /**
       * @brief Adds a scene to the manager.
       *
       * The manager takes ownership of the scene and stores it under its ID.
       * The first scene added becomes the current scene (the starting scene).
       * If a scene with the same ID already exists, an error is logged and
       * @p _ is discarded.
       *
       * @param[in] _ Scene to store. Ownership is transferred to the manager.
       */
      void AddScene(std::unique_ptr<Scene> _);

      /**
       * @brief Returns the current scene.
       *
       * @return Pointer to the active scene, owned by the manager. If no scene
       *         was added it asserts in Debug and returns nullptr in Release.
       */
      Scene* GetCurrent();

      /**
       * @brief Returns the ID of the requested scene.
       *
       * @return The ID passed to SetPedingScene(), or an empty string if there
       *         is no pending change (ChangeScene() clears it).
       */
      const std::string& GetPedingScene(void);

      /**
       * @brief Applies the pending scene change.
       *
       * If the pending ID exists, it becomes the current scene and the
       * batcher camera is reset (SetCamera2D(nullptr)). This is the only place
       * that clears the batcher camera: the new scene's Init() activates its
       * own camera or leaves it null (screen pixels). It only switches the
       * pointer: it does not call Destroy() on the old scene nor Init() on the
       * new one, that is the caller's job (see the class example).
       *
       * The pending request is always cleared, also when it fails, so an
       * invalid ID is reported once and never retried.
       *
       * @return true if the scene changed; false if there was no pending
       *         change or the ID is not in the manager (an error is logged).
       *         On false the current scene is untouched.
       */
      bool ChangeScene(void);

      /**
       * @brief Requests a change to another scene.
       *
       * The change is not applied immediately: it is applied the next time
       * ChangeScene() is called (the next update). The ID is not checked here;
       * ChangeScene() validates it. Requesting the same ID that is already
       * pending is ignored. Requesting the current scene's ID restarts it
       * (Destroy() + Init()).
       *
       * @param[in] id ID of the scene to change to.
       */
      void SetPedingScene(const std::string& id);

      /**
       * @brief Whether a scene change is waiting to be applied.
       *
       * @return true from SetPedingScene() until the next ChangeScene().
       */
      bool HasPedingScene(void) const;

      /**
       * @brief Removes a scene from the manager for good.
       *
       * Calls Destroy() on the scene and deletes it. Use it for scenes you will
       * not go back to (splash, credits, a finished level) or to rebuild one
       * from its constructor (DestroyScene() + AddScene() + SetPedingScene()).
       * If the pending change points to this scene, the request is cancelled.
       *
       * Destroy() is called even if the scene was already destroyed (the scene
       * left behind by a change already went through it), so Destroy() must
       * be safe to call twice.
       *
       * @param[in] id ID of the scene to destroy.
       * @return false if the ID is not in the manager or is the current scene
       *         (an error is logged and nothing changes); true otherwise.
       *
       * @warning The current scene can't be destroyed, so a scene can't
       *          destroy itself: change to another scene first and destroy it
       *          from there.
       */
      bool DestroyScene(const std::string& id);

      /**
       * @brief Destroys every scene stored in the manager.
       *
       * Does not call Scene::Destroy(), and leaves the current scene pointer
       * dangling: use it only at shutdown.
       */
      void Clear(){m_scenesMap.clear();}

      /**
       * @brief Checks whether a scene with this ID is in the manager.
       *
       * @param[in] id Scene ID to look for.
       * @return true if the scene exists.
       */
      bool GetCheckScene(const std::string& id);

    private:
      /// @brief Current scene. nullptr if there is none. Owned by m_scenesMap.
      Scene* m_s_current = nullptr;
      /// @brief ID of the scene requested by SetPedingScene(). Empty if none.
      std::string m_peding = "";
      /// @brief Whether a scene change is waiting to be applied by ChangeScene().
      bool m_hasPeding = false;
      /// @brief Scenes owned by the manager, indexed by ID.
      std::unordered_map<std::string, std::unique_ptr<Scene>, StringHash, std::equal_to<>> m_scenesMap;
  };
}
