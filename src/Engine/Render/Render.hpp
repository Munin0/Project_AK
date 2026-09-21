// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Utils/Config.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "SDL3/SDL_video.h"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Rendering and window manager of the engine.
   *
   * Render is a singleton that owns the SDL window, the OpenGL context and
   * the Batcher used to draw. It also handles the screen size and the
   * fullscreen state.
   *
   * Access the instance through Render::Get() and release it with
   * Render::Destroy() during engine shutdown.
   */
  class Render
  {
    public:
      /**
       * @brief Returns the global Render instance.
       * @return Reference to the singleton instance.
       */
      static Render& Get(void);
      /**
       * @brief Destroys the global Render instance.
       *
       * Must be called once, at engine shutdown. Any reference previously
       * obtained through Get() becomes invalid after this call.
       */
      static void Destroy(void);

      /// @name Initialization and shutdown
      /// @{
      /**
       * @brief Creates the SDL window.
       *
       * @param[in] title Title of the window.
       * @param[in] w     Width of the window, in pixels.
       * @param[in] h     Height of the window, in pixels.
       * @return Result code of the initialization.
       */
      int InitWindowSDLContext(std::string title, int w, int h);
      /**
       * @brief Initializes the render context from the engine configuration.
       *
       * @param[in] eConfig Configuration of the engine (title, resolution,
       *                    fullscreen mode).
       */
      void InitRenderContext(EngineConfig& eConfig);
      /**
       * @brief Initializes the OpenGL context.
       *
       * @return true if the OpenGL context was initialized successfully,
       *         false otherwise.
       */
      bool InitGLContext(void) const;
      /**
       * @brief Destroys the SDL window and its associated resources.
       */
      void DestroyWindowSDLContext(void);
      /// @}

      /// @name Frame control
      /// @{
      /**
       * @brief Clears the screen for the next frame.
       */
      void ClearRender(void) const;
      /**
       * @brief Presents the rendered frame on the window.
       */
      void UpdateRender(void);
      /**
       * @brief Begins the drawing of a frame.
       *
       * @see EndDraw()
       */
      void StartDraw();
      /**
       * @brief Ends the drawing of a frame.
       *
       * @see StartDraw()
       */
      void EndDraw();
      /// @}

      /// @name Window and screen
      /// @{
      /**
       * @brief Returns the SDL window.
       *
       * @return Pointer to the SDL window. It is owned by Render and must
       *         not be destroyed by the caller.
       */
      SDL_Window* GetWindow(void);
      /**
       * @brief Sets the screen size.
       *
       * @param[in] w New screen width, in pixels.
       * @param[in] h New screen height, in pixels.
       */
      void SetScreenSize(float w, float h);
      /**
       * @brief Returns the screen size.
       *
       * @return A Vector2 where x is the width and y is the height, in pixels.
       */
      Vector2 GetScreenSize(void) const;
      /**
       * @brief Tells whether the window is in fullscreen mode.
       *
       * @return true if the window is in fullscreen mode, false otherwise.
       */
      bool GetFullscreen(void) const;
      /**
       * @brief Enables or disables fullscreen mode.
       *
       * @param[in] _ true to enable fullscreen, false to return to windowed mode.
       */
      void SetFullscreen(bool _);
      /**
       * @brief Switches between fullscreen and windowed mode.
       */
      void ToggleFullscreen(void);
      /// @}

      /// @name Batcher
      /// @{
      /**
       * @brief Returns the Batcher used to submit draw calls.
       *
       * @return Reference to the Batcher owned by Render.
       */
      Batcher& GetBatcher(void);
      /**
       * @brief Destroys the Batcher and releases its resources.
       */
      void DestroyBatch(void);
      /// @}

      /**
       * @brief Sets the color used by the renderer.
       *
       * @param[in] c Color to use.
       */
      void RenderColor(const Color& c);
    protected:
      /// @brief Screen width (wW) and height (wH), in pixels.
      float wW, wH;
    private:
      /// @brief Whether the window is currently in fullscreen mode.
      bool              m_fullscreen;
      /// @brief Batcher used to submit and render draw calls.
      Batcher           m_batch;
      /// @brief SDL window. nullptr until the window is created.
      SDL_Window*       m_window = nullptr;
      /// @brief OpenGL context of the window. nullptr until it is created.
      SDL_GLContext     m_glContext = nullptr;
      /// @brief Pointer to the display IDs reported by SDL.
      SDL_DisplayID*    m_displayID = 0;
      /// @brief Display mode of the display the window is shown on.
      const SDL_DisplayMode* m_display;
    private:
      /// @brief Private constructor; the instance is managed through Get() and Destroy().
      explicit Render() = default;
      /// @brief Pointer to the global Render instance. nullptr while no instance exists.
      static inline Render* m_render = nullptr;
  };
}
