// | -------------------------------
#include "Engine/Utils/Path.hpp"
#include "Game/Game.hpp"
// | -------------------------------
#include "Engine/Engine.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Config.hpp"
// | -------------------------------
#include "nlohmann/json_fwd.hpp"
#include <nlohmann/json.hpp>
// | -------------------------------
#include <ios>
#include <fstream>
#include <memory>
#include <string>
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
  // Configurations object
  ENG::EngineConfig config;
  // Loafing path to open the configurations json.
  nlohmann::json fJson;
  auto& path = ENG::Path::Get();
  std::ifstream f(path.ConfigPath / "eng.json");
  if(!f.is_open())
  {
    LOG_ERROR(" | << CONFIGURATION for the engine doesnt exist [IN]: eng.json");
    config.m_eFullscreen = false;
    config.m_title = "Project_AK";
    config.m_viewHeight = 1080;
    config.m_viewWidth = 1920;
  }
  else
  {
    f >> fJson;
    // Getting all information from the json
    config.Load(fJson);
    // Closing the file.
    f.close();
  }
  // Pointer to the Engine object.
  auto engine = std::make_unique<ENG::Engine>(config, std::make_unique<APP::Game>());
  // First configurations of the engine.
  engine->OnInit();
  // Engine running.
  engine->OnRun();
  // Engine saving all configurations.
  engine->OnSave();
  // Save configurations of the engine to json.
  std::ofstream fOut(path.ConfigPath / "eng.json", std::ios::out | std::ios::trunc);
  if(!fOut.is_open())
    LOG_FATAL(" | << CONFIGURATION for the engine doesnt exist [OUT]: eng.json");
  fJson = config.Save();  
  fOut << fJson;
  fOut.close();
  // Creaning the engine resources.
  engine->OnDestroy();
  // Reset the engine to destroy everything.
  engine.reset();
  // Out
  LOG_INFO(" | Engine Out, cya.");
  return 0;
}
