/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Image/RImage.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
/// | ------------------------------------ |

namespace ENG
{
  class IComponents
  {
    public:
      virtual ~IComponents()=default;
  };

  class ITransform: public IComponents
  {
    public:
      ITransform();
      ITransform(const Vector2& pos, const Vector2& vel, float ang);
      ~ITransform() override {}
    public:
      Vector2 position;
      Vector2 prev_position;
      Vector2 velocity;
      Vector2 direction;
      float   angle;
  };

  class IStats : public IComponents
  {
    public:
      ~IStats() override {}
      uint8_t hp;
      uint8_t hp_max;
      uint8_t str;
      uint8_t def;
      uint8_t agi;
  };

  class ISprite : public IComponents
  {
    public:
      ISprite(const std::string& _keyName, float scale);
      ISprite(const std::string& atlasKey, const std::string& animName, int frameIndex, float scale = 1.0f);
      ISprite(const std::string& atlasKey, const std::string& tileID, float scale = 1.0f);
      ~ISprite() override {}

      const std::string& GetKeyName(void) const {return  this->keyName;}
      const std::shared_ptr<RImage>& GetImage(void) {return  this->image;}
      int GetWidth(void) const {return  this->width;}
      int GetHeight(void) const {return  this->height;}
      float GetScale(void) const {return  this->scale;}

      bool IsAtlas(void) const {return this->isAtlas;}
      int GetAtlasLayer(void) const {return this->atlasLayer;}
      const UVRect& GetUV(void) const {return this->uv;}

      void SetFrame(int frameIndex);
    private:
      std::string keyName;
      std::shared_ptr<RImage> image;
      int width;
      int height;
      float scale;

      bool isAtlas = false;
      int atlasLayer = -1;
      UVRect uv{};
      std::string animName;
      std::string tileID;
  };
    
  class IAnimator : public IComponents
  {
    public:
      IAnimator(std::string key,int frames, float speed, int step, float scale = 1.0f);
      IAnimator(std::string key, std::vector<ENG::Rect> _rects, float speed, int step, float scale = 1.0f);
      ~IAnimator() override {}

      void Play(void);
      void Stop(void);
      void Pause(void);
      void Resume(void);

      void Advance(float dt);
      int GetCurrentFrame(void) const {return this->currentFrame;}

      const std::string& GetName(void)const {return this->aName;}
      int GetFrames(void) const { return  this->frames;}
      int GetStep(void) const {return  this->step;}
      float GetSpeed(void) const {return this->speed;}
      float GetScale(void) const {return this->scale;}

    private:
      std::string aName;
      std::shared_ptr<RImage> asset;
      int frames  = 0;
      int step    = 0;
      float speed = 0;
      float scale = 0.f;

      std::vector<ENG::Rect> rectangles;

      int currentFrame = 0;
      float elapsed     = 0.0f;
      bool playing      = true;
  };

  class IBoundingBox: public IComponents
  {
    public:
      explicit IBoundingBox(const Vector2& dim, bool isTrigger = false);
      ~IBoundingBox() override {}

      // position is the same anchor Object/Batcher::DrawQuad use to draw a sprite (its
      // bottom-left corner), not the box's center -- this keeps the box aligned with what's
      // actually drawn on screen.
      void Update(const Vector2& _position) {this->position = _position;};
      void Update(float x, float y) {this->position = {x,y};};
      void ChangeSize(const Vector2& dim){ this->size = dim; }
      const Vector2& GetSize(void) const {return  this->size;}
      const Vector2& GetPosition(void) const {return this->position;}
      Vector2 GetCenter(void) const {return this->position + this->size / 2.0f;}

      // Trigger boxes report overlaps (see ResolveCollisions) but are never pushed apart;
      // non-trigger (solid) boxes get their overlaps resolved.
      bool IsTrigger(void) const {return this->isTrigger;}
      void SetTrigger(bool _isTrigger) {this->isTrigger = _isTrigger;}
    private:
      Vector2 position;
      Vector2 size;
      bool isTrigger;
  };

  class IColor : public IComponents
  {
    public:
      IColor(const Color& c);
      IColor(float r, float g, float b, float a = 1.0f);
      ~IColor() override {}
        
      void ChangeColor(const Color& _color);
      void ChangeColor(float r, float g, float b, float a = 1.0f);
      const Color& GetColor(void) const { return this->color;}
    private:
      Color color;
  };

  class IMaterial : public IComponents
  {
    public:
      IMaterial(const std::string& idKey);
      Shader* GetShader(void)const;
    private:
      Shader* shader;
  };
}
