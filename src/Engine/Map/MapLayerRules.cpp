// | -------------------------------
#include "MapLayerRules.hpp"
// | -------------------------------
#include "Engine/Map/MapData.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <algorithm>
#include <cstdint>
#include <exception>
#include <string>
// | -------------------------------

namespace ENG
{
  MapLayerRule ResolveLayerRule(const TileLayerData& layer, const MapLayerRules& rules)
  {
    MapLayerRule rule;

    // 1) Class, then name
    auto it = layer.m_class.empty() ? rules.end() : rules.find(layer.m_class);
    if (it == rules.end())
      it = rules.find(layer.m_name);

    if (it != rules.end())
      rule = it->second;
    else
      LOG_INFO(" | << Map: no rule for layer '" + layer.m_name +
               (layer.m_class.empty() ? std::string{} : "' (class '" + layer.m_class + "')") +
               "', using defaults");

    // 2) Per-layer overrides from the custom properties
    if (HasProperty(layer.m_properties, "drawLayer"))
    {
      try
      {
        rule.m_drawLayer = static_cast<uint8_t>(
          std::clamp(std::stoi(layer.m_properties.at("drawLayer")), 0, 255));
      }
      catch (const std::exception&)
      {
        LOG_ERROR(" | << Map: invalid 'drawLayer' in layer '" + layer.m_name + "'");
      }
    }
    if (HasProperty(layer.m_properties, "collision"))
      rule.m_collision = layer.m_properties.at("collision") == "true";
    if (HasProperty(layer.m_properties, "render"))
      rule.m_render = layer.m_properties.at("render") == "true";

    // 3) Hidden in the editor -> never drawn (it can still collide)
    rule.m_render = rule.m_render && layer.m_visible;
    return rule;
  }
}
