// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
// | -------------------------------
#include "nlohmann/json.hpp"
#include "nlohmann/detail/macro_scope.hpp"
// | -------------------------------
#include <cstdint>
// | -------------------------------

namespace APP
{
  /**
   * @brief Gameplay stats of an object (RPG-style). Optional component.
   *
   * Every stat is one byte (0 to 255). It is saved and loaded by the engine
   * through ENG::ComponentSerializers under the key "stats" (registered in
   * Game::OnInit()).
   */
  class IStats : public ENG::IComponent
  {
    public:
      /// @brief Current health points.
      uint8_t m_hp     = 0;
      /// @brief Maximum health points.
      uint8_t m_hp_max = 0;
      /// @brief Strength.
      uint8_t m_str    = 0;
      /// @brief Defense.
      uint8_t m_def    = 0;
      /// @brief Agility.
      uint8_t m_agi    = 0;

      /// @brief JSON conversion; missing keys keep their default value.
      NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(IStats, m_hp, m_hp_max, m_str, m_def, m_agi)
  };
}
