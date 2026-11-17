// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Services/KeyHash.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "nlohmann/json.hpp"
// | -------------------------------
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
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
    float   m_width = 0.0f;
    /// @brief Height of the bounding box.
    float   m_height = 0.0f;
    /// @brief Whether the box is a trigger (detects overlaps without
    ///        blocking movement).
    bool    m_isTrigger = false;
    /// @brief Offset of the box from the object position.
    Vector2 m_offset{0.0f, 0.0f};
    /// @brief Enables JSON conversion of all the members. Missing keys keep
    ///        their default value, so older saves still load.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(IBoundingBoxData,m_width,m_height,m_isTrigger,m_offset)
  };

  /**
   * @brief Serializable animation data of an object.
   */
  struct IAnimatorData
  {
    /// @brief Key of the animation.
    std::string m_key;
    /// @brief Key of the atlas.
    std::string m_keyAtlas;
    /// @brief Number of frames of the animation.
    int m_frames = 0;
    /// @brief Step between frames of the animation.
    int m_step = 1;
    /// @brief Playback speed of the animation.
    float m_speed = 0.0f;
    /// @brief Scale applied to the animation.
    float m_scale = 1.0f;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(IAnimatorData,m_key,m_keyAtlas,m_frames,m_step,m_speed,m_scale)
  };

  /**
   * @brief Serializable sprite data of an object.
   */
  struct ISpriteData
  {
    /// @brief Key of the texture, or of the atlas when m_animKey / m_tileID is set.
    std::string m_keyName;
    /// @brief Width of the sprite, in pixels (informative; rebuilt on load).
    int m_width = 0;
    /// @brief Height of the sprite, in pixels (informative; rebuilt on load).
    int m_height = 0;
    /// @brief Scale applied to the sprite.
    float m_scale = 1.0f;
    /// @brief Atlas animation the sprite was built from (empty if none).
    std::string m_animKey;
    /// @brief Atlas tile the sprite was built from (empty if none).
    std::string m_tileID;
    /// @brief Enables JSON conversion of all the members. Missing keys keep
    ///        their default value, so older saves still load.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ISpriteData,m_keyName,m_width,m_height,m_scale,m_animKey,m_tileID)
  };

  /**
   * @brief Serializable transform data of an object.
   */
  struct ITransformData
  {
    /// @brief Current position.
    Vector2 m_position{0.0f, 0.0f};
    /// @brief Position in the previous frame.
    Vector2 m_prev_position{0.0f, 0.0f};
    /// @brief Current velocity.
    Vector2 m_velocity{0.0f, 0.0f};
    /// @brief Direction the object is facing or moving in.
    Vector2 m_direction{0.0f, 0.0f};
    /// @brief Direction in the previous frame.
    Vector2 m_prev_direction{0.0f, 0.0f};
    /// @brief Rotation angle of the object.
    float   m_angle = 0.0f;
    /// @brief Enables JSON conversion of all the members. Missing keys keep
    ///        their default value, so older saves still load.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ITransformData, m_position,m_prev_position,m_velocity,m_direction,m_prev_direction,m_angle)
  };

  /**
   * @brief Serializable circle collider data of an object.
   */
  struct ICircleBoxData
  {
    /// @brief Radius of the circle.
    float m_radius = 0.0f;
    /// @brief Whether the circle is a trigger.
    bool  m_isTrigger = false;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(ICircleBoxData, m_radius, m_isTrigger)
  };

  /**
   * @brief Serializable material data of an object (its shader, by key).
   */
  struct IMaterialData
  {
    /// @brief Key of the shader in the ShaderManager.
    std::string m_shaderKey;
    /// @brief Enables JSON conversion of all the members.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(IMaterialData, m_shaderKey)
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
   * data of each of its components. The transform data is always present;
   * the sprite, animator, bounding box, circle box, color, text and material
   * data are optional
   * (std::nullopt when the object lacks that component). Components of the
   * game are stored as json in m_components (see ComponentSerializers).
   */
  struct ObjectState
  {
    /// @brief Name of the object.
    std::string       m_name;
    /// @brief Layer the object belongs to.
    uint8_t           m_layer = 0;
    /// @brief Render space of the object (0 = World, 1 = Screen).
    uint8_t           m_renderSpace = 0;
    /// @brief Whether the object is drawn.
    bool              m_visible = true;
    /// @brief Transform data of the object.
    ITransformData    m_tData;
    /// @brief Sprite data, if the object has a sprite.
    std::optional<ISpriteData>      m_spData;
    /// @brief Animator data, if the object has an animator.
    std::optional<IAnimatorData>    m_anData;
    /// @brief Bounding box data, if the object has an IBoundingBox.
    std::optional<IBoundingBoxData> m_bbData;
    /// @brief Color data, if the object has a color.
    std::optional<IColorData>       m_cData;
    /// @brief Text data, if the object is a text.
    std::optional<ITextData>        m_txData;
    /// @brief Circle collider data, if the object has an ICircleBox.
    std::optional<ICircleBoxData>   m_ccData;
    /// @brief Material data, if the object has an IMaterial.
    std::optional<IMaterialData>    m_mtData;
    /// @brief Game components registered in ComponentSerializers, by key.
    std::unordered_map<std::string, nlohmann::json> m_components;
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
      std::unordered_map<std::string, ObjectState, StringHash, std::equal_to<>> m_objectSates;
      /// @brief JSON document used to write to and read from disk.
      nlohmann::json data;
  };
}
