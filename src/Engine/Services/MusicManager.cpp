// | -------------------------------
#include "MusicManager.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_properties.h"
#include "SDL3_mixer/SDL_mixer.h"
// | -------------------------------
#include <string>
// | -------------------------------

namespace ENG
{
  void MusicManager::PlayMusic(const std::string& key)
  {
    auto p = music.at(key);
    MIX_PlayTrack(p.track, p.options);
  }

  void MusicManager::LoadMusic(const std::string& path, const std::string& key, SDL_PropertiesID _options)
  {
    auto pathComplete = Path::Get().AssetsPath / path;
    MusicTrack mT;
    mT.audio = MIX_LoadAudio(this->mixer, pathComplete.string().c_str(), false);
    if (!mT.audio)
      LOG_ERROR(std::string(" | << MIX_LoadAudio failed: ") + SDL_GetError());
    mT.options = _options;
    mT.track = MIX_CreateTrack(mixer);
    MIX_SetTrackAudio(mT.track, mT.audio);

    music[key] = mT;
    LOG_INFO(" | <<    ->"+pathComplete.string());
  }

  bool MusicManager::IsMusicLoaded(const std::string& key)
  {
    return music.contains(key);
  }

  float MusicManager::GetVolume(const std::string& key) const
  {
    auto& m = music.at(key);
    return m.volume * 100.0f;
  }

  void MusicManager::SetVolume(const std::string& key, float _volume)
  {
    if(_volume < 0.0f)
      _volume = 0.0f;
    if(_volume > 100.0f)
      _volume = 100.0f;

    auto& foo = music.at(key);
    foo.volume = _volume * 0.01;
    if(!MIX_SetTrackGain(foo.track, foo.volume))
      LOG_INFO(" | << Error " + (std::string)SDL_GetError());
  }

  void MusicManager::Clear()
  {
    for (auto& [key, mT] : music)
    {
      MIX_DestroyTrack(mT.track);
    }
    music.clear();
  }
}

