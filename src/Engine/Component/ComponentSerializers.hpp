// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
// | -------------------------------
#include <exception>
#include <functional>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_set>
#include <vector>
// | -------------------------------

namespace ENG
{
  /**
   * @brief How to save and load one component type that the engine does not know.
   */
  struct ComponentSerializer
  {
    /// @brief Key of the component inside ObjectState::m_components (and the save file).
    std::string m_key;
    /// @brief Component type, to know which components of an object are covered.
    std::type_index m_type = typeid(void);
    /// @brief Writes the component of the object into the json. Returns false if the
    ///        object does not have that component.
    std::function<bool(const Object&, nlohmann::json&)> m_save;
    /// @brief Adds (or replaces) the component on the object from the json.
    std::function<void(Object&, const nlohmann::json&)> m_load;
  };

  /**
   * @brief Registry of game components that Object::Save() / Object::Load() handle.
   *
   * The engine serializes its own components (transform, sprite, animator...)
   * by itself. Components that belong to a game (stats, inventory, quests...)
   * are registered once at start-up, so the engine can save and load them
   * without knowing their type:
   *
   * @code
   * // Game side: the component only needs to_json / from_json.
   * struct IStats : ENG::IComponent { uint8_t m_hp = 0; ...
   *   NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(IStats, m_hp, ...) };
   *
   * // Once, e.g. in Game::OnInit():
   * ENG::ComponentSerializers::Register<APP::IStats>("stats");
   * @endcode
   *
   * Requirements of T: derives from IComponent, is default constructible and
   * has nlohmann to_json / from_json.
   *
   * Components that must never be saved (runtime caches, AI state, handles...)
   * are marked with Ignore<T>(). In Debug builds Object::Save() logs, once per
   * type, any component that is neither native, registered nor ignored, so a
   * forgotten Register() does not silently lose data.
   *
   * @code
   * ENG::ComponentSerializers::Ignore<APP::IPathCache>();
   * @endcode
   */
  class ComponentSerializers
  {
    public:
      /**
       * @brief Registers a component type under a key.
       *
       * @tparam T Component type.
       * @param[in] key Unique key used in the save file. Registering the same
       *                key twice is an error and the second one is ignored.
       */
      template <typename T>
      static void Register(const std::string& key)
      {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from ENG::IComponent");
        static_assert(std::is_default_constructible_v<T>, "T must be default constructible");

        if (Find(key))
        {
          LOG_ERROR(" | << ComponentSerializers: key '" + key + "' already registered");
          return;
        }

        if (IsRegistered(typeid(T)))
        {
          LOG_ERROR(" | << ComponentSerializers: type of '" + key + "' already registered");
          return;
        }

        List().push_back({
          key,
          typeid(T),
          [](const Object& obj, nlohmann::json& j)
          {
            const T* component = obj.GetComponent<T>();
            if (!component)
              return false;
            j = *component;
            return true;
          },
          [key](Object& obj, const nlohmann::json& j)
          {
            try
            {
              T component = j.get<T>();
              obj.AddComponent<T>(std::move(component));
            }
            catch (const std::exception& e)
            {
              LOG_ERROR(" | << ComponentSerializers: cannot load '" + key + "': " + e.what());
            }
          }});
      }

      /**
       * @brief Marks a component type as runtime-only: it is never saved and
       *        Object::Save() does not warn about it.
       *
       * @tparam T Component type.
       */
      template <typename T>
      static void Ignore()
      {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from ENG::IComponent");
        Ignored().insert(typeid(T));
      }

      /// @brief Whether @p type was registered with Register().
      static bool IsRegistered(std::type_index type)
      {
        for (const ComponentSerializer& s : List())
          if (s.m_type == type)
            return true;
        return false;
      }

      /// @brief Whether @p type was marked with Ignore().
      static bool IsIgnored(std::type_index type) { return Ignored().contains(type); }

      /// @brief Every registered serializer, in registration order.
      static const std::vector<ComponentSerializer>& GetAll() { return List(); }

      /// @brief Serializer registered under @p key, or nullptr.
      static const ComponentSerializer* Find(const std::string& key)
      {
        for (const ComponentSerializer& s : List())
          if (s.m_key == key)
            return &s;
        return nullptr;
      }

      /// @brief Removes every registered serializer and ignored type.
      static void Clear()
      {
        List().clear();
        Ignored().clear();
      }

    private:
      static std::vector<ComponentSerializer>& List()
      {
        static std::vector<ComponentSerializer> list;
        return list;
      }

      static std::unordered_set<std::type_index>& Ignored()
      {
        static std::unordered_set<std::type_index> ignored;
        return ignored;
      }
  };
}
