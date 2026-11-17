// | -------------------------------
#include "PollEvent.hpp"
// | -------------------------------
#include "Engine/Render/Render.hpp"
// | -------------------------------
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_stdinc.h"
// | -------------------------------
#include <cassert>
#include <string>
#include <vector>
// | -------------------------------

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
    return !IsTextInputActive() && m_keyMap.contains(key) && !m_prevkeyMap.contains(key);
  }

  bool PollEvent::IsKeyDown(SDL_Scancode key)
  {
    return !IsTextInputActive() && m_keyMap.contains(key) && m_prevkeyMap.contains(key);
  }

  bool PollEvent::IsKeyRelease(SDL_Scancode key)
  {
    return !IsTextInputActive() && !m_keyMap.contains(key) && m_prevkeyMap.contains(key);
  }

  /// | --------------------- Text input Part
  void PollEvent::StartTextInput(const void* owner)
  {
    if (!owner)
      return;
    if (!m_textOwner)
      SDL_StartTextInput(Render::Get().GetWindow());
    m_textOwner = owner;
  }

  void PollEvent::StopTextInput(const void* owner)
  {
    // another box took it: leave it on
    if (!owner || m_textOwner != owner)
      return;   
    SDL_StopTextInput(Render::Get().GetWindow());
    m_textOwner = nullptr;
    m_textInput.clear();
  }

  /// | --------------------- Mouse Part
  bool PollEvent::IsMouseButtonPress(SDL_MouseButton button)
  {
    return m_mouseMap.contains(button) && 
          !m_prevmouseMap.contains(button) &&
          !m_consumedPress.contains(button);
  }

  bool PollEvent::IsMouseButtonDown(SDL_MouseButton button)
  {
    return m_mouseMap.contains(button);
  }

  bool PollEvent::IsMouseButtonRepeat(SDL_MouseButton button, int times)
  {
    if (!m_mouseMap.contains(button) || m_claimedClick.contains(button))
      return false;
    return m_mouseMap.at(button).clicks == times;  
  }

  bool PollEvent::IsMouseButtonRelease(SDL_MouseButton button)
  {
    return  !m_mouseMap.contains(button) &&
            m_prevmouseMap.contains(button) &&
            !m_claimedClick.contains(button) &&
            !m_consumedRelease.contains(button);
  }

  bool PollEvent::ConsumeMouseButtonPress(SDL_MouseButton button)
  {
    // no new press, or someone already took it
    if (!IsMouseButtonPress(button))
      return false;
    m_consumedPress.insert(button);

    // hide the rest of this click from the others
    m_claimedClick.insert(button);
    return true;
  }

  bool PollEvent::ConsumeMouseButtonRelease(SDL_MouseButton button)
  {
    // The claim is ignored on purpose: the owner of the click takes its release here.
    const bool released = !m_mouseMap.contains(button) && m_prevmouseMap.contains(button);
    if (!released || m_consumedRelease.contains(button))
      return false;
    m_consumedRelease.insert(button);
    return true;
  }

  bool PollEvent::IsMouseButtonClaimed(SDL_MouseButton button) const
  {
    return m_claimedClick.contains(button);
  }

  bool PollEvent::IsMouseButtonPressRaw(SDL_MouseButton button) const
  {
    return m_mouseMap.contains(button) && !m_prevmouseMap.contains(button);
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
    m_keyMap.clear();
    m_prevkeyMap.clear();
    m_mouseMap.clear();
    m_prevmouseMap.clear();
    m_consumedPress.clear();
    m_consumedRelease.clear();
    m_claimedClick.clear();
    m_textInput.clear();
    m_typedKeys.clear();
  }

  void PollEvent::ResetPollEvent(void)
  {
    m_prevkeyMap = m_keyMap;
    m_prevmouseMap = m_mouseMap;

    // Consumed presses/releases only last one frame. A claimed click lasts
    // until its button is up: this frame was its release, so the claim ends.
    m_consumedPress.clear();
    m_consumedRelease.clear();
    std::erase_if(m_claimedClick, [this](SDL_MouseButton b) { return !m_mouseMap.contains(b); });

    // Typed text and keys only belong to the frame they were typed in.
    m_textInput.clear();
    m_typedKeys.clear();
  }

  void PollEvent::ProcessPollEvents()
  {
    // Wheel motion is a one-shot delta, not a held state -- only valid for the frame it fires in.
    m_wheel = 0.0f;

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_EVENT_QUIT:
          s_statusApp = true;
          break;
        case SDL_EVENT_KEY_DOWN:
          m_keyMap.insert(event.key.scancode);
          m_typedKeys.push_back({event.key.scancode, event.key.mod});   // repeats too (holding Backspace)
          break;
        case SDL_EVENT_TEXT_INPUT:
          if (m_textOwner)
            m_textInput += event.text.text;            // UTF-8, already composed by SDL
          break;
        case SDL_EVENT_KEY_UP:
          m_keyMap.erase(event.key.scancode);
          break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          m_mouseMap[event.button.button] = event.button;
          break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
          m_mouseMap.erase(event.button.button);
          break;
        case SDL_EVENT_MOUSE_WHEEL:
          m_wheel += (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) ? -event.wheel.y : event.wheel.y;
          break;
      }
    }
  }

}


