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
  /**
   * @brief Base class of every object in the game world.
   *
   * An Object has a name, a layer, a transform, stats, a list of bounding
   * boxes and a set of components (sprite, animator, material, etc.). It
   * can be updated every frame, drawn through the Batcher, and saved to /
   * loaded from an ObjectState.
   *
   * Derived classes (for example Text, GLine or GRectangle) customize the
   * behavior by overriding Draw(), Update(), Save() and Load().
   *
   * Usage:
   * @code
   * ENG::Object player("player");
   * player.SetPosition(100.0f, 200.0f);
   * player.AddBoundingBox({32.0f, 32.0f});
   * @endcode
   */
  class Object
  {
    public:
      /**
       * @brief Creates an object.
       *
       * @param[in] _name Name of the object.
       */
      explicit Object(const std::string& _name);

      /// @brief Virtual destructor, so derived objects are destroyed correctly.
      virtual ~Object() = default;

      /**
       * @brief Submits the object to the Batcher for rendering.
       *
       * Derived classes override it to draw themselves.
       *
       * @param[in,out] b Batcher that collects the draw data of this frame.
       */
      virtual void Draw(Batcher& b) const;

      /**
       * @brief Updates the object once per frame.
       *
       * Derived classes override it to add their own logic.
       *
       * @param[in] dt Time elapsed since the previous frame, in seconds.
       */
      virtual void Update(float dt);

      /**
       * @brief Adds a component to the object.
       *
       * The component is constructed in place inside the object's component
       * storage.
       *
       * @tparam T    Type of the component to add.
       * @tparam Args Types of the arguments forwarded to the constructor of T.
       * @param[in] args Arguments forwarded to the constructor of T.
       * @return Reference to the new component.
       */
      template<typename T, typename... Args>
      T& AddComponent(Args&&... args)
      {
        return components.Emplace<T>(std::forward<Args>(args)...);
      }

      /**
       * @brief Returns a component of the object.
       *
       * @tparam T Type of the component to look for. It must derive from IComponents.
       * @return Pointer to the component, or nullptr if the object does not
       *         have a component of that type.
       */
      template<typename T>
      T* GetComponent() const
      {
        static_assert(std::is_base_of_v<IComponents, T>);
        return components.Get<T>();
      }

      /**
       * @brief Tells whether the object has a component.
       *
       * @tparam T Type of the component to look for. It must derive from IComponents.
       * @return true if the object has a component of that type, false otherwise.
       */
      template<typename T>
      bool HasComponent() const
      {
        static_assert(std::is_base_of_v<IComponents, T>);
        return components.Has<T>();
      }

      /**
       * @brief Adds a bounding box to the object.
       *
       * @param[in] dim       Size of the bounding box: { width, height }.
       * @param[in] isTrigger true if the box is a trigger (detects overlaps
       *                      without blocking movement). Defaults to false.
       * @return Reference to the new bounding box.
       *
       * @warning The reference (and any pointer to a bounding box of this
       *          object) can become invalid when another bounding box is
       *          added, because the boxes are stored in a std::vector.
       */
      IBoundingBox& AddBoundingBox(const Vector2& dim, bool isTrigger = false)
      {
        boundingBoxes.emplace_back(dim, isTrigger);
        return boundingBoxes.back();
      }

      /**
       * @brief Returns the bounding boxes of the object.
       * @return Reference to the list of bounding boxes.
       */
      std::vector<IBoundingBox>& GetBoundingBoxes(void) { return boundingBoxes; }

      /**
       * @brief Tells whether the object has any bounding box.
       * @return true if the object has at least one bounding box, false otherwise.
       */
      bool HasBoundingBoxes(void) const { return !boundingBoxes.empty(); }

      /**
       * @brief Returns the first solid (non-trigger) bounding box.
       * @return Pointer to the box, or nullptr if the object has none.
       */
      IBoundingBox* GetSolidBox(void)
      {
        for (auto& bb : boundingBoxes)
          if (!bb.IsTrigger())
            return &bb;
        return nullptr;
      }

      /**
       * @brief Returns the first trigger bounding box.
       * @return Pointer to the box, or nullptr if the object has none.
       */
      IBoundingBox* GetTriggerBox(void)
      {
        for (auto& bb : boundingBoxes)
          if (bb.IsTrigger())
            return &bb;
        return nullptr;
      }

      /**
       * @brief Returns the number of objects created.
       * @return Value of the internal object counter.
       */
      static int GetCountObj()
      {
        return countObject;
      }

      /**
       * @brief Sets the name of the object.
       *
       * @param[in] _name New name.
       */
      void SetName(const std::string& _name)
      {
        this->name = _name;
      }

      /**
       * @brief Returns the name of the object.
       * @return The name of the object.
       */
      const std::string& GetName(void)
      {
        return this->name;
      }

      /**
       * @brief Returns the transform of the object.
       * @return Reference to the transform (position, velocity, etc.).
       */
      ITransform& GetTransform(void)
      {
        return this->transform;
      }

      /**
       * @brief Returns the stats of the object.
       * @return Reference to the stats (health, strength, etc.).
       */
      IStats& GetStats(void)
      {
        return this->stats;
      }

      /**
       * @brief Tells whether the object is alive.
       * @return true if the health points are greater than 0, false otherwise.
       */
      const bool IsAlive(void)
      {
        return stats.m_hp > 0;
      }

      /**
       * @brief Returns the components of the object.
       * @return Constant reference to the component storage.
       */
      const ComponentStorage& GetListComponents(void)
      {
        return components;
      }

      /**
       * @brief Sets the position of the object.
       *
       * @param[in] x New X coordinate.
       * @param[in] y New Y coordinate.
       */
      void SetPosition(float x, float y);

      /**
       * @brief Sets the position of the object.
       *
       * @param[in] new_pos New position.
       */
      void SetPosition(const Vector2& new_pos);

      /**
       * @brief Returns the position of the object.
       * @return The current position.
       */
      const Vector2& GetPosition(void) const;

      /**
       * @brief Returns the size of the object's sprite.
       * @return The size of the sprite: { width, height }.
       */
      Vector2 GetSizeSprite(void) const;

      /**
       * @brief Sets the layer of the object.
       *
       * @param[in] _layer New layer.
       */
      void SetLayer(uint8_t _layer) { layer = _layer;}

      /**
       * @brief Returns the layer of the object.
       * @return The layer the object belongs to.
       */
      uint8_t GetLayer() const { return layer; }

      /**
       * @brief Returns the shader of the object.
       *
       * The shader comes from the object's IMaterial component.
       *
       * @return Pointer to the shader, or nullptr if the object has no
       *         IMaterial component.
       */
      Shader* GetShader() const
      {
        if(HasComponent<IMaterial>())
          return GetComponent<IMaterial>()->GetShader();
        else
          return nullptr;
      }

      // Serialization

      /**
       * @brief Saves the state of the object.
       *
       * Derived classes override it to save their own data.
       *
       * @return An ObjectState describing this object, suitable for
       *         restoring it later with Load().
       */
      virtual ObjectState Save();

      /**
       * @brief Restores the object from a previously saved state.
       *
       * Derived classes override it to load their own data.
       *
       * @param[in] state State produced by Save().
       */
      virtual void Load(const ObjectState& state);

    private:
      /// @brief Counter of the objects created.
      static int countObject;

    protected:
      /// @brief Layer the object belongs to.
      uint8_t layer;

      /// @brief Name of the object.
      std::string name;

      /// @brief Stats of the object (health, strength, etc.).
      IStats stats;

      /// @brief Transform of the object (position, velocity, etc.).
      ITransform transform;

      /// @brief Bounding boxes of the object.
      std::vector<IBoundingBox> boundingBoxes;

      /// @brief Components attached to the object.
      ComponentStorage components;
  };
}
