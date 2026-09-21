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
    auto it = m_music.find(key);
    if (it == m_music.end())
    {
      LOG_ERROR( " | << Can't found music >PlayMusic(): " + key);
      return;
    }
    auto p = it->second;
    MIX_PlayTrack(p.track, p.options);
  }

  void MusicManager::StopMusic(const std::string& key)
  {
    auto it = m_music.find(key);
    if (it == m_music.end())
    {
      LOG_ERROR( " | << Can't find music >StopMusic(): " + key);
      return;
    }
    auto p = it->second; 
    MIX_StopTrack(p.track, 0);
  }

  void MusicManager::LoadMusic(const std::string& path, const std::string& key, SDL_PropertiesID _options)
  {
    if (m_music.contains(key))
    {
      // LOG_ERROR( " | << Can't load music, already loaded >LoadMusic(): " + key);
      return;
    }

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
    auto it = m_music.find(key);
    if (it == m_music.end())
    {
      LOG_ERROR( " | << Can't found music >GetVolume(): " + key);
      return -1.0f;
    }
    auto& m = it->second; 
    return m.volume * 100.0f;
  }

  void MusicManager::SetVolume(const std::string& key, float _volume)
  {
    if(_volume < 0.0f)
      _volume = 0.0f;
    if(_volume > 100.0f)
      _volume = 100.0f;
    auto it = m_music.find(key);
    if(it == m_music.end())
      return;

    auto& foo = it->second;
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
    MIX_DestroyMixer(m_mixer);
    m_music.clear();
  }
}

