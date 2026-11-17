// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/ComponentStorage.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/RenderSpace.hpp"
#include "Engine/Render/Shaders/Shader.hpp"
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Component/Component.hpp"
// | -------------------------------
#include <glm/detail/qualifier.hpp>
// | -------------------------------
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <type_traits>
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
       * @brief Receives the mouse position, for objects that react to the pointer.
       *
       * Called by ENG::DispatchPointer() (PoolSystems.hpp) once per frame on every visible
       * object of the pool, from the top layer down (the reverse of the draw
       * order), before the scene reads the mouse for the world. Does nothing by
       * default; UI objects (Button, TextBox) override it.
       *
       * To take a click, check it is yours first and then consume it, so the
       * objects below and the world do not get it:
       * @code
       * if (CollisionPointRect(mouse, rect) &&
       *     PollEvent::Get().ConsumeMouseButtonPress(SDL_BUTTON_LEFT))
       *   Action();
       * @endcode
       *
       * @param[in] mouse Mouse position in this object's RenderSpace: screen
       *                  pixels for Screen objects, world units (through the
       *                  active camera) for World objects.
       *
       * @warning Do not add or remove objects of the pool from here.
       */
      virtual void OnPointer(const Vector2& mouse) { (void)mouse; }

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
        T& component = components.Emplace<T>(std::forward<Args>(args)...);
        if constexpr (std::is_same_v<T, IBoundingBox>)
        {
          /// Centra la caja en el sprite (si ya existe): asi una caja mas
          /// pequena que el frame queda en el centro en vez de en la esquina.
          if (auto* sprite = GetComponent<ISprite>())
          {
            const Vector2 frame{(float)sprite->GetWidth(), (float)sprite->GetHeight()};
            const Vector2 boxSize = component.GetSize();
            component.SetOffset({(frame.x - boxSize.x) * 0.5f,
                                 (frame.y - boxSize.y) * 0.5f});
          }
          component.Update(GetTransform().m_position);
        }
        return component;
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
        static_assert(std::is_base_of_v<IComponent, T>);
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
        static_assert(std::is_base_of_v<IComponent, T>);
        return components.Has<T>();
      }

      /**
       * @brief Returns the number of objects created.
       * @return Value of the internal object counter.
       */
      static int GetCountObj()
      {
        return s_countObject;
      }

      /**
       * @brief Sets the name of the object.
       *
       * @param[in] _name New name.
       */
      void SetName(const std::string& _name)
      {
        this->m_name = _name;
      }

      /**
       * @brief Returns the name of the object.
       * @return The name of the object.
       */
      const std::string& GetName(void)
      {
        return this->m_name;
      }

      /**
       * @brief Returns the transform of the object.
       * @return Reference to the transform (position, velocity, etc.).
       */
      ITransform& GetTransform(void)
      {
        return this->m_transform;
      }

      /**
       * @brief Tells whether the object is alive.
       * @return true if the health points are greater than 0, false otherwise.
       */
      const bool IsAlive(void)
      {
        return m_alive;
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

      void SetTag(const std::string& tag)
      {
        this->m_tag = tag;
      }

      const std::string GetTag(void) const
      {
        return this->m_tag;
      }

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
      void SetLayer(uint8_t _layer) { m_layer = _layer;}

      /**
       * @brief Returns the layer of the object.
       * @return The layer the object belongs to.
       */
      uint8_t GetLayer() const { return m_layer; }

      /**
       * @brief Sets whether the object moves with the camera or stays fixed on screen.
       *
       * World (default): world coordinates, affected by the scene Camera2D.
       * Screen: screen pixels, the camera is ignored (HUD, menus, dialog boxes).
       *
       * @note Like SetLayer(), call it before the object is first drawn, or call
       *       ObjectPool::InvalidateQueue() after changing it.
       *
       * @param[in] space New render space.
       */
      void SetRenderSpace(RenderSpace space) { m_renderSpace = space; }

      /**
       * @brief Returns the render space of the object.
       * @return World or Screen.
       */
      RenderSpace GetRenderSpace() const { return m_renderSpace; }

      /**
       * @brief Shows or hides the object.
       *
       * A hidden object is not drawn (ENG::RenderObjects skips it) but it is
       * still updated and stays in the pool, so it can be shown again at once.
       *
       * @param[in] visible true to draw it (default), false to hide it.
       */
      void SetVisible(bool visible) { m_visible = visible; }

      /**
       * @brief Whether the object is drawn.
       * @return true if it is visible.
       */
      bool IsVisible() const { return m_visible; }

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
#ifndef NDEBUG
      /// @brief Debug only: logs (once per type) the components of the object
      ///        that are neither native, registered nor ignored, so they are
      ///        not saved by Save(). See ComponentSerializers.
      void WarnUnsavedComponents() const;
#endif

      /// @brief Counter of the objects created.
      static int s_countObject;

    protected:
      /// @brief Layer the object belongs to.
      uint8_t m_layer;
      /// @brief World (moves with the camera) or Screen (fixed on screen).
      RenderSpace m_renderSpace = RenderSpace::World;
      /// @brief Whether the object is drawn (it is updated anyway).
      bool m_visible = true;
      bool m_alive = false;

      /// @brief Name of the object.
      std::string m_name;
      std::string m_tag;

      /// @brief Transform of the object (position, velocity, etc.).
      ITransform m_transform;

      /// @brief Components attached to the object.
      ComponentStorage components;
  };
}
