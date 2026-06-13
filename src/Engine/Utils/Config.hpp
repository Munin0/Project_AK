// | -------------------------------
#pragma once
// | -------------------------------
#include <string>
#include <cstdint>
// | -------------------------------

namespace ENG
{
  enum class FLAGS 
  {
    IS_FULLSCREEN   = 1<<0,
    IS_ENGINE_ON    = 1<<1,
    IS_GAME_RUNNING = 1<<2
  };

  typedef struct _EngineConfig
  {
    unsigned int vW = 0;
    unsigned int vH = 0;
    std::string title = "";
    std::uint8_t flags = 0;
  } EngineConfig;

}
