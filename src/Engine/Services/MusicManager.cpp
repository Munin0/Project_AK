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
    auto p = m_music.at(key);
    MIX_PlayTrack(p.track, p.options);
  }

  void MusicManager::StopMusic(const std::string& key)
  {
    auto p = m_music.at(key);
    MIX_StopTrack(p.track, 0);
  }

  void MusicManager::LoadMusic(const std::string& path, const std::string& key, SDL_PropertiesID _options)
  {
    auto pathComplete = Path::Get().AssetsPath / path;
    MusicTrack mT;
    mT.audio = MIX_LoadAudio(this->m_mixer, pathComplete.string().c_str(), false);
    if (!mT.audio)
      LOG_ERROR(std::string(" | << MIX_LoadAudio failed: ") + SDL_GetError());
    mT.options = _options;
    mT.track = MIX_CreateTrack(m_mixer);
    MIX_SetTrackAudio(mT.track, mT.audio);

    m_music[key] = mT;
    LOG_INFO(" | <<    ->"+pathComplete.string());
  }

  bool MusicManager::IsMusicLoaded(const std::string& key)
  {
    return m_music.contains(key);
  }

  float MusicManager::GetVolume(const std::string& key) const
  {
    auto& m = m_music.at(key);
    return m.volume * 100.0f;
  }

  void MusicManager::SetVolume(const std::string& key, float _volume)
  {
    if(_volume < 0.0f)
      _volume = 0.0f;
    if(_volume > 100.0f)
      _volume = 100.0f;

    auto& foo = m_music.at(key);
    foo.volume = _volume * 0.01;
    if(!MIX_SetTrackGain(foo.track, foo.volume))
      LOG_INFO(" | << Error " + (std::string)SDL_GetError());
  }

  void MusicManager::Clear()
  {
    for (auto& [key, mT] : m_music)
    {
      MIX_DestroyTrack(mT.track);
    }
    m_music.clear();
  }
}

