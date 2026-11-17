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
  /// @brief Unique identifier of an object inside an ObjectPool.
  using ObjectID = uint32_t;

  /// @brief ID that does not belong to any object. Valid IDs start at 1.
  constexpr ObjectID NULL_ID = 0;

  /**
   * @brief Owns the objects of the world and gives access to them by ID.
   *
   * ObjectPool stores its objects contiguously (a "dense" list) and keeps a
   * map from each ObjectID to the position of its object (a "sparse" map).
   * This makes adding, looking up and removing objects fast, and keeps
   * iteration over all the objects cache-friendly.
   *
   * The pool owns the objects: they are destroyed when they are removed or
   * when the pool is cleared. Pointers returned by Get() stay valid while
   * the object is in the pool, even if other objects are added or removed.
   *
   * It also builds a render queue (see Sort()) with the objects ordered by
   * layer, render space and shader, so they can be drawn in the right order
   * with few camera / material changes.
   *
   * Usage:
   * @code
   * ENG::ObjectPool pool;
   * ENG::ObjectID id = pool.Add(std::make_unique<ENG::Object>("player"));
   * pool.Get(id)->SetPosition(10.0f, 20.0f);
   * pool.Remove(id);
   * @endcode
   */
  class ObjectPool
  {
    private:
      /// @brief Objects owned by the pool, stored contiguously.
      std::vector<std::unique_ptr<ENG::Object>> dense;

      /// @brief ID of each object of `dense`, in the same order.
      std::vector<ObjectID>                     denseID;

      /// @brief Position in `dense` of the object with each ID.
      std::unordered_map<ObjectID, size_t>      sparse;

      /// @brief Cached render queue built by Sort().
      std::vector<ENG::RenderEntry> renderQueue;

      /// @brief ID that will be given to the next added object.
      ObjectID nextID = 1;

      /// @brief Whether `renderQueue` is up to date with the objects of the pool.
      bool sortedQueue = false;

    public:
      /**
       * @brief Returns the render queue, sorted for drawing.
       *
       * The entries are ordered by layer (ascending) and, within the same
       * layer, by shader, so objects that use the same shader are drawn
       * together. Entries with the same layer and shader keep the order of
       * the objects in the pool's internal storage.
       *
       * The queue is cached: it is rebuilt only after an object has been
       * added or removed.
       *
       * @warning The cache is not refreshed when the layer or the shader of
       *          an existing object changes.
       *
       * @return Constant reference to the sorted render queue. It is
       *         invalidated by the next call that changes the pool.
       */
      const std::vector<RenderEntry>& Sort()
      {
        if(!sortedQueue)
        {
          renderQueue.clear();
          for (ENG::ObjectID id : denseID)
          {
            auto obj = Get(id);
            renderQueue.push_back({ id, obj->GetLayer(), obj->GetRenderSpace(), obj->GetShader()});
          }

          std::stable_sort(renderQueue.begin(), renderQueue.end(),
            [](const ENG::RenderEntry& a, const ENG::RenderEntry& b)
          {
            // layer -> space -> shader: inside a layer, World and Screen objects are
            // grouped, so the camera only changes (and flushes) once per group.
            if(a.m_layer != b.m_layer)
              return a.m_layer < b.m_layer;
            if(a.m_space != b.m_space)
              return a.m_space < b.m_space;
            return a.m_shader < b.m_shader;
          });
          sortedQueue = true;
        }
        return renderQueue;
      }

      /**
       * @brief Adds an object to the pool.
       *
       * The pool takes ownership of the object.
       *
       * @param[in] obj Object to add. It must not be nullptr.
       * @return The ID given to the object.
       */
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

      /**
       * @brief Returns an object by ID.
       *
       * @param[in] id ID of the object.
       * @return Pointer to the object, or nullptr if there is no object with
       *         that ID. The object is owned by the pool and the pointer
       *         becomes invalid when the object is removed.
       */
      ENG::Object* Get(ObjectID id)
      {
        auto it = sparse.find(id);
        if (it == sparse.end())
          return nullptr;
        return dense[it->second].get();
      }

      /**
       * @brief Returns the IDs of all the objects in the pool.
       *
       * The IDs are in the internal storage order, which is not necessarily
       * the order in which the objects were added.
       *
       * @return Constant reference to the list of IDs. It is invalidated by
       *         Add(), Remove() and Clear(), so do not add or remove
       *         objects while iterating over it.
       */
      const std::vector<ObjectID>& GetAllIDs() const
      {
        return denseID;
      }

      /**
       * @brief Removes an object from the pool and destroys it.
       *
       * Does nothing if there is no object with that ID. The last object of
       * the pool takes the place of the removed one, so the internal order
       * of the objects changes.
       *
       * @param[in] id ID of the object to remove.
       */
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

      /**
       * @brief Removes and destroys every object of the pool.
       *
       * The ID counter is reset, so the next object added gets the ID 1
       * again.
       *
       * The cached render queue is emptied too.
       */
      void Clear()
      {
        dense.clear();
        denseID.clear();
        sparse.clear();
        renderQueue.clear();
        sortedQueue = false;
        nextID = 1;
      }

      /**
       * @brief Forces the render queue to be rebuilt on the next Sort().
       *
       * Call it after changing the layer, the render space or the shader of an object that is
       * already in the pool (Add/Remove/Clear invalidate it on their own).
       */
      void InvalidateQueue()
      {
        sortedQueue = false;
      }

      /**
       * @brief Returns the number of objects in the pool.
       * @return The number of objects.
       */
      int Size() const
      {
        return dense.size();
      }
  };

}
