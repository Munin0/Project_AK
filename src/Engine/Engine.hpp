// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Layer/GameLayer.hpp"
#include "Engine/Services/AssetsManager.hpp"
#include "Engine/Services/CSVManager.hpp"
#include "Engine/Services/ClockManager.hpp"
#include "Engine/Services/FontManager.hpp"
#include "Engine/Services/MusicManager.hpp"
#include "Engine/Services/SFXManager.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/ShaderManager.hpp"
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Utils/Config.hpp"
// | -------------------------------
#include <memory>
#include <string>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Main class of the engine.
   *
   * Engine ties everything together: it owns the game layer and all the
   * engine subsystems (assets, scenes, fonts, audio, shaders, world saving,
   * clock and CSV data), and drives their lifecycle through the On* functions.
   *
   * Expected lifecycle:
   * -# Construct the Engine with an EngineConfig and a GameLayer.
   * -# Call OnInit() to initialize the engine.
   * -# Call OnRun() to run it.
   * -# Call OnDestroy() when the engine is closing.
   *
   * @note The Engine does not own the EngineConfig it receives; the caller
   *       must keep it alive for as long as the Engine exists. The
   *       GameLayer, on the other hand, is owned by the Engine.
   */
  class Engine
  {
    public:
      /**
       * @brief Creates the engine.
       *
       * @param[in] eConfig Configuration of the engine (window title,
       *                    resolution, etc.). It is stored by pointer, not
       *                    copied, so it must outlive the Engine.
       * @param[in] layer   The game layer that connects the game with the
       *                    engine. Ownership is transferred to the Engine.
       */
      Engine(EngineConfig& eConfig, std::unique_ptr<GameLayer> layer);
      /**
       * @brief Default destructor.
       *
       * @note It does not call OnDestroy(); that function must be called
       *       explicitly when the engine is closing.
       */
      ~Engine() = default;
      /**
       * @brief Initializes the engine.
       *
       * Applies the initial configuration and prepares the engine to run.
       *
       * @return true if the initialization succeeded, false otherwise.
       */
      bool OnInit(void);
      /**
       * @brief Runs the engine.
       *
       * @return true if the engine ran successfully, false otherwise.
       */
      bool OnRun(void);
      /**
       * @brief Saves all the engine information.
       */
      void OnSave(void);
      /**
       * @brief Destroys everything when the engine is closing.
       *
       * Releases the resources held by the engine and its subsystems.
       */
      void OnDestroy(void);
    private:
      /// @brief Title of the window viewport.
      std::string m_engineTitle  = "";
      /// @brief Width of the window viewport, in pixels.
      int         m_engineWidth = 0;
      /// @brief Height of the window viewport, in pixels.
      int         m_engineHeight = 0;
      /// @brief Configuration of the engine. Not owned by the Engine.
      EngineConfig*                 m_eConfig = nullptr;
      /// @brief Connection to the game. Owned by the Engine.
      std::unique_ptr<GameLayer>    m_game;
      /// @brief Manager of the game assets.
      AssetsManager   m_eAssets;
      /// @brief Manager of the game scenes.
      ScenesManager   m_eScenes;
      /// @brief Manager of the fonts.
      FontManager     m_eFont;
      /// @brief Manager of the sound effects.
      SFXManager      m_eSFX;
      /// @brief Manager of the music.
      MusicManager    m_eMusic;
      /// @brief Manager of the shaders.
      ShaderManager   m_eShaders;
      /// @brief Saves and restores the state of the world.
      WorldSaver      m_eWorldSaver;
      /// @brief Manager of the engine clock and timing.
      ClockManager    m_eClock;
      /// @brief Manager of the CSV files.
      CSVManager      m_eCSV;
  };
}
