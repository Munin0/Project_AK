// | -------------------------------
#pragma once
// | -------------------------------
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_stdinc.h"
// | -------------------------------
#include <unordered_map>
#include <unordered_set>
// | -------------------------------

namespace ENG
{
  using SDL_MouseButton = int;

  class PollEvent
  {
    public:
      static PollEvent& Get(void);
      void DestroyPollEvent(void);
      
      void ProcessPollEvents(void);
      void ResetPollEvent(void);
      void ClearPollEvent(void);

      bool IsKeyPress(SDL_Scancode key);
      bool IsKeyDown(SDL_Scancode key);
      bool IsKeyRelease(SDL_Scancode key);

      bool IsMouseButtonPress(SDL_MouseButton button);
      bool IsMouseButtonDown(SDL_MouseButton button);
      bool IsMouseButtonRepeat(SDL_MouseButton button, int times);
      bool IsMouseButtonRelease(SDL_MouseButton button);
      
      bool IsGamepadButtonPress(Uint8 button);
      bool IsGamepadButtonDown(Uint8 button);
      bool IsGamepadAxisMove(Uint8 axis);

    public:
      static inline bool StatusApp = false;

    private:
      static inline PollEvent* _pollEvent = nullptr;

      std::unordered_set<SDL_Scancode> keyMap;
      std::unordered_set<SDL_Scancode> prevkeyMap;

      std::unordered_map<SDL_MouseButton,SDL_MouseButtonEvent> mouseMap;
      std::unordered_map<SDL_MouseButton,SDL_MouseButtonEvent> prevmouseMap;
  };
}
