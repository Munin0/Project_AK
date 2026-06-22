// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "SDL3/SDL_video.h"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  class Render
  {
    public:
      static Render& Get(void);
      static void Destroy(void);

      int InitWindowSDLContext(std::string title, int w, int h);
      void InitRenderContext(void);
      bool InitGLContext(void) const;
      void ClearRender(void) const;
      void UpdateRender(void);
      void DestroyWindowSDLContext(void);

      SDL_Window* GetWindow(void);
      void SetScreenSize(float w, float h);
      Vector2 GetScreenSize(void) const;

      void StartDraw();
      void EndDraw();
      Batcher& GetBatcher(void);
      void DestroyBatch(void);

      void ToggleFullscreen(void);

      void RenderColor(const Color& c);
    protected:
      float wW, wH;
    public:
      bool _toggleFullscreen;
    private:
      SDL_Window* window = nullptr;
      SDL_GLContext glContext = nullptr;
      const SDL_DisplayMode* display;
      SDL_DisplayID* dsplaysIDs = 0;
      Batcher batch;
    private:
      explicit Render() = default;
      static inline Render* _render = nullptr;
  };
}
