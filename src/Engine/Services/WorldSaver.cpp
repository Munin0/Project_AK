// | -------------------------------
#include "WorldSaver.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
// | -------------------------------
// | -------------------------------
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
// | -------------------------------


namespace ENG
{
  void WorldSaver::SaveObject(const ObjectState& obj)
  {
    m_objectSates[obj.m_name] = obj;
  }

  ObjectState* WorldSaver::GetObject(const std::string& key)
  {
    auto it = m_objectSates.find(key);
    if(it == m_objectSates.end())
    {
      LOG_ERROR(" ObjectState not fount");
       return nullptr;
    }

    return &it->second;
  }

  bool WorldSaver::HasObject(const std::string& key) const
  {
    return m_objectSates.contains(key);
  }

  void WorldSaver::RemoveObject(const std::string& key)
  {
    m_objectSates.erase(key);
  }

  void WorldSaver::Clear(void)
  {
    m_objectSates.clear();
  }

  void WorldSaver::to_json()
  {
    // Rebuild the array from scratch so calling SaveToDisk() twice does not duplicate objects.
    data["objects"] = nlohmann::json::array();

    for(auto& [key, obj]: m_objectSates)
    {
      nlohmann::json j = nlohmann::json
      {
        {"name", obj.m_name},
        {"layer", obj.m_layer},
        {"space", obj.m_renderSpace},
        {"visible", obj.m_visible},
        {"trans", obj.m_tData}
      };

      // Optional components: only written when present.
      if(obj.m_spData)
        j["sprt"] = *obj.m_spData;
      if(obj.m_anData)
        j["anim"] = *obj.m_anData;
      if(obj.m_cData)
        j["color"] = *obj.m_cData;
      if(obj.m_bbData)
        j["bbox"] = *obj.m_bbData;
      if(obj.m_txData)
        j["text"] = *obj.m_txData;
      if(obj.m_ccData)
        j["circle"] = *obj.m_ccData;
      if(obj.m_mtData)
        j["mat"] = *obj.m_mtData;
      // Game components (ComponentSerializers), each under its own key.
      if(!obj.m_components.empty())
        j["comps"] = obj.m_components;

      data["objects"].push_back(j);
    }
  }

  void WorldSaver::from_json()
  {
    m_objectSates.clear();

    if(!data.contains("objects"))
    {
      LOG_ERROR(" | << Error, JSON has no \"objects\" array");
      return;
    }

    for(auto& item:data.at("objects"))
    {
      if (item.is_null())
      {
        LOG_ERROR(" | << Error, JSON is empty");
        continue;
      }
      ObjectState objState;
      objState.m_name = item.at("name").get<std::string>();
      objState.m_layer = item.at("layer").get<decltype(objState.m_layer)>();
      objState.m_renderSpace = item.value("space", static_cast<uint8_t>(0));   // older saves: World
      objState.m_visible = item.value("visible", true);
      objState.m_tData = item.at("trans").get<ITransformData>();

      if(item.contains("sprt"))
        objState.m_spData = item.at("sprt").get<ISpriteData>();

      if(item.contains("anim"))
        objState.m_anData = item.at("anim").get<IAnimatorData>();

      if(item.contains("color"))
        objState.m_cData = item.at("color").get<IColorData>();

      if (item.contains("bbox"))
        objState.m_bbData = item.at("bbox").get<IBoundingBoxData>();

      if(item.contains("text"))
        objState.m_txData = item.at("text").get<ITextData>();

      if(item.contains("circle"))
        objState.m_ccData = item.at("circle").get<ICircleBoxData>();

      if(item.contains("mat"))
        objState.m_mtData = item.at("mat").get<IMaterialData>();

      if(item.contains("comps") && item.at("comps").is_object())
        for(auto& [key, value] : item.at("comps").items())
          objState.m_components[key] = value;

      // Saves made before ComponentSerializers kept the stats at this level.
      if(item.contains("stats") && !item.at("stats").is_null() && !objState.m_components.contains("stats"))
        objState.m_components["stats"] = item.at("stats");

      m_objectSates[objState.m_name] = objState;
    }
  }

  bool WorldSaver::SaveToDisk(const std::string& path)
  {
    /// Load to data (json) the objects map
    to_json();
    /// file to write data
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open())
    {
      LOG_ERROR(" | << File cant be open");
      return false;
    }
    /// Convert to binary the data json & write
    std::vector<uint8_t> bin = nlohmann::json::to_cbor(data);
    file.write(reinterpret_cast<const char*>(bin.data()), bin.size());

    return file.good();
  }

  bool WorldSaver::LoadFromDisk(const std::string& path)
  {
    /// File to read from the data
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
      LOG_ERROR(" | << File cant be open");
      return false;
    }
    /// Convert the data binary to json struct
    std::vector<uint8_t> bin((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    data = nlohmann::json::from_cbor(bin);

    from_json();
    return true;
  }
}
