// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/ComponentStorage.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Component/Component.hpp"
// | -------------------------------
#include <glm/detail/qualifier.hpp>
// | -------------------------------
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>
// | -------------------------------

namespace ENG
{
  class Object
  {
    public:
      explicit Object(const std::string& _name);
      virtual ~Object() = default;
      virtual void Draw(Batcher& b) const;
      virtual void Update(float dt);

      template<typename T, typename... Args>
      T& AddComponent(Args&&... args)
      {
        return components.Emplace<T>(std::forward<Args>(args)...);
      }

      template<typename T>
      T* GetComponent() const
      {
        static_assert(std::is_base_of_v<IComponents, T>);
        return components.Get<T>(); 
      }

      template<typename T>
      bool HasComponent() const
      {
        static_assert(std::is_base_of_v<IComponents, T>);
        return components.Has<T>();
      }

      // IBoundingBox needs multiple simultaneous instances per object (e.g. one solid box +
      // one trigger box), which ComponentStorage doesn't support (one instance per type), so it
      // gets its own dedicated storage instead of going through AddComponent/GetComponent.
      IBoundingBox& AddBoundingBox(const Vector2& dim, bool isTrigger = false)
      {
        boundingBoxes.emplace_back(dim, isTrigger);
        return boundingBoxes.back();
      }

      std::vector<IBoundingBox>& GetBoundingBoxes(void) { return boundingBoxes; }
      const std::vector<IBoundingBox>& GetBoundingBoxes(void) const { return boundingBoxes; }
      bool HasBoundingBoxes(void) const { return !boundingBoxes.empty(); }

      IBoundingBox* GetSolidBox(void)
      {
        for (auto& bb : boundingBoxes)
          if (!bb.IsTrigger())
            return &bb;
        return nullptr;
      }

      IBoundingBox* GetTriggerBox(void)
      {
        for (auto& bb : boundingBoxes)
          if (bb.IsTrigger())
            return &bb;
        return nullptr;
      }

      static int GetCountObj()
      {
        return countObject;
      }

      const std::string& GetName(void)
      {
        return this->name;
      }
   
      ITransform& GetTransform(void)
      {
        return this->transform;
      }

      IStats& GetStats(void)
      {
        return this->stats;
      }

      const bool IsAlive(void)
      {
        return stats.hp > 0;
      }

      const ComponentStorage& GetListComponents(void)
      {
        return components;
      }
      
      void SetPosition(float x, float y);
      void SetPosition(const Vector2& new_pos);
      const Vector2& GetPosition(void) const;
      Vector2 GetSize(void) const;
      void SetLayer(uint8_t _layer) { layer = _layer;}
      uint8_t GetLayer() const { return layer; }
      Shader* GetShader() const
      {
        if(HasComponent<IMaterial>())
          return GetComponent<IMaterial>()->GetShader();
        else
          return nullptr;
      }

      // Serialization
      ObjectState Save() const;
      void Load(const ObjectState& state);
    private:
      static int countObject;
    protected:
      uint8_t layer;
      
      std::string name;
      IStats stats;
      ITransform transform;
      std::vector<IBoundingBox> boundingBoxes;

      ComponentStorage components;
  };
} // namespace ENG
