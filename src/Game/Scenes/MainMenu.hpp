/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Layer/Scene.hpp"
#include "Engine/Render/Batching/Batch.hpp"
/// | ------------------------------------ |
#include <string>
/// | ------------------------------------ |

namespace APP
{
  /**
   * @brief Main menu: a title (Label), a name field (TextBox) and two buttons.
   *
   * - "Jugar": goes to the "GameScene".
   * - "Salir": closes the application.
   *
   * It is drawn in screen space (no camera).
   */
  class MainMenu : public ENG::Scene
  {
    public:
      MainMenu(const std::string& id);

      void Init(void)               override;
      void Destroy(void)            override;
      void Inputs(float dt)         override;
      void Update(float dt)         override;
      void UpdateFixed(float dt)    override;
      void Render(ENG::Batcher& b)  override;

    private:
  };
}
