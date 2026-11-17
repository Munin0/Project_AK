// | -------------------------------
#include "DialogRunner.hpp"
// | -------------------------------
#include "Engine/GUI/Label.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Services/CSVManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
// | -------------------------------
#include <string>
#include <utility>
#include <vector>
// | -------------------------------

namespace APP
{
  bool DialogRunner::Start(const std::string& csvKey, const std::string& language,
                           const std::string& from, const std::string& to)
  {
    std::vector<std::string> lines = ENG::Services::CSV().GetRangeText(language, from, to, csvKey);
    if (lines.empty())
    {
      LOG_ERROR(" | << DialogRunner: no lines for '" + csvKey + "' " + from + " -> " + to);
      return false;
    }
    return Start(std::move(lines));
  }

  bool DialogRunner::Start(std::vector<std::string> lines)
  {
    if (lines.empty())
      return false;
    ENG::Label* box = GetBox();
    if (!box)
    {
      LOG_ERROR(" | << DialogRunner: no Label set (SetBox) or it was removed");
      return false;
    }

    m_lines  = std::move(lines);
    m_index  = 0;
    m_active = true;
    box->SetVisible(true);
    ShowCurrent();
    return true;
  }

  void DialogRunner::Advance(void)
  {
    if (!m_active)
      return;
    ENG::Label* box = GetBox();
    if (!box)
    {
      Stop();   // the Label was removed from the pool
      return;
    }

    // 1) The line is still appearing: show it whole.
    if (!box->IsRevealDone())
    {
      box->FinishReveal();
      return;
    }

    // 2) Next line, or the end.
    if (++m_index < m_lines.size())
      ShowCurrent();
    else
      Stop();
  }

  void DialogRunner::Stop(void)
  {
    if (ENG::Label* box = GetBox())
      box->SetVisible(false);
    m_active = false;
    m_lines.clear();
    m_index = 0;
  }

  void DialogRunner::ShowCurrent(void)
  {
    if (ENG::Label* box = GetBox())
      box->StartReveal(m_lines[m_index], m_speed);
  }

  void DialogRunner::SetBox(ENG::ObjectPool& pool, ENG::ObjectID id)
  {
    m_pool  = &pool;
    m_boxId = id;
    if (ENG::Label* box = GetBox())
      box->SetVisible(m_active);
    else
      LOG_ERROR(" | << DialogRunner: object is not a Label in the pool");
  }

  void DialogRunner::ClearBox(void)
  {
    m_pool  = nullptr;
    m_boxId = ENG::NULL_ID;
  }

  ENG::Label* DialogRunner::GetBox(void) const
  {
    if (!m_pool)
      return nullptr;
    return dynamic_cast<ENG::Label*>(m_pool->Get(m_boxId));
  }
}
