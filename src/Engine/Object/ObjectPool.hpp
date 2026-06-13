/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Object/Object.hpp"
/// | ------------------------------------ |
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <unordered_map>
#include <vector>
/// | ------------------------------------ |

namespace ENG
{
  using ObjectID = uint32_t;
  constexpr ObjectID NULL_ID = 0;

  class ObjectPool
  {
    std::vector<std::unique_ptr<ENG::Object>> dense;
    std::vector<ObjectID>                     denseID;
    std::unordered_map<ObjectID, size_t>      sparse;

    ObjectID nextID = 1;

    public:
      ObjectID Add(std::unique_ptr<Object> obj) 
      {
        ObjectID id = nextID++;
        size_t index = dense.size();

        dense.push_back(std::move(obj));
        denseID.push_back(id);
        sparse[id] = index;

        return id;
      }

      ENG::Object* Get(ObjectID id) 
      {
        auto it = sparse.find(id);
        if (it == sparse.end()) 
          return nullptr;
        return dense[it->second].get();
      }

      const std::vector<ObjectID>& GetAllIDs() const
      {
        return denseID;
      }

      void Remove(ObjectID id)
      {
        auto it = sparse.find(id);
        if (it == sparse.end()) return;

        size_t index     = it->second;
        size_t lastIndex = dense.size() - 1;

        std::swap(dense[index],    dense[lastIndex]);
        std::swap(denseID[index], denseID[lastIndex]);

        if(index != lastIndex)
          sparse[denseID[index]] = index;

        dense.back().reset();
        dense.pop_back();
        denseID.pop_back();
        sparse.erase(id);
      }

      void Clear()
      {
        dense.clear();
        denseID.clear();
        sparse.clear();
        nextID = 1;
      }

      int Size() const
      {
        return dense.size();
      }
  };

}
