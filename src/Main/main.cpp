// | -------------------------------
#include "Game/Game.hpp"
// | -------------------------------
#include "Engine/Engine.hpp"
#include "Engine/Utils/Config.hpp"
// | -------------------------------
#include "SDL3/SDL_log.h"
// | -------------------------------
#include <memory>
// | -------------------------------
// STB Implementation 
#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Extern/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Engine/Extern/stb_image_write.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "Engine/Extern/stb_truetype.h"
// | -------------------------------

int main(void)
{
  ENG::EngineConfig _config;

  _config.title = "Project AK";
  _config.vW = 1080;
  _config.vH = 720;
  _config.flags = 0;

  auto engine = std::make_unique<ENG::Engine>(_config, std::make_unique<APP::Game>());
  
  engine->OnInit();

  engine->OnRun();

  engine->OnDestroy();

  engine.reset();

  SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, " | Engine Out, cya.");
  return 0;
}
