// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/GUI/Label.hpp"
#include "Engine/Object/ObjectPool.hpp"
// | -------------------------------
#include <cstddef>
#include <string>
#include <vector>
// | -------------------------------

namespace APP
{
  /**
   * @brief Runs a conversation: a sequence of lines shown in a Label.
   *
   * - The texts come from Services::CSV() (rows "from".."to" of a column / language).
   * - Each line is revealed letter by letter (Label::StartReveal()).
   * - Advance(): if the line is still being revealed, shows it whole; otherwise
   *   goes to the next line, and ends the dialog after the last one.
   *
   * The Label lives in the scene pool (Screen space, UI layer). The runner keeps
   * its ObjectID, not a pointer, so it never dangles: if the Label is removed,
   * the dialog just stops. The box is shown on Start() and hidden on Stop().
   * It is not a service: the conversation belongs to the scene and ends with it.
   *
   * @code
   * m_dialog.SetBox(m_pool, boxId);        // a Label in the pool
   * m_dialog.Start("Dialogs", "esp", "DIALOG_1", "DIALOG_4");
   * // Inputs: if (E) m_dialog.Advance();
   * // Update / Render: the pool does it (ENG::UpdateObjects / ENG::RenderObjects)
   * @endcode
   */
  class DialogRunner
  {
    public:
      /**
       * @brief Label (in @p pool) where the lines are shown. It is hidden until Start().
       *
       * @param[in] pool Pool that owns the Label (the scene's m_pool).
       * @param[in] id   ObjectID of the Label.
       */
      void SetBox(ENG::ObjectPool& pool, ENG::ObjectID id);

      /// @brief Forgets the box (call it before the pool is cleared).
      void ClearBox(void);

      /// @brief Reveal speed, in characters per second (<= 0: instant).
      void SetSpeed(float charsPerSecond) { m_speed = charsPerSecond; }

      /**
       * @brief Starts a dialog with the rows @p from .. @p to of a loaded CSV.
       *
       * @param[in] csvKey   Key the CSV was loaded with (Services::CSV().Load).
       * @param[in] language Column of the CSV ("esp", "eng"...).
       * @param[in] from     First row ID (inclusive).
       * @param[in] to       Last row ID (inclusive).
       * @return false if there are no lines or no box; the dialog is not started.
       */
      bool Start(const std::string& csvKey, const std::string& language,
                 const std::string& from, const std::string& to);

      /**
       * @brief Starts a dialog with lines given directly.
       *
       * @param[in] lines UTF-8 lines, in order.
       * @return false if @p lines is empty or there is no box.
       */
      bool Start(std::vector<std::string> lines);

      /// @brief Shows the whole line, or goes to the next one, or ends the dialog.
      void Advance(void);

      /// @brief Ends the dialog now.
      void Stop(void);

      /// @brief Whether a dialog is running.
      bool IsActive(void) const { return m_active; }

    private:
      /// @brief Shows m_lines[m_index] in the box.
      void ShowCurrent(void);

      /// @brief The Label, or nullptr if it is not set or no longer in the pool.
      ENG::Label* GetBox(void) const;

      ENG::ObjectPool*         m_pool   = nullptr;   ///< Not owned.
      ENG::ObjectID            m_boxId  = ENG::NULL_ID;
      std::vector<std::string> m_lines;
      size_t                   m_index  = 0;
      float                    m_speed  = 30.0f;
      bool                     m_active = false;
  };
}
