// | -------------------------------
#pragma once
// | -------------------------------
#include "AssetsManager.hpp"
// | -------------------------------
#include "Engine/Services/CSVManager.hpp"
#include "Engine/Services/ClockManager.hpp"
#include "Engine/Services/FontManager.hpp"
#include "Engine/Services/MusicManager.hpp"
#include "Engine/Services/SFXManager.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/ShaderManager.hpp"
#include "Engine/Services/WorldSaver.hpp"
// | -------------------------------

namespace ENG
{
  /**
   * @brief Global access point to the engine's managers (service locator).
   *
   * Services lets any part of the engine or the game reach a manager
   * without receiving it as a parameter. Each manager is registered once
   * with its `Provide*` function and then retrieved with the matching
   * accessor.
   *
   * The Services class does not own the managers: it only stores pointers
   * to them. The registered managers must outlive every use of the
   * accessors.
   *
   * Usage:
   * @code
   * ENG::Services::ProvideAssets(&assets);
   * ENG::Services::Assets().Load("player.png", "player");
   * @endcode
   *
   * @warning Each accessor dereferences its pointer without checking it.
   *          Calling an accessor before the matching `Provide*` function
   *          (or after providing nullptr) is undefined behavior.
   *
   * @note The `Provide*` functions of the assets, sound effects, music and
   *       clock managers also call Init() on the manager they receive
   *       (unless it is nullptr). The other managers are stored as they are.
   */
  class Services
  {
    public:
      /**
       * @brief Returns the assets manager.
       * @return Reference to the registered AssetsManager.
       */
      static AssetsManager& Assets() {return *m_assets;}
      /**
       * @brief Registers the assets manager and initializes it.
       *
       * @param[in] _ Manager to register. Passing nullptr unregisters it
       *              (no initialization is done).
       */
      static void ProvideAssets(AssetsManager* _)
      {
        m_assets = _;
        if(m_assets)
          m_assets->Init();
      }
      /**
       * @brief Returns the fonts manager.
       * @return Reference to the registered FontManager.
       */
      static FontManager& Fonts() {return *m_fonts;}
      /**
       * @brief Registers the fonts manager.
       *
       * @param[in] _ Manager to register. Passing nullptr unregisters it.
       */
      static void ProvideFonts(FontManager* _) {m_fonts = _;}
      /**
       * @brief Returns the scenes manager.
       * @return Reference to the registered ScenesManager.
       */
      static ScenesManager& Scenes() {return *m_scenes;}
      /**
       * @brief Registers the scenes manager.
       *
       * @param[in] _ Manager to register. Passing nullptr unregisters it.
       */
      static void ProvideScenes(ScenesManager* _) {m_scenes = _; }
      /**
       * @brief Returns the shaders manager.
       * @return Reference to the registered ShaderManager.
       */
      static ShaderManager& Shaders() {return *m_shaders;}
      /**
       * @brief Registers the shaders manager.
       *
       * @param[in] _ Manager to register. Passing nullptr unregisters it.
       */
      static void ProvideShaders(ShaderManager* _) {m_shaders = _;}
      /**
       * @brief Returns the sound effects manager.
       * @return Reference to the registered SFXManager.
       */
      static SFXManager& SFX() {return *m_sfx;}
      /**
       * @brief Registers the sound effects manager and initializes it.
       *
       * @param[in] _ Manager to register. Passing nullptr unregisters it
       *              (no initialization is done).
       */
      static void ProvideSFX(SFXManager* _)
      {
        m_sfx = _;
        if(m_sfx)
          m_sfx->Init();
      }
      /**
       * @brief Returns the music manager.
       * @return Reference to the registered MusicManager.
       */
      static MusicManager& Music() {return *m_music;}
      /**
       * @brief Registers the music manager and initializes it.
       *
       * @param[in] _ Manager to register. Passing nullptr unregisters it
       *              (no initialization is done).
       */
      static void ProvideMusic(MusicManager* _)
      {
        m_music = _;
        if(m_music)
          m_music->Init();
      }
      /**
       * @brief Returns the world saver.
       * @return Reference to the registered WorldSaver.
       */
      static WorldSaver& WSaver() {return *m_wSaver;}
      /**
       * @brief Registers the world saver.
       *
       * @param[in] _ World saver to register. Passing nullptr unregisters it.
       */
      static void ProvideWorldSaver(WorldSaver* _)
      {
        m_wSaver = _;
      }
      /**
       * @brief Returns the clock manager.
       * @return Reference to the registered ClockManager.
       */
      static ClockManager& Clock() {return *m_clock;}
      /**
       * @brief Registers the clock manager and initializes it.
       *
       * @param[in] _ Manager to register. Passing nullptr unregisters it
       *              (no initialization is done).
       */
      static void ProvideClockSaver(ClockManager* _)
      {
        m_clock = _;
        if(m_clock)
            m_clock->Init();
      }
      /**
       * @brief Returns the CSV manager.
       * @return Reference to the registered CSVManager.
       */
      static CSVManager& CSV() {return *m_csv;}
      /**
       * @brief Registers the CSV manager.
       *
       * @param[in] _ Manager to register. Passing nullptr unregisters it.
       */
      static void ProvideCSVManager(CSVManager* _)
      {
        m_csv = _;
      }
    private:
      /// @brief Registered assets manager. Not owned. nullptr until provided.
      static inline AssetsManager*  m_assets  = nullptr;
      /// @brief Registered shaders manager. Not owned. nullptr until provided.
      static inline ShaderManager*  m_shaders = nullptr;
      /// @brief Registered fonts manager. Not owned. nullptr until provided.
      static inline FontManager*    m_fonts   = nullptr;
      /// @brief Registered scenes manager. Not owned. nullptr until provided.
      static inline ScenesManager*  m_scenes  = nullptr;
      /// @brief Registered music manager. Not owned. nullptr until provided.
      static inline MusicManager*   m_music   = nullptr;
      /// @brief Registered sound effects manager. Not owned. nullptr until provided.
      static inline SFXManager*     m_sfx     = nullptr;
      /// @brief Registered world saver. Not owned. nullptr until provided.
      static inline WorldSaver*     m_wSaver  = nullptr;
      /// @brief Registered clock manager. Not owned. nullptr until provided.
      static inline ClockManager*   m_clock   = nullptr;
      /// @brief Registered CSV manager. Not owned. nullptr until provided.
      static inline CSVManager*     m_csv     = nullptr;
  };
}
