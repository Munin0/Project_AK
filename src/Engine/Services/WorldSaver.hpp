// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  struct IColorData
  {
    float r;
    float g;
    float b;
    float a;
  };

  struct IBoundingBoxData
  {
    float width;
    float height;
  };

  struct IAnimatorData
  {
    std::string key;
    int frames;
    int step;
    float speed;
    float scale;
  };

  struct ISpriteData
  {
    std::string keyName;
    int width;
    int height;
    float scale;
  };

  struct IStatsData
  {
    uint8_t hp;
    uint8_t hp_max;
    uint8_t str;
    uint8_t def;
    uint8_t agi;
  };

  struct ITransformData
  {
    Vector2 position;
    Vector2 prev_position;
    Vector2 velocity;
    Vector2 direction;
    float   angle;
  };

  struct ObjectState
  {
    std::string       _name;
    uint8_t           _layer;
    ITransformData    _tData;
    IStatsData        _sData;
    std::optional<ISpriteData>       _spData;
    std::optional<IAnimatorData>    _anData;
    std::optional<IBoundingBoxData> _bbData;
    std::optional<IColorData>       _cData;
  };

  class WorldSaver
  {
    public:
      void SaveObject(const std::string& key, const ObjectState& obj);
      ObjectState* GetObject(const std::string& key);
      bool HasObject(const std::string& key) const;
      void Clear(void);
    private:
      std::unordered_map<std::string, ObjectState> states;
  };
}
