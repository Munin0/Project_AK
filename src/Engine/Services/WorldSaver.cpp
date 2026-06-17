// | -------------------------------
#include "WorldSaver.hpp"
// | -------------------------------
// | -------------------------------
#include <pulse/def.h>
#include <string>
// | -------------------------------


namespace ENG
{
  void WorldSaver::SaveObject(const std::string& key, const ObjectState& obj)
  {
    states[key] = obj;
  }

  ObjectState* WorldSaver::GetObject(const std::string& key)
  {
    auto it = states.find(key);
    if(it == states.end()) return nullptr;

    return &it->second;
  }
}
