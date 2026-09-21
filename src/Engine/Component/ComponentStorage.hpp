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
  /**
   * @brief Stores components, at most one of each type.
   *
   * ComponentStorage owns a set of components (classes derived from
   * IComponents) and lets them be created, queried and looked up by their
   * type. It is what an Object uses to hold its components (see
   * Object::AddComponent(), Object::GetComponent() and Object::HasComponent()).
   *
   * Components are looked up by their exact type: a component stored as a
   * derived class is not found when asking for its base class.
   *
   * Usage:
   * @code
   * ENG::ComponentStorage storage;
   * storage.Emplace<ENG::IColor>(ENG::Color::Red);
   * if (storage.Has<ENG::IColor>())
   *   ENG::IColor* color = storage.Get<ENG::IColor>();
   * @endcode
   *
   * @note It owns its components through std::unique_ptr, so it can be
   *       moved but not copied.
   *
   * @note It relies on run-time type information (RTTI): typeid is used to
   *       identify the type of each component.
   */
  class ComponentStorage {
    public:
      /**
       * @brief Creates a component of type T and stores it.
       *
       * The component is constructed in place with the given arguments.
       *
       * @warning If a component of type T is already stored, it is replaced
       *          and destroyed: any reference or pointer to the previous
       *          one becomes invalid.
       *
       * @tparam T    Type of the component to create. It must derive from IComponents.
       * @tparam Args Types of the arguments forwarded to the constructor of T.
       * @param[in] args Arguments forwarded to the constructor of T.
       * @return Reference to the new component. It is owned by the storage.
       */
      template <typename T, typename... Args> T &Emplace(Args &&...args)
      {
        auto key = std::type_index(typeid(T));
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T &ref = *ptr;
        _map[key] = std::move(ptr);
        return ref;
      }

      /**
       * @brief Returns the stored component of type T.
       *
       * @tparam T Type of the component to look for. It must be the exact
       *           type the component was created with.
       * @return Pointer to the component, or nullptr if there is none of
       *         that type. It is owned by the storage.
       */
      template <typename T> T *Get() const
      {
        auto it = _map.find(std::type_index(typeid(T)));
        if (it != _map.end())
          return static_cast<T *>(it->second.get());
        return nullptr;
      }

      /**
       * @brief Tells whether a component of type T is stored.
       *
       * @tparam T Type of the component to look for. It must be the exact
       *           type the component was created with.
       * @return true if a component of that type is stored, false otherwise.
       */
      template <typename T> bool Has() const
      {
        return _map.contains(std::type_index(typeid(T)));
      }

    private:
      /// @brief Stored components, indexed by their exact type.
      std::unordered_map<std::type_index, std::unique_ptr<IComponents>> _map;
  };

}
