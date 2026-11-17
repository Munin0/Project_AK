// | -------------------------------
#include "TiledLoader.hpp"
// | -------------------------------
#include "Engine/Map/MapData.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
// | -------------------------------
#include <algorithm>
#include <cstddef>
#include <exception>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>
// | -------------------------------

namespace IMP
{
  namespace
  {
    /// Tiled stores flip / rotation flags in the 4 highest bits of a gid.
    constexpr uint32_t GID_FLAGS_MASK = 0xF0000000u;

    struct TilesetRange
    {
      int      m_index;     // index into MapData::m_tilesets
      uint32_t m_firstGid;
    };

    struct LoadContext
    {
      std::string               m_path;
      std::vector<TilesetRange> m_ranges;   // sorted by firstgid
      bool                      m_warnedFlip = false;
    };

    std::string ValueToString(const nlohmann::json& v)
    {
      if (v.is_string())  return v.get<std::string>();
      if (v.is_boolean()) return v.get<bool>() ? "true" : "false";
      return v.dump();
    }

    ENG::MapProperties ReadProperties(const nlohmann::json& j)
    {
      ENG::MapProperties props;
      if (!j.contains("properties") || !j.at("properties").is_array())
        return props;

      for (const auto& p : j.at("properties"))
      {
        if (p.contains("name") && p.contains("value"))
          props[p.at("name").get<std::string>()] = ValueToString(p.at("value"));
      }
      return props;
    }

    /// Returns the range a gid belongs to, or nullptr if it is below every firstgid.
    const TilesetRange* FindRange(const LoadContext& ctx, uint32_t gid)
    {
      const TilesetRange* found = nullptr;
      for (const auto& r : ctx.m_ranges)
      {
        if (r.m_firstGid <= gid) found = &r;
        else break;
      }
      return found;
    }

    void ReadTileLayer(const nlohmann::json& lj, const glm::vec2& parentOffset, bool parentVisible,
                       LoadContext& ctx, ENG::MapData& out)
    {
      ENG::TileLayerData layer;
      layer.m_name       = lj.value("name", std::string{});
      layer.m_class      = lj.value("class", std::string{});   // Tiled 1.9+
      layer.m_visible    = parentVisible && lj.value("visible", true);
      layer.m_offset     = parentOffset + glm::vec2{lj.value("offsetx", 0.0f), lj.value("offsety", 0.0f)};
      layer.m_properties = ReadProperties(lj);

      if (lj.contains("chunks"))
      {
        LOG_ERROR(" | << TiledLoader: infinite maps are not supported (layer '" + layer.m_name +
                  "' in " + ctx.m_path + ")");
        return;
      }
      if (!lj.contains("data") || !lj.at("data").is_array())
      {
        LOG_ERROR(" | << TiledLoader: layer '" + layer.m_name + "' has no CSV/JSON data "
                  "(base64/compressed is not supported) in " + ctx.m_path);
        return;
      }

      layer.m_width  = lj.value("width", out.m_width);
      layer.m_height = lj.value("height", out.m_height);

      const auto& data = lj.at("data");
      layer.m_tiles.assign(data.size(), ENG::EMPTY_TILE);

      int  layerTileset  = -1;
      bool warnedMixed   = false;
      for (size_t i = 0; i < data.size(); i++)
      {
        uint32_t gid = data[i].get<uint32_t>();
        if (gid & GID_FLAGS_MASK)
        {
          gid &= ~GID_FLAGS_MASK;
          if (!ctx.m_warnedFlip)
          {
            LOG_INFO(" | << TiledLoader: flipped/rotated tiles are drawn unflipped (" + ctx.m_path + ")");
            ctx.m_warnedFlip = true;
          }
        }
        if (gid == 0)
          continue;

        const TilesetRange* range = FindRange(ctx, gid);
        if (!range)
          continue;

        if (layerTileset < 0)
          layerTileset = range->m_index;

        if (range->m_index != layerTileset)
        {
          if (!warnedMixed)
          {
            LOG_ERROR(" | << TiledLoader: layer '" + layer.m_name + "' mixes tilesets; only '" +
                      out.m_tilesets[layerTileset].m_name + "' is kept (" + ctx.m_path + ")");
            warnedMixed = true;
          }
          continue;
        }

        layer.m_tiles[i] = static_cast<int32_t>(gid - range->m_firstGid);
      }

      layer.m_tileset = std::max(layerTileset, 0);
      out.m_layers.push_back(std::move(layer));
    }

    void ReadObjectLayer(const nlohmann::json& lj, const glm::vec2& parentOffset, ENG::MapData& out)
    {
      if (!lj.contains("objects"))
        return;

      const std::string layerName = lj.value("name", std::string{});
      const glm::vec2 offset = parentOffset + glm::vec2{lj.value("offsetx", 0.0f), lj.value("offsety", 0.0f)};

      for (const auto& oj : lj.at("objects"))
      {
        ENG::MapObjectData obj;
        obj.m_name  = oj.value("name", std::string{});
        // Tiled 1.9+ writes "class"; older versions write "type".
        obj.m_type  = oj.value("class", oj.value("type", std::string{}));
        obj.m_layer = layerName;
        obj.m_rect  = {oj.value("x", 0.0f) + offset.x, oj.value("y", 0.0f) + offset.y,
                       oj.value("width", 0.0f), oj.value("height", 0.0f)};
        obj.m_properties = ReadProperties(oj);

        // Tiled object coordinates: orthogonal maps use plain pixels; isometric
        // maps measure both grid axes in units of tileheight.
        const float unitX = out.m_orientation == ENG::TileOrientation::Isometric ? out.m_tileSize.y : out.m_tileSize.x;
        const float unitY = out.m_tileSize.y;
        if (unitX > 0.0f && unitY > 0.0f)
          obj.m_gridPos = {obj.m_rect.x / unitX, obj.m_rect.y / unitY};
        out.m_objects.push_back(std::move(obj));
      }
    }

    void ReadLayers(const nlohmann::json& layers, const glm::vec2& offset, bool visible,
                    LoadContext& ctx, ENG::MapData& out)
    {
      for (const auto& lj : layers)
      {
        const std::string type = lj.value("type", std::string{});
        if (type == "tilelayer")
        {
          ReadTileLayer(lj, offset, visible, ctx, out);
        }
        else if (type == "objectgroup")
        {
          ReadObjectLayer(lj, offset, out);
        }
        else if (type == "group" && lj.contains("layers"))
        {
          const glm::vec2 groupOffset = offset + glm::vec2{lj.value("offsetx", 0.0f), lj.value("offsety", 0.0f)};
          ReadLayers(lj.at("layers"), groupOffset, visible && lj.value("visible", true), ctx, out);
        }
        // "imagelayer" is ignored.
      }
    }
  }

  std::optional<ENG::MapData> TiledLoader::Load(const std::string& mapPath)
  {
    nlohmann::json j;
    try
    {
      const auto fullPath = ENG::Path::Get().AssetsPath / mapPath;
      j = nlohmann::json::parse(ENG::Path::Get().ReadFileString(fullPath), nullptr, true, true);
    }
    catch (const std::exception& e)
    {
      LOG_ERROR(" | << TiledLoader: cannot read map " + mapPath + " - " + e.what());
      return std::nullopt;
    }

    if (!j.contains("layers") || !j.at("layers").is_array())
    {
      LOG_ERROR(" | << TiledLoader: " + mapPath + " is not a Tiled map (no \"layers\")");
      return std::nullopt;
    }

    ENG::MapData map;
    map.m_orientation = j.value("orientation", std::string{"orthogonal"}) == "isometric"
                          ? ENG::TileOrientation::Isometric
                          : ENG::TileOrientation::Orthogonal;
    map.m_tileSize    = {j.value("tilewidth", 0.0f), j.value("tileheight", 0.0f)};
    map.m_width       = j.value("width", 0);
    map.m_height      = j.value("height", 0);
    map.m_properties  = ReadProperties(j);

    if (j.value("orientation", std::string{"orthogonal"}) != "orthogonal" &&
        map.m_orientation != ENG::TileOrientation::Isometric)
      LOG_ERROR(" | << TiledLoader: unsupported orientation in " + mapPath + ", using orthogonal");

    LoadContext ctx;
    ctx.m_path = mapPath;
    if (j.contains("tilesets"))
    {
      for (const auto& tj : j.at("tilesets"))
      {
        ENG::MapTilesetData ts;
        // External tilesets only have "source"; embedded ones have "name".
        ts.m_name = tj.value("name", tj.value("source", std::string{}));
        ctx.m_ranges.push_back({static_cast<int>(map.m_tilesets.size()), tj.value("firstgid", 1u)});
        map.m_tilesets.push_back(std::move(ts));
      }
      std::sort(ctx.m_ranges.begin(), ctx.m_ranges.end(),
                [](const TilesetRange& a, const TilesetRange& b) { return a.m_firstGid < b.m_firstGid; });
    }

    ReadLayers(j.at("layers"), {0.0f, 0.0f}, true, ctx, map);
    return map;
  }
}
