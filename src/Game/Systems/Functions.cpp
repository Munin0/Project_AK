/// | ------------------------------------ |
#include "Functions.hpp"
/// | ------------------------------------ |
#include "Engine/Services/ScenesManager.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
/// | ------------------------------------ |
#include <any>
#include <string>
/// | ------------------------------------ |


namespace APP
{
  void ChangeSceneButton(std::any _)
  {
    LOG_INFO("Tipo real en el any: " + std::string(_.type().name()));
    if(auto* idScene = std::any_cast<ENG::SceneID>(&_))
    {
      LOG_INFO(" || << IDSCENE: " + std::to_string(*idScene));
      ENG::Services::Scenes().PeddingScene(*idScene);
    }
  }
}
