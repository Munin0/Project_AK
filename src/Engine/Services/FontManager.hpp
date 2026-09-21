// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Services/KeyHash.hpp"
#include "Engine/Text/Font/Font.hpp"
// | -------------------------------
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Stores the fonts loaded by the engine.
   *
   * FontManager owns every Font it receives and lets the rest of the engine
   * retrieve them by name, so a font is loaded once and shared.
   *
   * Usage:
   * @code
   * fonts.LoadFont(std::make_unique<ENG::Font>("roboto"));
   * ENG::Font* font = fonts.GetFont("roboto");
   * @endcode
   */
  class FontManager
  {
    public:
      /// @brief Default constructor.
      FontManager() = default;
      /// @brief Default destructor.
      ~FontManager() = default;
      /**
       * @brief Adds a font to the manager.
       *
       * The manager takes ownership of the font and stores it under the
       * name returned by Font::GetName().
       *
       * @param[in] font Font to store. Ownership is transferred to the manager.
       */
      void LoadFont(std::unique_ptr<Font> font);
      /**
       * @brief Returns a stored font by name.
       *
       * @param[in] fontName Name of the font, as given by Font::GetName().
       * @return Pointer to the font, or nullptr if no font with that name
       *         has been loaded. The font is owned by the manager and the
       *         pointer becomes invalid after Clear().
       */
      Font* GetFont(const std::string& fontName);
      /**
       * @brief Destroys every stored font.
       *
       * Any pointer previously returned by GetFont() becomes invalid after
       * this call.
       */
      void Clear(void);
    private:
      /// @brief Stored fonts, indexed by font name.
      std::unordered_map<std::string, std::unique_ptr<Font>, StringHash, std::equal_to<>> m_fonts;
  };
}
