// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "nlohmann/detail/macro_scope.hpp"
#include "nlohmann/json_fwd.hpp"
#include "nlohmann/json.hpp"
// | -------------------------------
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
// | -------------------------------

namespace ENG
{
  struct IColorData
  {
    float m_r;
    float m_g;
    float m_b;
    float m_a;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IColorData,m_r,m_g,m_b,m_a)
  };

  struct IBoundingBoxData
  {
    float m_width;
    float m_height;
    bool  m_isTrigger;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IBoundingBoxData,m_width,m_height,m_isTrigger)
  };

  struct IAnimatorData
  {
    std::string m_key;
    int m_frames;
    int m_step;
    float m_speed;
    float m_scale;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IAnimatorData,m_key,m_frames,m_step,m_speed,m_scale)
  };

  struct ISpriteData
  {
    std::string m_keyName;
    int m_width;
    int m_height;
    float m_scale;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ISpriteData,m_keyName,m_width,m_height,m_scale)
  };

  struct IStatsData
  {
    uint8_t m_hp;
    uint8_t m_hp_max;
    uint8_t m_str;
    uint8_t m_def;
    uint8_t m_agi;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IStatsData, m_hp,m_hp_max,m_str,m_def,m_agi)
  };

  struct ITransformData
  {
    Vector2 m_position;
    Vector2 m_prev_position;
    Vector2 m_velocity;
    Vector2 m_direction;
    float   m_angle;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ITransformData, m_position,m_prev_position,m_velocity,m_direction,m_angle)
  };

  struct ITextData
  {
    std::string m_text;
    float m_fontSize;
    std::string m_fontKey;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ITextData, m_text,m_fontSize,m_fontKey)
  };

  struct ObjectState
  {
    std::string       m_name;
    uint8_t           m_layer;
    ITransformData    m_tData;
    IStatsData        m_sData;
    std::optional<ISpriteData>      m_spData;
    std::optional<IAnimatorData>    m_anData;
    std::vector<IBoundingBoxData>   m_bbData;
    std::optional<IColorData>       m_cData;
    std::optional<ITextData>        m_txData;
  };

  class WorldSaver
  {
    public:
      void SaveObject(const ObjectState& obj);
      ObjectState* GetObject(const std::string& key);
      bool HasObject(const std::string& key) const;
      void RemoveObject(const std::string& key);
      void Clear(void);
      
      bool SaveToDisk(const std::string& path);
      bool LoadFromDisk(const std::string& path);

    private:
      void to_json();
      void from_json();
    private:
      std::unordered_map<std::string, ObjectState> m_ObjectSates;
      nlohmann::json data;
  };
  
}


