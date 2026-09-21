// | -------------------------------
#pragma once
// | -------------------------------
#include "nlohmann/json_fwd.hpp"
#include "nlohmann/json.hpp"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Basic engine/window configuration that can be saved to and loaded from JSON.
   *
   * EngineConfig holds the settings needed to create the application window:
   * title, resolution and fullscreen mode. It can be serialized with Save()
   * and restored with Load().
   *
   * JSON layout:
   * @code
   * {
   *   "name": "My Game",
   *   "screen": { "w": 1280, "h": 720 },
   *   "fullscreen": false
   * }
   * @endcode
   */
  class EngineConfig
  {
    public:
      /// @brief Default constructor.
      EngineConfig() = default;
      /// @brief Default destructor.
      ~EngineConfig() = default;
      /**
       * @brief Serializes this configuration to JSON.
       *
       * @return A JSON object with the keys "name", "screen" (with "w" and
       *         "h") and "fullscreen".
       */
      nlohmann::json Save()
      {
        return {
          { "name",   m_title },
          { "screen",{{ "w", m_viewWidth  },{ "h", m_viewHeight } } },
          { "fullscreen",  m_eFullscreen }
        };
      }
      /**
       * @brief Loads this configuration from JSON.
       *
       * Missing keys are not an error: any value that is not present in the
       * JSON keeps its current value. The "screen" entry is only read if it
       * exists and is a JSON object.
       *
       * @param[in] j JSON object with the configuration, in the layout
       *              produced by Save().
       */
      void Load(const nlohmann::json& j)
      {
        m_title = j.value("name", m_title);
        if (auto it = j.find("screen"); it != j.end() && it->is_object())
        {
          m_viewWidth  = it->value("w", m_viewWidth);
          m_viewHeight = it->value("h", m_viewHeight);
        }
        m_eFullscreen = j.value("fullscreen", m_eFullscreen);
      }
    public:
      /// @brief Title of the window.
      std::string   m_title      = "";
      /// @brief Width of the screen (window), in pixels.
      unsigned int  m_viewWidth  = 0;
      /// @brief Height of the screen (window), in pixels.
      unsigned int  m_viewHeight = 0;
      /// @brief Whether the window runs in fullscreen mode.
      bool m_eFullscreen;
  };
}
