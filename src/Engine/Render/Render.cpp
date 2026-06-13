// | -------------------------------
#include "Render.hpp"
// | -------------------------------
#include "RColor.hpp"
#include "RBatch.hpp"
// | -------------------------------
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <glad/glad.h>
// | -------------------------------
#include <SDL3/SDL_init.h>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_video.h"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  Render& Render::Get(void)
  {
    if(!_render)
      _render = new Render();
    return *_render;
  }

  void Render::Destroy(void)
  {
    delete _render;
  }

  void Render::InitRenderContext(void)
  {
    LOG_INFO(" | << Init Render context");
    int amountDisplays=0;
    this->dsplaysIDs = SDL_GetDisplays(&amountDisplays);
    LOG_INFO(" | << Amount of Displays: " + std::to_string(amountDisplays));
    this->display = SDL_GetCurrentDisplayMode(this->dsplaysIDs[0]);
    LOG_INFO(" | << Display size; Width: " +std::to_string(this->display->w) + " Height: "+std::to_string(this->display->h));
    this->_toggleFullscreen = true;
    glViewport(0,0,display->w,display->h);
    batch.Init(); 
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
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    this->window = SDL_CreateWindow(title.c_str(), w, h, SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    if(!this->window)
    {
      LOG_FATAL("| <<    Error creating Windows" + static_cast<std::string>(SDL_GetError()));
      return false;
    }
    
    this->glContext = SDL_GL_CreateContext(this->window);
    if(!this->glContext)
    {
      LOG_FATAL(" | <<   Error creating glContext" );
      return false;
    }
    SDL_GL_SetSwapInterval(1);
    return true;
  }
  
  void Render::StartDraw(void)
  {
    batch.Begin();
  }

  void Render::EndDraw(void)
  {
    batch.End();
  }

  void Render::DestroyBatch()
  {
    batch.Shutdown();
  }

  Batcher& Render::GetBatcher(void)
  {
    return this->batch;
  }

  void Render::ClearRender(void) const
  {
    SDL_GL_SwapWindow(this->window);
  }

  void Render::UpdateRender(void)
  {
    SDL_SetWindowFullscreen(this->window,_toggleFullscreen);
    if(!_toggleFullscreen)
    {
      this->wW =  1080;
      this->wH = 720;
    }
    if(_toggleFullscreen)
    {
      int w,h;
      SDL_GetWindowSize(this->window, &w,&h);
      this->wH = h;
      this->wW = w;
    }
    glViewport(0,0,this->wW,this->wH);
  }

  void Render::DestroyWindowSDLContext(void)
  {
    SDL_GL_DestroyContext(this->glContext);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
  }

  void Render::ToggleFullscreen(void)
  {
  }

  // | ----------------------------------- Getters && Setters Context
  SDL_Window* Render::GetWindow(void)
  {
    return this->window;
  }

  void Render::SetScreenSize(float w, float h)
  {
    this->wW = w;
    this->wH = h;
  }

  Vector2 Render::GetScreenSize(void) const
  {
    return {this->wW,this->wH};
  }

  // | ----------------------------------- Color Context
  void Render::RenderColor(const Color& c)
  {
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  //
}
