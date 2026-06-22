// | -------------------------------
#include "Engine/Utils/Path.hpp"
#include "Game/Game.hpp"
// | -------------------------------
#include "Engine/Engine.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Config.hpp"
#include "nlohmann/json_fwd.hpp"
// | -------------------------------
#include <nlohmann/json.hpp>
// | -------------------------------
#include <fstream>
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

  auto& path = ENG::Path::Get();

  std::ifstream f(path.ConfigPath / "eng.json");
  LOG_INFO(" | << PATH to JSON: " + path.ConfigPath.string() + "eng.json");
  auto fJson = nlohmann::json::parse(f);
  _config.title = fJson.at("name");
  _config.vW = fJson["screen"]["w"];
  _config.vH = fJson["screen"]["h"];
  _config.flags = fJson["flags"];

  auto engine = std::make_unique<ENG::Engine>(_config, std::make_unique<APP::Game>());
  
  engine->OnInit();

  engine->OnRun();

  engine->OnDestroy();

  engine.reset();

  LOG_INFO(" | Engine Out, cya.");
  return 0;
}
