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
  /// @brief Mouse button identifier (for example, SDL_BUTTON_LEFT).
  using SDL_MouseButton = int;

  /**
   * @brief Input manager: polls the SDL events and answers input queries.
   *
   * PollEvent is a singleton that reads the pending SDL events once per
   * frame and keeps the state of the keyboard, the mouse and the gamepad,
   * so the rest of the engine and the game can ask about it without dealing
   * with SDL events directly.
   *
   * For keys and mouse buttons, it keeps the state of the previous frame,
   * which allows telling apart three situations:
   * - Press: the key or button went down in this frame.
   * - Down: the key or button is currently held.
   * - Release: the key or button went up in this frame.
   *
   * Usage:
   * @code
   * ENG::PollEvent& input = ENG::PollEvent::Get();
   * input.ResetPollEvent();
   * input.ProcessPollEvents();
   * if (input.IsKeyPress(SDL_SCANCODE_SPACE))
   *   Jump();
   * @endcode
   */
  class PollEvent
  {
    public:
      /**
       * @brief Returns the global PollEvent instance.
       * @return Reference to the singleton instance.
       */
      static PollEvent& Get(void);

      /**
       * @brief Destroys the global PollEvent instance.
       *
       * Must be called once, at engine shutdown. Any reference previously
       * obtained through Get() becomes invalid after this call.
       */
      void DestroyPollEvent(void);

      /**
       * @brief Reads the pending SDL events and updates the input state.
       *
       * Must be called once per frame.
       */
      void ProcessPollEvents(void);

      /**
       * @brief Prepares the input state for a new frame.
       *
       * Must be called once per frame, before ProcessPollEvents().
       */
      void ResetPollEvent(void);

      /**
       * @brief Clears the stored input state.
       */
      void ClearPollEvent(void);

      /// @name Keyboard
      /// @{

      /**
       * @brief Tells whether a key went down in this frame.
       *
       * @param[in] key Scancode of the key.
       * @return true only in the frame in which the key is pressed.
       */
      bool IsKeyPress(SDL_Scancode key);

      /**
       * @brief Tells whether a key is being held down.
       *
       * @param[in] key Scancode of the key.
       * @return true while the key is held down.
       */
      bool IsKeyDown(SDL_Scancode key);

      /**
       * @brief Tells whether a key went up in this frame.
       *
       * @param[in] key Scancode of the key.
       * @return true only in the frame in which the key is released.
       */
      bool IsKeyRelease(SDL_Scancode key);
      /// @}

      /// @name Mouse
      /// @{

      /**
       * @brief Tells whether a mouse button went down in this frame.
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true only in the frame in which the button is pressed.
       */
      bool IsMouseButtonPress(SDL_MouseButton button);

      /**
       * @brief Tells whether a mouse button is being held down.
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true while the button is held down.
       */
      bool IsMouseButtonDown(SDL_MouseButton button);

      /**
       * @brief Tells whether a mouse button was pressed a number of times in a row.
       *
       * For example, with @p times equal to 2 it detects a double click.
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @param[in] times  Number of consecutive clicks to detect.
       * @return true if the button was clicked that many times in a row.
       */
      bool IsMouseButtonRepeat(SDL_MouseButton button, int times);

      /**
       * @brief Tells whether a mouse button went up in this frame.
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true only in the frame in which the button is released.
       */
      bool IsMouseButtonRelease(SDL_MouseButton button);

      /**
       * @brief Returns the vertical movement of the mouse wheel.
       *
       * @return The amount the wheel moved vertically. Following the SDL
       *         convention, positive values scroll away from the user.
       */
      float GetMouseWheel(void) const { return wheelY; }
      /// @}

      /// @name Gamepad
      /// @{

      /**
       * @brief Tells whether a gamepad button went down in this frame.
       *
       * @param[in] button Gamepad button.
       * @return true in the frame in which the button is pressed.
       */
      bool IsGamepadButtonPress(Uint8 button);

      /**
       * @brief Tells whether a gamepad button is being held down.
       *
       * @param[in] button Gamepad button.
       * @return true while the button is held down.
       */
      bool IsGamepadButtonDown(Uint8 button);

      /**
       * @brief Tells whether a gamepad axis is being moved.
       *
       * @param[in] axis Gamepad axis.
       * @return true if the axis is moved.
       */
      bool IsGamepadAxisMove(Uint8 axis);
      /// @}

    public:
      /// @brief Application status flag, updated while the events are
      ///        processed (for example, when the user closes the window).
      static inline bool StatusApp = false;

    private:
      /// @brief Pointer to the global PollEvent instance. nullptr while no instance exists.
      static inline PollEvent* _pollEvent = nullptr;

      /// @brief Keys held down in the current frame.
      std::unordered_set<SDL_Scancode> keyMap;

      /// @brief Keys held down in the previous frame.
      std::unordered_set<SDL_Scancode> prevkeyMap;

      /// @brief Mouse buttons held down in the current frame, with their last event.
      std::unordered_map<SDL_MouseButton,SDL_MouseButtonEvent> mouseMap;

      /// @brief Mouse buttons held down in the previous frame, with their last event.
      std::unordered_map<SDL_MouseButton,SDL_MouseButtonEvent> prevmouseMap;

      /// @brief Vertical movement of the mouse wheel.
      float wheelY = 0.0f;
  };
}
