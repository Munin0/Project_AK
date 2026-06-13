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

    class IName : public IComponents
    {
      public:
        explicit IName(const std::string& name);
        ~IName() override{};
      public:
        std::string name;
    };

    class ITransform: public IComponents
    {
      public:
        ITransform();
        ITransform(const Vector2& pos, const Vector2& vel, float ang);
        ~ITransform() override{};
      public:
        Vector2 position;
        Vector2 prev_position;
        Vector2 velocity;
        Vector2 direction;
        float   angle;
    };

    class ITimer : public IComponents
    {
      public:
        ITimer();
        ~ITimer() override{};
        int actualFrame;
    };
    
    class IStats : public IComponents
    {
      public:
        ~IStats()override{};
        uint8_t hp;
        uint8_t hp_max;
        uint8_t str;
        uint8_t def;
        uint8_t agi;
    };

    class IText : public IComponents
    {
      public:
        IText(const std::string& texto);
        ~IText()override{};

    };

    class ISprite : public IComponents
    {
      public:
        ISprite(const std::string& _keyName, float scale);
        ~ISprite() override{};
      public:
        std::string keyName;
        std::shared_ptr<RImage> image;
        int width;
        int height;
        float scale;
    };
    
    class IAnimator : public IComponents
    {
      public:
        IAnimator(int frames, float speed, int step, float scale);
        IAnimator(std::vector<ENG::Rects> _rects, float speed, int step, float scale);
        ~IAnimator() override;

        void Play(void);
        void Stop(void);
        void Pause(void);
        void Resume(void);

      private:
        int frames  = 0;
        int step    = 0;
        float speed = 0;
        float scale = 0.f;

        std::vector<ENG::Rects> rectangles;
    };
      
    class IShader : public IComponents
    {
      public:
        enum class ShaderVars
        {
          FLOAT,
          VEC2F,
          VEC3F,
          VEC4F,
          MAT3,
          MAT4,
          TEX
        };

      public:
        explicit IShader(const std::string& vert, const std::string& frag);
        ~IShader() override{};
        // void setEscalar(const std::string& key, float* valor);
        // void setVector2(const std::string& key, sf::Vector2f* valor);
        // void setVector3(const std::string& key, sf::Vector3f* valor);
        // void setVector4(const std::string& key, sf::Glsl::Vec4* valor);
        // void setMat3(const std::string& key, sf::Glsl::Mat3* valor);
        // void setMat4(const std::string& key, sf::Glsl::Mat4* valor);
        // void setTextura(const std::string& key, sf::Texture* valor);
        // void cambiarShader(const std::string& vert, const std::string& frag);
      public:
        std::string m_fragshaderFile;
        std::string m_vertshaderFile;
        //al sacarlo hay que castear a un tipo de dato
        //m_vars["tiempo"] = &tiempo; (float*)m_vars["tiempo"];
        //m_vars["textura"] = &sprite->getTexture(); (sf::Texture*)m_vars["textura"];
        // std::map<std::string,std::pair<IShader::ShaderVars,void*>> m_vars;
    };

    //Control
    class IControl : public IComponents
    {
      public:
        explicit IControl();
        ~IControl() override{};
      public:
        //si tiene muchos botones usar un
        //arreglo o un entero de 32bits para
        //32 botones y con mascaras
        bool arr;
        bool abj;
        bool izq;
        bool der;
        bool run;
        bool jmp;
        bool acc;
        bool sacc;
      public:
        void SetActive(bool a){activo=a;}
        bool IsActived()const{return activo;}
      private:
        bool activo{true};
    };

    class IBoundingBox: public IComponents
    {
      public:
        explicit IBoundingBox(const Vector2& dim);
        Vector2 tam;
        Vector2 mitad;
    };

    // class IRespawn: public IComponents
    // {
    //   public:
    //     IRespawn(std::vector<std::shared_ptr<IComponents>>& lista_comp, int max);
    //     IRespawn(int max,int w, int h); // Entity
    //     int timer_actual;
    //     int timer_maximo;
    //     int num_objetos;
    //
    //     int width;
    //     int height;
    //     int curr_spawn{0};
    //     int max_spawn;
    //     std::vector<std::shared_ptr<IComponents>> componentes;
    // };
    //
    // class IPaths : public IComponents
    // {
    //   public:
    //     explicit IPaths(int total_frames);
    //     void AddCurve(Vector2 p1, Vector2 p2, Vector2 p3);
    //   public:
    //     std::vector<Vector2> puntos;
    //     int offset;
    //     int id_curva;
    //     int frame_total_curva;
    //     int frame_actual_curva;
    // };

  class IColor : public IComponents
  {
    public:
      IColor(const Color& c);
      IColor(float r, float g, float b, float a = 1.0f);

      void SetOutColor(const Color& c);
      void SetOutColor(float r, float g, float b, float a = 1.0f);
    private:
      Color color;
      Color outColor;
  };
}
