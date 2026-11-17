/// | ------------------------------------ |
#include "Functions.hpp"
/// | ------------------------------------ |
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
/// | ------------------------------------ |
#include <any>
#include <string>
/// | ------------------------------------ |


namespace APP
{
  void ChangeSceneButton(std::any _)
  {
    if(auto* idScene = std::any_cast<std::string>(&_))
    {
      ENG::Services::Scenes().SetPedingScene(*idScene);
    }
  }

  void QuitGameButton(std::any _)
  {
    (void)_;
    ENG::PollEvent::s_statusApp = true;
  }
  
}
