/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include <any>
/// | ------------------------------------ |

namespace APP
{
  /// SignType:       std::function<void(std::any)>; 
  
  [[maybe_unused]] void ChangeSceneButton(std::any _);

  /// Button callback: closes the application (same path as closing the window).
  [[maybe_unused]] void QuitGameButton(std::any _);
}

