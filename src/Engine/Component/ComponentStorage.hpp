// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
// | -------------------------------
#include <memory>
#include <typeindex>
#include <unordered_map>
// | -------------------------------

namespace ENG 
{
  class ComponentStorage {
    public:
      template <typename T, typename... Args> T &Emplace(Args &&...args)
      {
        auto key = std::type_index(typeid(T));
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T &ref = *ptr;
        _map[key] = std::move(ptr);
        return ref;
      }

      template <typename T> T *Get() const
      {
        auto it = _map.find(std::type_index(typeid(T)));
        if (it != _map.end())
          return static_cast<T *>(it->second.get());
        return nullptr;
      }

      template <typename T> bool Has() const 
      {
        return _map.contains(std::type_index(typeid(T)));
      }

    private:
      std::unordered_map<std::type_index, std::unique_ptr<IComponents>> _map;
  };

} // namespace ENG
