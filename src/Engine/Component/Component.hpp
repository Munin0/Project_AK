/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Image/RImage.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
/// | ------------------------------------ |

namespace ENG
{
  //! Father class for all componentes
  class IComponents
  {
    public:
      virtual ~IComponents()=default;
  };

  //! ITransform, control movement, velocity, direction of the Object.
  //! ALL objects have it mandatory
  class ITransform: public IComponents
  {
    public:
      //! Defatul constructor.
      ITransform();
      //! Constructor with Position, Velocity and Angle.
      ITransform(const Vector2& pos, const Vector2& vel, float ang);
      //! Defatult destructor
      ~ITransform() override {}

    public:
      Vector2 m_position;         //! The Actual position of the Object.
      Vector2 m_prev_position;    //! The prev position of the Object (1 Frame behind).
      Vector2 m_velocity;         //! Velocity from the object, (X, Y).
      Vector2 m_direction;        //! Direction from the object, (X , Y).
      float   m_angle;            //! Rotation of the object (NOT WORKING RIGHT NOW)
  };

  //! IStats, control stats of the Object.
  //! Right now you can "Kill" or erease an Object with the 'hp' member
  //! | Why not have constructor.
  //! > Because we dont need it, this objects is just a package of the stats,
  //! > Actually mandatory on all objects.
  class IStats : public IComponents
  {
    public:
      ~IStats() override {}
      /*
       * | Why using uint8_t.
       * > Caps the stat to only 1 byte (8 bits), so only 0 to 255 point of life.
       */
      uint8_t m_hp;
      uint8_t m_hp_max;
      uint8_t m_str;
      uint8_t m_def;
      uint8_t m_agi;
  };

  //! ISprite, control the graphics to draw on the engine, this are only reference to the sprites.
  class ISprite : public IComponents
  {
    public:
      //! Only for have one texture, its going yo be unique.
      ISprite(const std::string& key, float scale = 1.0f);
      //! Only for atlas and animation draw logic/
      ISprite(const std::string& atlasKey, const std::string& animName, int frameIndex, float scale = 1.0f);
      //! Only for the atlas.
      ISprite(const std::string& atlasKey, const std::string& tileID, float scale = 1.0f);
      //! Defatult destructor.
      ~ISprite() override {}

      //! Get the key from the loaded texture.
      const std::string& GetKeyName(void) const {return this->m_keyTexture;}
      //! Get a const texture from the texture.
      const std::shared_ptr<RImage>& GetImage(void) const {return  this->m_image;}
      //! Get the with from the texture.
      int GetWidth(void) const {return  this->m_width;}
      //! Get the height from the texture.
      int GetHeight(void) const {return  this->m_height;}
      //! Get the scale, if you dont put a scale is 1.0f.
      float GetScale(void) const {return  this->m_scale;}

      //! Verify if the texture loaded is a Atlas. (Boolean)
      bool IsAtlas(void) const {return this->m_isAtlas;}
      //! Get the AtlasLayer. (GPU information)
      int GetAtlasLayer(void) const {return this->m_atlasLayer;}
      //! Get the section of the Atlas to draw. (Example: Frame to animation or the Tile)
      const UVRect& GetUV(void) const {return this->m_uv;}

      //! Set the actual frame form the animation to draw and Update the UVRect to draw.
      void SetFrame(int frameIndex);
    private:
      std::string             m_keyTexture; //! String Key to take the original texture from the Service::Assets().
      std::shared_ptr<RImage> m_image;      //! Shared_ptr.

      int   m_width;    //! Width of the original texture.
      int   m_height;   //! Height of the original texture.
      float m_scale;    //! Scale of the sprite to draw.

      bool    m_isAtlas = false;      //! The texture is Atlas or not.
      int     m_atlasLayer = -1;      //! Layer of the Atlas. (GPU information)
      UVRect  m_uv{};                 //! Secction of the Atlas to draw.
      std::string m_animationKey;     //! Key of the animation to draw.
      std::string m_tileID;           //! Tile of the atlas to draw.
  };
    
  //! IAnimator, takes the logic of how the animation works.
  //! | Why IAnimator need ISprite to work.
  //! > Because IAnimator is only the logic and the ISprite is the logic to Draw and give to the batching what to draw.
  class IAnimator : public IComponents
  {
    public:
      //! Constructor "Defatult", this create automatic the rectangles to draw.
      IAnimator(std::string key, int frames, float speed, int step = 1, float scale = 1.0f);
      //! Constructor needs the rectables to manage the animation.
      IAnimator(std::string key, std::vector<ENG::Rectangle> _rects, float speed, int step = 1, float scale = 1.0f);
      //! Defatult destructor.
      ~IAnimator() override {}

      //! Play the animation from the begin.
      void Play(void);
      //! Stop the animation completely.
      void Stop(void);
      //! Pause the animator, stop the animation right were is right now.
      void Pause(void);
      //! Continues the animation from were was pause.
      void Resume(void);

      //! Advance the animation, validating if the animation can advance.
      void Advance(float dt);
      //! GetThe current frame where is playin the animation.
      int GetCurrentFrame(void) const {return this->m_currentFrame;}

      //! Get the name from the animation
      const std::string& GetName(void)const {return this->m_keyAnimation;}
      //! Get the amount of frames to pass.
      int GetFrames(void) const  { return this->m_frames;  }
      //! Get how many frames need to pass (If step is 1.0f, pass frame per frame).
      int GetStep(void) const    { return this->m_step;    }
      //! Get the speed of the animation.
      float GetSpeed(void) const { return this->m_speed;   }
      //! Get the scale to draw the animation (Texture scale).
      float GetScale(void) const { return this->m_scale;   }

    private:
      std::string m_keyAnimation;                 //! Key to get the texture, from the Service::Assets().
      std::vector<ENG::Rectangle> m_rectangles;   //! Rectangles of the animation.
      int         m_frames        = 0;            //! Amount of animation to draw.
      int         m_step          = 1;            //! How many frames advance.
      float       m_speed         = 0.0f;         //! Speed of the animation.
      float       m_scale         = 0.0f;         //! Scale of the animation to draw.
      float       m_elapsed       = 0.0f;         //! How much time passed between last frame to the actual to meet the animation schedule.
      bool        m_playing       = false;        //! If the animation is playing.
      int         m_currentFrame  = 1.0f;         //! Current frame of the animation.
  };

  //! IBoundingBox, Vector2 position and Vector2 size.
  //! Used fot the physics system
  class IBoundingBox: public IComponents
  {
    public:
      //! Unique constructor, Dimention and if isTrigger (Defatult is solid).
      //! explicit because, you always need to pass a IBoundingBox.
      //! Example: void sum(const IBoundingBox& bb), when sum({{3,4},false});, this not work.
      //! need to explicit make the object: sum( IBoundingBox{{3,4},false} );
      explicit IBoundingBox(const Vector2& dim, bool isTrigger = false);
      ~IBoundingBox() override {}

      //! Position is the same anchor Object/Batcher::DrawQuad use to draw a sprite (its
      //! bottom-left corner), not the box's center -- this keeps the box aligned with what's
      //! actually drawn on screen.
      void Update(const Vector2& _position)   { this->m_position = _position; };
      void Update(float x, float y)           { this->m_position = {x,y};     };
      //! Change the dimention from the IBoundingBox.
      void ChangeSize(const Vector2& dim)     { this->m_size = dim;     }
      //! Get the actual size from the IBoundingBox.
      const Vector2& GetSize(void) const      { return this->m_size;    }
      //! Get the position from the IBoundingBox.
      const Vector2& GetPosition(void) const  { return this->m_position;}
      //! Get the center from the IBoundingBox.
      Vector2 GetCenter(void) const           { return this->m_position + this->m_size / 2.0f;}

      //! Validate if the IBoundingBox was created for be a TriggerBox. 
      bool IsTrigger(void) const {return this->m_isTrigger;}
      //! You can change a IBoundingBox from Solid to Trigger.
      void SetTrigger(bool _isTrigger) {this->m_isTrigger = _isTrigger;}
    private:
      Vector2 m_position;
      Vector2 m_size;
      bool    m_isTrigger;
  };

  //! IColor, is a storage for the object to save a color, change it and serialization
  class IColor : public IComponents
  {
    public:
      //! Constructor, you can give a color. (Example: ENG::Color::White).
      IColor(const Color& c);
      //! Constructor, you can create a color from the RGBA. (A can be avoided, from default is 1.0f).
      IColor(float r, float g, float b, float a = 1.0f);
      //! Defatult destructor.
      ~IColor() override {}
        
      //! You can change the actual color with a ENG::Color.
      void ChangeColor(const Color& _color);
      //! You can change the actual color with the RGBA. 
      void ChangeColor(float r, float g, float b, float a = 1.0f);
      //! Get the color stored.
      const Color& GetColor(void) const { return this->m_color;}
    private:
      Color m_color; //! ENG::Color.
  };

  //! IMaterial, this class storage one shader.
  //! Automatically draw the shader if have one.
  class IMaterial : public IComponents
  {
    public:
      //! Defatult constructor, takes a Shader from Service::Shader() with the key.
      IMaterial(const std::string& idKey);
      //! Gets the stored shader, if have one.
      Shader* GetShader(void)const;
    private:
      Shader* m_shader; //! Shader to storage
  };

  //! IText, text storage
  class IText : public IComponents
  {
    public:
      //! Constructor, need the text to storage.
      IText(const std::string& text);

      //! You can change the original text to a new one.
      void SetText(const std::string& _text)
      {
        this->m_text = _text;
      }

      //! Can change the size the font size.
      void SetFontSize(float _size)
      {
        this->m_fontSize = _size;
      }

      //! If you want to draw text, need a Font to draw it. MANDATORY.
      //! You cant taked from a Service::Font() or make it yourself.
      void SetFont(Font* _font)
      {
        this->m_font = _font;
      }

      //! Get the text stored
      std::string GetText() const
      {
        return this->m_text;
      }

      //! Get the font size.
      float GetFontSize() const
      {
        return this->m_fontSize;
      }

      //! Get the Font* if have one.
      Font* GetFont() const
      {
        if(m_font)
          return this->m_font;
        else
        {
          LOG_ERROR(" | << GetFont(), doesnt have a Font*: " + m_text);
          return nullptr;
        }
      }
    private:
      std::string m_text;               //! Text to storage.
      Font*       m_font = nullptr;     //! Font ptr.
      float       m_fontSize = 0.0f;    //! Font size.
  };

  //! IDialog, system for make dialogs for the game.
  class IDialog : public IComponents
  {
    public:
      //! Constructor, give the information to take the text from Service::CSV().
      IDialog(const std::string& keyDialogs, int text);
      //! Constructor, give the information to take a range of text's from Service::CSV().
      IDialog(const std::string& keyDialogs, int fromText, int whereText);
      //! Constructor, give the information to take the text from Service::CSV(). (String form)
      IDialog(const std::string& keyDialogs, const std::string& dialog);
      //! Constructor, give the information to take a range of text's from Service::CSV(). (String form)
      IDialog(const std::string& keyDialogs, const std::string& from, const std::string& where);

      //! Set the velocity for the dialog.
      void SetSpeed(float s);
      //! Set the step, this is for oh many characters can pass trough.
      void SetStep(int step);

      //! Get if the dialog was completed
      bool IsCompleteDialog(void) const;

      //! Advance on the dialog. (If can do it)
      void Advance(void) const;
      //! Update the IDialog object.
      void Update(void);
      //! Draw the text logic.
      void Draw(void) const;

    private:
      bool    m_complete;
      float   m_speed;
      int     m_step;
      int     m_amountText;
  };
}
