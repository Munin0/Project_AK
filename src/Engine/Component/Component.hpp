/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Render/RImage.hpp"
#include "Engine/Render/RColor.hpp"
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
        ~ISprite() override {}

        const std::string& GetKeyName(void) const {return  this->keyName;}
        const std::shared_ptr<RImage>& GetImage(void) {return  this->image;}
        int GetWidth(void) const {return  this->width;}
        int GetHeight(void) const {return  this->height;}
        float GetScale(void) const {return  this->scale;}
      private:
        std::string keyName;
        std::shared_ptr<RImage> image;
        int width;
        int height;
        float scale;
    };
    
    class IAnimator : public IComponents
    {
      public:
        IAnimator(std::string key,int frames, float speed, int step, float scale);
        IAnimator(std::string key, std::vector<ENG::Rect> _rects, float speed, int step, float scale);
        ~IAnimator() override {}

        void Play(void);
        void Stop(void);
        void Pause(void);
        void Resume(void);
        
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
    };

    class IBoundingBox: public IComponents
    {
      public:
        explicit IBoundingBox(const Vector2& dim);
        ~IBoundingBox() override {}

        void ChangeSize(const Vector2& dim){ this->size = dim;}
        const Vector2& GetSize(void) const {return  this->size;}
        const Vector2& GetHalf(void) const {return this->half;}
        bool GetResolution(void) const {return this->resolution;}
      private:
        Vector2 size;
        Vector2 half;
        bool resolution;
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
}
