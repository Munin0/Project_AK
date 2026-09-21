/// | ------------------------------------ |
#include "Functions.hpp"
/// | ------------------------------------ |
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
/// | ------------------------------------ |
#include <any>
/// | ------------------------------------ |


namespace APP
{
  void ChangeSceneButton(std::any _)
  {
    if(auto* idScene = std::any_cast<ENG::SceneID>(&_))
    {
      ENG::Services::Scenes().PeddingScene(*idScene);
    }
  }
}
