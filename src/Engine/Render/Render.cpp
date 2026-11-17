// | -------------------------------
#include "Render.hpp"
#include "Engine/Render/Camera/Camera2D.hpp"
// | -------------------------------
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Utils/Config.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <glad/glad.h>
// | -------------------------------
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_video.h"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  Render& Render::Get(void)
  {
    if(!m_render)
      m_render = new Render();
    return *m_render;
  }

  void Render::Destroy(void)
  {
    delete m_render;
  }

  void Render::InitRenderContext(EngineConfig& eConfig)
  {
    LOG_INFO(" | << Init Render context");
    int amountDisplays=0;
    this->m_displayId = SDL_GetDisplays(&amountDisplays);
    LOG_INFO(" | << Amount of Displays: " + std::to_string(amountDisplays));
    this->m_display = SDL_GetCurrentDisplayMode(this->m_displayId[0]);
    LOG_INFO(" | << Display size; Width: " +std::to_string(this->m_display->w) + " Height: "+std::to_string(this->m_display->h));
    this->m_fullscreen = eConfig.m_eFullscreen;
    if(this->m_fullscreen)
      glViewport(0,0,m_display->w,m_display->h);
    LOG_INFO(" | << Creating BatchingContext");
    m_batch.Init(); 
  }
  
  bool Render::InitGLContext(void) const
  {
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
      LOG_FATAL("Glad load GLLoader failed");
      return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
  }

  int Render::InitWindowSDLContext(std::string title, int w, int h)
  {
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
      LOG_FATAL(" | << SDL_Init failed: " + std::string(SDL_GetError()));
      return false;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#if defined(__APPLE__)
    // macOS only hands out a Core context if it's also marked forward-compatible
    // (i.e. no legacy/deprecated GL functionality); other platforms ignore this flag.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    this->m_window = SDL_CreateWindow(title.c_str(), w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    if(!this->m_window)
    {
      LOG_FATAL("| <<    Error creating Windows" + static_cast<std::string>(SDL_GetError()));
      return false;
    }
    
    this->m_glContext = SDL_GL_CreateContext(this->m_window);
    if(!this->m_glContext)
    {
      LOG_FATAL(" | <<   Error creating glContext" );
      return false;
    }
    SDL_GL_SetSwapInterval(1);
    return true;
  }
  
  void Render::StartDraw(void)
  {
    m_batch.Begin();
  }

  void Render::EndDraw(void)
  {
    m_batch.End();
  }

  void Render::DestroyBatch()
  {
    m_batch.Shutdown();
  }

  Batcher& Render::GetBatcher(void)
  {
    return this->m_batch;
  }

  void Render::ClearRender(void) const
  {
    SDL_GL_SwapWindow(this->m_window);
  }

  void Render::UpdateRender(void)
  {
    // Only touch the window when the requested mode differs from the real one:
    // calling SDL_SetWindowFullscreen every frame is wasteful and can flicker.
    const bool isFullscreen = (SDL_GetWindowFlags(this->m_window) & SDL_WINDOW_FULLSCREEN) != 0;
    if (isFullscreen != m_fullscreen)
      SDL_SetWindowFullscreen(this->m_window, m_fullscreen);

    // Logical size: used by the projections (screen and camera).
    int w = 0, h = 0;
    SDL_GetWindowSize(this->m_window, &w, &h);
    this->m_windowsWidth  = static_cast<float>(w);
    this->m_windowsHeight = static_cast<float>(h);

    // Pixel size: what glViewport needs (differs from the logical size on HiDPI / scaled displays).
    int pw = 0, ph = 0;
    SDL_GetWindowSizeInPixels(this->m_window, &pw, &ph);
    glViewport(0, 0, pw, ph);

    // Keep the active camera's viewport in sync with the window, so changing
    // the window size or toggling fullscreen does not stretch or cut the view.
    if (Camera2D* cam = m_batch.GetCamera2D())
      cam->Resize(this->m_windowsWidth, this->m_windowsHeight);
  }

  void Render::DestroyWindowSDLContext(void)
  {
    SDL_GL_DestroyContext(this->m_glContext);
    SDL_DestroyWindow(this->m_window);
    SDL_Quit();
  }

  void Render::ToggleFullscreen(void)
  {
    this->m_fullscreen = !this->m_fullscreen;
  }

  // | ----------------------------------- Getters && Setters Context
  SDL_Window* Render::GetWindow(void)
  {
    return this->m_window;
  }

  void Render::SetScreenSize(float w, float h)
  {
    this->m_windowsWidth  = w;
    this->m_windowsHeight = h;
  }

  Vector2 Render::GetScreenSize(void) const
  {
    return {this->m_windowsWidth,this->m_windowsHeight};
  }

  bool Render::GetFullscreen(void) const
  {
    return m_fullscreen;
  }

  void Render::SetFullscreen(bool _)
  {
    this->m_fullscreen = _;
  }

  // | ----------------------------------- Color Context
  void Render::RenderColor(const Color& c)
  {
    glClearColor(c.m_r, c.m_g, c.m_b, c.m_a);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  //
}
