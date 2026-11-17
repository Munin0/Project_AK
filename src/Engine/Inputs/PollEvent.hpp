// | -------------------------------
#pragma once
// | -------------------------------
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_stdinc.h"
// | -------------------------------
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// | -------------------------------

namespace ENG
{
  /// @brief Mouse button identifier (for example, SDL_BUTTON_LEFT).
  using SDL_MouseButton = int;

  /// @brief A key that went down, with the modifiers held at that moment.
  struct TypedKey
  {
    SDL_Scancode m_key = SDL_SCANCODE_UNKNOWN;   ///< Physical key.
    SDL_Keymod   m_mod = SDL_KMOD_NONE;          ///< Modifiers (Ctrl, Shift, Alt...) when it went down.

    /// @brief Whether Ctrl (left or right) was held.
    bool Ctrl(void) const  { return (m_mod & SDL_KMOD_CTRL) != 0; }
    /// @brief Whether Shift (left or right) was held.
    bool Shift(void) const { return (m_mod & SDL_KMOD_SHIFT) != 0; }
  };

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
   * Mouse clicks can be consumed, so a click used by one object (a UI
   * button) is not seen by the others (another button below, the world).
   * Consuming the press claims the whole click: its press, its repeat and
   * its release are hidden from the plain queries until the button goes up.
   * Down is never hidden, so the owner can keep dragging. Who asks first
   * wins, so objects on top must ask first.
   *
   * Text input: an object that wants typed text (a focused TextBox) calls
   * StartTextInput(this). While it is active, SDL composes the text with the
   * keyboard layout (accents, ñ, capitals) and GetTextInput() returns what was
   * typed this frame (UTF-8); GetTypedKeys() returns the editing keys
   * (Backspace, Enter, arrows...) including the OS key repeat. The keyboard is
   * captured: IsKeyPress/IsKeyDown/IsKeyRelease return false for everyone, so
   * typing "e" in a box does not trigger the game's "E" action.
   * @code
   * // Check that the click is yours first, then consume it.
   * if (CollisionPointRect(mouse, rect) && input.ConsumeMouseButtonPress(SDL_BUTTON_LEFT))
   *   Action();
   * // Elsewhere (the world), a consumed click is just not there:
   * if (input.IsMouseButtonPress(SDL_BUTTON_LEFT)) MoveTo(mouse);
   * @endcode
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
       *         Always false while the text input is active.
       */
      bool IsKeyPress(SDL_Scancode key);

      /**
       * @brief Tells whether a key is being held down.
       *
       * @param[in] key Scancode of the key.
       * @return true while the key is held down.
       *         Always false while the text input is active.
       */
      bool IsKeyDown(SDL_Scancode key);

      /**
       * @brief Tells whether a key went up in this frame.
       *
       * @param[in] key Scancode of the key.
       * @return true only in the frame in which the key is released.
       *         Always false while the text input is active.
       */
      bool IsKeyRelease(SDL_Scancode key);
      /// @}

      /// @name Text input
      /// @{

      /**
       * @brief Starts receiving typed text, on behalf of @p owner.
       *
       * Calls SDL_StartTextInput() on the window and captures the keyboard
       * (see the class description). Starting again with another owner just
       * changes the owner (one focused box at a time).
       *
       * @param[in] owner Who asks (usually `this`); only it can stop it.
       */
      void StartTextInput(const void* owner);

      /**
       * @brief Stops receiving typed text, if @p owner is the current owner.
       *
       * Ignored when another owner took the text input in the meantime, so a
       * box losing the focus does not cut the text of the box that took it.
       *
       * @param[in] owner The same pointer given to StartTextInput().
       */
      void StopTextInput(const void* owner);

      /// @brief Whether the text input is active (and the keyboard captured).
      bool IsTextInputActive(void) const { return m_textOwner != nullptr; }

      /// @brief Whether @p owner currently owns the text input.
      bool IsTextInputOwner(const void* owner) const { return owner && m_textOwner == owner; }

      /// @brief Text typed in this frame, UTF-8 (already composed by SDL). Empty if none.
      const std::string& GetTextInput(void) const { return m_textInput; }

      /// @brief Keys that went down in this frame, in order, with their modifiers
      ///        (Ctrl+A...) and including the OS repeats (holding Backspace deletes
      ///        continuously). Not affected by the capture.
      const std::vector<TypedKey>& GetTypedKeys(void) const { return m_typedKeys; }
      /// @}

      /// @name Mouse
      /// @{

      /**
       * @brief Tells whether a mouse button went down in this frame.
       *
       * It only looks, it does not consume (see ConsumeMouseButtonPress()).
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true only in the frame in which the button is pressed, and
       *         only if nobody consumed that press.
       */
      bool IsMouseButtonPress(SDL_MouseButton button);

      /**
       * @brief Tells whether a mouse button is being held down.
       *
       * Not affected by consuming: use IsMouseButtonClaimed() to know whether
       * the held click belongs to someone else.
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
       * @return true if the button was clicked that many times in a row, and
       *         the click is not claimed (see ConsumeMouseButtonPress()).
       */
      bool IsMouseButtonRepeat(SDL_MouseButton button, int times);

      /**
       * @brief Tells whether a mouse button went up in this frame.
       *
       * It only looks, it does not consume (see ConsumeMouseButtonRelease()).
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true only in the frame in which the button is released, and
       *         only if the click is not claimed and the release was not
       *         consumed.
       */
      bool IsMouseButtonRelease(SDL_MouseButton button);

      /**
       * @brief Takes the press of a mouse button: uses it and claims the click.
       *
       * If the button went down in this frame and nobody took it yet, the
       * press is consumed and the whole click is claimed until the button
       * goes up: from now on IsMouseButtonPress(), IsMouseButtonRepeat() and
       * IsMouseButtonRelease() return false for it. IsMouseButtonDown() is
       * not affected. The claim ends after the frame in which it is released.
       *
       * Call it only once you know the click is yours (e.g. after checking
       * the mouse is over the object), or you will eat clicks meant for others.
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true if the press was available and is now yours.
       */
      bool ConsumeMouseButtonPress(SDL_MouseButton button);

      /**
       * @brief Takes the release of a mouse button (for actions on release).
       *
       * Ignores the claim, so the owner of a click consumed with
       * ConsumeMouseButtonPress() can still get its release. After this,
       * IsMouseButtonRelease() returns false for the rest of the frame.
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true if the button went up in this frame and nobody took
       *         the release yet.
       */
      bool ConsumeMouseButtonRelease(SDL_MouseButton button);

      /**
       * @brief Tells whether the current click of a button was claimed.
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true from ConsumeMouseButtonPress() until the end of the
       *         frame in which the button goes up.
       */
      bool IsMouseButtonClaimed(SDL_MouseButton button) const;

      /**
       * @brief Tells whether a mouse button went down in this frame, even if
       *        someone consumed it.
       *
       * For objects that only observe a click and never take it, e.g. a
       * TextBox that loses the focus when the user clicks anywhere else.
       *
       * @param[in] button Mouse button (for example, SDL_BUTTON_LEFT).
       * @return true only in the frame in which the button is pressed.
       */
      bool IsMouseButtonPressRaw(SDL_MouseButton button) const;

      /**
       * @brief Returns the vertical movement of the mouse wheel.
       *
       * @return The amount the wheel moved vertically. Following the SDL
       *         convention, positive values scroll away from the user.
       */
      float GetMouseWheel(void) const { return m_wheel; }
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
      static inline bool s_statusApp = false;

    private:
      /// @brief Pointer to the global PollEvent instance. nullptr while no instance exists.
      static inline PollEvent* _pollEvent = nullptr;

      /// @brief Keys held down in the current frame.
      std::unordered_set<SDL_Scancode> m_keyMap;

      /// @brief Keys held down in the previous frame.
      std::unordered_set<SDL_Scancode> m_prevkeyMap;

      /// @brief Mouse buttons held down in the current frame, with their last event.
      std::unordered_map<SDL_MouseButton,SDL_MouseButtonEvent> m_mouseMap;

      /// @brief Mouse buttons held down in the previous frame, with their last event.
      std::unordered_map<SDL_MouseButton,SDL_MouseButtonEvent> m_prevmouseMap;

      /// @brief Text typed in this frame (SDL_EVENT_TEXT_INPUT). Cleared every frame.
      std::string m_textInput;

      /// @brief Keys pressed in this frame, with repeats. Cleared every frame.
      std::vector<TypedKey> m_typedKeys;

      /// @brief Owner of the text input, nullptr if it is off.
      const void* m_textOwner = nullptr;

      /// @brief Buttons whose press was consumed in this frame. Cleared every frame.
      std::unordered_set<SDL_MouseButton> m_consumedPress;

      /// @brief Buttons whose release was consumed in this frame. Cleared every frame.
      std::unordered_set<SDL_MouseButton> m_consumedRelease;

      /// @brief Buttons whose whole click is claimed (press consumed, not yet
      ///        released). Each one is removed after the frame it goes up.
      std::unordered_set<SDL_MouseButton> m_claimedClick;

      /// @brief Vertical movement of the mouse wheel.
      float m_wheel = 0.0f;
  };
}
