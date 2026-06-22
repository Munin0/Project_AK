/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RenderEntry.hpp"
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
    private:
      std::vector<std::unique_ptr<ENG::Object>> dense;
      std::vector<ObjectID>                     denseID;
      std::unordered_map<ObjectID, size_t>      sparse;
      std::vector<ENG::RenderEntry> renderQueue;
      ObjectID nextID = 1;
      bool sortedQueue = false;
    public:
      const std::vector<RenderEntry>& Sort()
      {
        if(!sortedQueue)
        {
          renderQueue.clear();
          for (ENG::ObjectID id : denseID)
          {
            auto obj = Get(id);
            renderQueue.push_back({ id, obj->GetLayer() });
          }

          std::sort(renderQueue.begin(), renderQueue.end(),
            [](const ENG::RenderEntry& a, const ENG::RenderEntry& b) {
            return a.layer > b.layer;
          });
          sortedQueue = true;
        }
        return renderQueue;
      }

      ObjectID Add(std::unique_ptr<Object> obj) 
      {
        ObjectID id = nextID++;
        size_t index = dense.size();

        dense.push_back(std::move(obj));
        denseID.push_back(id);
        sparse[id] = index;
        
        sortedQueue = false;
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
        sortedQueue = false;
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
