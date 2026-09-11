// | -------------------------------
#include "WorldSaver.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include "nlohmann/json_fwd.hpp"
// | -------------------------------
#include <climits>
#include <cmath>
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
    m_ObjectSates[obj.m_name] = obj;
  }

  ObjectState* WorldSaver::GetObject(const std::string& key)
  {
    auto it = m_ObjectSates.find(key);
    if(it == m_ObjectSates.end())
    {
      LOG_ERROR(" ObjectState not fount");
       return nullptr;
    }

    return &it->second;
  }

  bool WorldSaver::HasObject(const std::string& key) const
  {
    return m_ObjectSates.contains(key);
  }

  void WorldSaver::RemoveObject(const std::string& key)
  {
    m_ObjectSates.erase(key);
  }

  void WorldSaver::Clear(void)
  {
    m_ObjectSates.clear();
  }

  void WorldSaver::to_json()
  {
    for(auto& [key, obj]: m_ObjectSates)
    {
      nlohmann::json j = nlohmann::json
      {
        {"name", obj.m_name},
        {"layer", obj.m_layer},
        {"trans", obj.m_tData},
        {"stats",obj.m_sData}
      };

      if(obj.m_spData.has_value())
        j["sprt"] = obj.m_spData.value();
      if(obj.m_anData.has_value())
        j["anim"] = obj.m_anData.value();
      if(obj.m_cData.has_value())
        j["color"] = obj.m_cData.value();
      if(!obj.m_bbData.empty())
        j["bbox"] = obj.m_bbData;
      if(obj.m_txData.has_value())
        j["text"] = obj.m_txData;

      data["objects"].push_back(j);
    }
  }

  void WorldSaver::from_json()
  {
    m_ObjectSates.clear();

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
      objState.m_tData = item.at("trans").get<ITransformData>();
      objState.m_sData = item.at("stats").get<IStatsData>();

      if(item.contains("sprt"))
        objState.m_spData = item.at("sprt").get<ISpriteData>();

      if(item.contains("anim"))
        objState.m_anData = item.at("anim").get<IAnimatorData>();

      if(item.contains("color"))
        objState.m_cData = item.at("color").get<IColorData>();

      if (item.contains("bbox"))
        objState.m_bbData = item.at("bbox").get<std::vector<IBoundingBoxData>>();
      
      if(item.contains("text"))
        objState.m_txData = item.at("text").get<ITextData>();
      
      m_ObjectSates[objState.m_name] = objState;
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
