#include "PollEvent.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_stdinc.h"
#include <cassert>

namespace ENG
{
  /// | --------------------- PollEvent static class 
  PollEvent& PollEvent::Get()
  {
    if(!_pollEvent)
        _pollEvent = new PollEvent();
    return *_pollEvent;
  }

  void PollEvent::DestroyPollEvent(void)
  {
    delete _pollEvent;
  }

  /// | --------------------- Keyboard Part
  bool PollEvent::IsKeyPress(SDL_Scancode key)
  {
    return keyMap.contains(key) && !prevkeyMap.contains(key);
  }

  bool PollEvent::IsKeyDown(SDL_Scancode key)
  {
    return keyMap.contains(key) && prevkeyMap.contains(key);
  }

  bool PollEvent::IsKeyRelease(SDL_Scancode key)
  {
    return !keyMap.contains(key) && prevkeyMap.contains(key);
  }


  /// | --------------------- Mouse Part
  bool PollEvent::IsMouseButtonPress(SDL_MouseButton button)
  {
    return mouseMap.contains(button) && !prevmouseMap.contains(button);
  }

  bool PollEvent::IsMouseButtonDown(SDL_MouseButton button)
  {
    return mouseMap.contains(button);
  }

  bool PollEvent::IsMouseButtonRepeat(SDL_MouseButton button, int times)
  {
    if (!mouseMap.contains(button))
      return false;
    return mouseMap.at(button).clicks == times;  
  }

  bool PollEvent::IsMouseButtonRelease(SDL_MouseButton button)
  {
    return !mouseMap.contains(button) && prevmouseMap.contains(button);
  }


  /// | --------------------- GamePad Part
  /// Need implementation right now is empty and return false.
  bool PollEvent::IsGamepadButtonPress(Uint8 button)
  {
    (void)button;
    return false;
  }

  bool PollEvent::IsGamepadButtonDown(Uint8 button)
  {
    (void)button;
    return false;
  }

  bool PollEvent::IsGamepadAxisMove(Uint8 axis)
  {
    (void)axis;
    return false;
  }

  /// | --------------------- Process PollEvent Part
  void PollEvent::ClearPollEvent(void)
  {
    keyMap.clear();
    prevkeyMap.clear();
    mouseMap.clear();
    prevmouseMap.clear();
  }

  void PollEvent::ResetPollEvent(void)
  {
    prevkeyMap = keyMap;
    prevmouseMap = mouseMap;
  }

  void PollEvent::ProcessPollEvents() 
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_EVENT_QUIT:
          StatusApp = true;
          break;
        case SDL_EVENT_KEY_DOWN:
          keyMap.insert(event.key.scancode);
          break;
        case SDL_EVENT_KEY_UP:
          keyMap.erase(event.key.scancode);
          break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          mouseMap[event.button.button] = event.button;
          break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
          mouseMap.erase(event.button.button);
          break;
      }
    }
  }

}


