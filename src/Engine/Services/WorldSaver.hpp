// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Services/KeyHash.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "nlohmann/detail/macro_scope.hpp"
#include "nlohmann/json_fwd.hpp"
#include "nlohmann/json.hpp"
// | -------------------------------
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Serializable color data of an object.
   *
   * Stores a color as RGBA components.
   */
  struct IColorData
  {
    /// @brief Red component.
    float m_r;
    /// @brief Green component.
    float m_g;
    /// @brief Blue component.
    float m_b;
    /// @brief Alpha (opacity) component.
    float m_a;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IColorData,m_r,m_g,m_b,m_a)
  };

  /**
   * @brief Serializable bounding box data of an object.
   *
   * Describes the size of a collision box and whether it acts as a trigger.
   */
  struct IBoundingBoxData
  {
    /// @brief Width of the bounding box.
    float m_width;
    /// @brief Height of the bounding box.
    float m_height;
    /// @brief Whether the box is a trigger (detects overlaps without
    ///        blocking movement).
    bool  m_isTrigger;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IBoundingBoxData,m_width,m_height,m_isTrigger)
  };

  /**
   * @brief Serializable animation data of an object.
   */
  struct IAnimatorData
  {
    /// @brief Key of the animation.
    std::string m_key;
    /// @brief Number of frames of the animation.
    int m_frames;
    /// @brief Step between frames of the animation.
    int m_step;
    /// @brief Playback speed of the animation.
    float m_speed;
    /// @brief Scale applied to the animation.
    float m_scale;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IAnimatorData,m_key,m_frames,m_step,m_speed,m_scale)
  };

  /**
   * @brief Serializable sprite data of an object.
   */
  struct ISpriteData
  {
    /// @brief Key of the sprite texture.
    std::string m_keyName;
    /// @brief Width of the sprite, in pixels.
    int m_width;
    /// @brief Height of the sprite, in pixels.
    int m_height;
    /// @brief Scale applied to the sprite.
    float m_scale;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ISpriteData,m_keyName,m_width,m_height,m_scale)
  };

  /**
   * @brief Serializable gameplay stats of an object.
   *
   * All the stats are unsigned 8-bit values (0 to 255).
   */
  struct IStatsData
  {
    /// @brief Current health points.
    uint8_t m_hp;
    /// @brief Maximum health points.
    uint8_t m_hp_max;
    /// @brief Strength.
    uint8_t m_str;
    /// @brief Defense.
    uint8_t m_def;
    /// @brief Agility.
    uint8_t m_agi;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IStatsData, m_hp,m_hp_max,m_str,m_def,m_agi)
  };

  /**
   * @brief Serializable transform data of an object.
   */
  struct ITransformData
  {
    /// @brief Current position.
    Vector2 m_position;
    /// @brief Position in the previous frame.
    Vector2 m_prev_position;
    /// @brief Current velocity.
    Vector2 m_velocity;
    /// @brief Direction the object is facing or moving in.
    Vector2 m_direction;
    /// @brief Rotation angle of the object.
    float   m_angle;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ITransformData, m_position,m_prev_position,m_velocity,m_direction,m_angle)
  };

  /**
   * @brief Serializable text data of an object.
   */
  struct ITextData
  {
    /// @brief Text content.
    std::string m_text;
    /// @brief Font size used to render the text.
    float m_fontSize;
    /// @brief Key of the font used to render the text.
    std::string m_fontKey;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ITextData, m_text,m_fontSize,m_fontKey)
  };

  /**
   * @brief Complete saved state of an object.
   *
   * ObjectState is a snapshot of an object made of its name, layer and the
   * data of each of its components. The transform and stats data are always
   * present; the sprite, animator, color and text data are optional, and an
   * object can have any number of bounding boxes.
   */
  struct ObjectState
  {
    /// @brief Name of the object.
    std::string       m_name;
    /// @brief Layer the object belongs to.
    uint8_t           m_layer;
    /// @brief Transform data of the object.
    ITransformData    m_tData;
    /// @brief Stats data of the object.
    IStatsData        m_sData;
    /// @brief Sprite data, if the object has a sprite.
    std::optional<ISpriteData>      m_spData;
    /// @brief Animator data, if the object has an animator.
    std::optional<IAnimatorData>    m_anData;
    /// @brief Bounding boxes of the object. Empty if it has none.
    std::vector<IBoundingBoxData>   m_bbData;
    /// @brief Color data, if the object has a color.
    std::optional<IColorData>       m_cData;
    /// @brief Text data, if the object is a text.
    std::optional<ITextData>        m_txData;
  };

  /**
   * @brief Keeps the saved state of the world's objects and stores it on disk.
   *
   * WorldSaver holds an ObjectState for each object, indexed by a string
   * key, and can write all of them to a file and read them back.
   */
  class WorldSaver
  {
    public:
      /**
       * @brief Stores the state of an object.
       *
       * The state is stored under the object's name (ObjectState::m_name).
       *
       * @param[in] obj State of the object to store.
       */
      void SaveObject(const ObjectState& obj);
      /**
       * @brief Returns the stored state of an object.
       *
       * @param[in] key Key the object was stored with.
       * @return Pointer to the stored state, or nullptr if there is none with
       *         that key. The state is owned by the WorldSaver.
       */
      ObjectState* GetObject(const std::string& key);
      /**
       * @brief Tells whether the state of an object is stored.
       *
       * @param[in] key Key to look for.
       * @return true if a state is stored under that key, false otherwise.
       */
      bool HasObject(const std::string& key) const;
      /**
       * @brief Removes the stored state of an object.
       *
       * @param[in] key Key the object was stored with.
       */
      void RemoveObject(const std::string& key);
      /**
       * @brief Removes every stored object state.
       */
      void Clear(void);
      /**
       * @brief Writes the stored states to a file.
       *
       * @param[in] path Path of the file to write.
       * @return true if the file was written successfully, false otherwise.
       */
      bool SaveToDisk(const std::string& path);
      /**
       * @brief Reads the stored states from a file.
       *
       * @param[in] path Path of the file to read.
       * @return true if the file was read successfully, false otherwise.
       */
      bool LoadFromDisk(const std::string& path);
    private:
      /// @brief Converts the stored object states into the JSON document.
      void to_json();
      /// @brief Fills the stored object states from the JSON document.
      void from_json();
    private:
      /// @brief Stored object states, indexed by key.
      std::unordered_map<std::string, ObjectState, StringHash, std::equal_to<>> m_ObjectSates;
      /// @brief JSON document used to write to and read from disk.
      nlohmann::json data;
  };
}
