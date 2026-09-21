// | -------------------------------
#pragma once
// | -------------------------------
#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Transparent string hasher for unordered containers.
   *
   * StringHash lets an `std::unordered_map` (or `std::unordered_set`) keyed
   * by `std::string` be searched with a `std::string_view` or a
   * `const char*` directly, without creating a temporary `std::string` for
   * every lookup. This makes searches more efficient.
   *
   * All the overloads hash the same characters, so a string produces the
   * same hash regardless of the type it is passed as.
   *
   * To enable heterogeneous lookup, the container must also use a
   * transparent equality comparator such as `std::equal_to<>`:
   * @code
   * std::unordered_map<std::string, int, ENG::StringHash, std::equal_to<>> map;
   * map.find("player");   // No std::string is created.
   * @endcode
   *
   * @note Heterogeneous lookup in unordered containers requires C++20.
   */
  struct StringHash
  {
    /// @brief Marks the hasher as transparent, enabling heterogeneous lookup.
    using is_transparent = void;
    /**
     * @brief Hashes a string view.
     *
     * @param[in] sv String view to hash.
     * @return The hash value of the characters in @p sv.
     */
    size_t operator()(std::string_view sv) const {
        return std::hash<std::string_view>{}(sv);
    }
    /**
     * @brief Hashes a std::string.
     *
     * @param[in] s String to hash.
     * @return The hash value of the characters in @p s.
     */
    size_t operator()(const std::string& s) const {
        return std::hash<std::string_view>{}(s);
    }
    /**
     * @brief Hashes a null-terminated C string.
     *
     * @param[in] s Null-terminated string to hash. Must not be nullptr.
     * @return The hash value of the characters in @p s.
     */
    size_t operator()(const char* s) const {
        return std::hash<std::string_view>{}(s);
    }
  };
}
