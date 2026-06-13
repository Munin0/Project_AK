// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/ComponentStorage.hpp"
#include "Engine/Render/RBatch.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Component/Component.hpp"
// | -------------------------------
#include <cstdint>
#include <glm/detail/qualifier.hpp>
#include <string>
#include <type_traits>
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
    private:
      uint8_t layer;
      static int countObject;
    protected:
      ITransform transform;
      IStats stats;
      std::string name;

      ComponentStorage components;
  };
} // namespace ENG
