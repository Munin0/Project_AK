// | -------------------------------
#include "SFXManager.hpp"
// | -------------------------------
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Path.hpp"
// | -------------------------------
#include "SDL3/SDL_error.h"
#include "SDL3_mixer/SDL_mixer.h"
// | -------------------------------
#include <cstddef>
#include <string>
// | -------------------------------

namespace ENG
{
  void SFXManager::PlaySFX(const std::string& key)
  {
    auto p = sfx.at(key);

    auto track = GetTrackFreeOrNew(p);
    if(!track) return;

    MIX_PlayTrack(track, 0);
  }

  void SFXManager::LoadSFX(const std::string& path, const std::string& key, size_t _maxTracksMemory)
  {
    auto pathComplete = Path::Get().AssetsPath / path;
    
    auto audio = MIX_LoadAudio(this->mixer, pathComplete.string().c_str(), false);
    if (!audio)
      LOG_ERROR(std::string(" | << MIX_LoadAudio failed: ") + SDL_GetError());
    
    SFXPool p;
    p.audio = audio;
    p.maxTracksMemory = _maxTracksMemory;
    sfx[key] = p;
    LOG_INFO(" | <<    ->"+pathComplete.string());
  }

  float SFXManager::GetVolume(const std::string& key) const
  {
    return sfx.at(key).volume;
  }

  void SFXManager::SetVolume(const std::string& key, float volume)
  {
    if(volume < 0.0f)
        volume = 0.0f;
    if(volume>100.0f)
        volume = 100.0f;

    volume = volume * 0.01;
    auto foo = sfx.at(key);
    foo.volume = volume;
    for(auto& t : foo.tracksPool)
      MIX_SetTrackGain(t,foo.volume);
  }

  bool SFXManager::IsSFXLoaded(const std::string& key)
  {
    return sfx.contains(key);
  }

  void SFXManager::Clear()
  {
    for (auto& [key, pool] : sfx)
    {
      for(auto* track : pool.tracksPool)
        MIX_DestroyTrack(track);
    }
    sfx.clear();
  }

  MIX_Track* SFXManager::GetTrackFreeOrNew(SFXPool& p)
  {
    for (auto* track : p.tracksPool)
    {
      if (!MIX_TrackPlaying(track))
        return track; 
    }

    if(p.tracksPool.size() < p.maxTracksMemory)
    {
        auto track = MIX_CreateTrack(this->mixer);
        if (!track) {
            LOG_ERROR(std::string("MIX_CreateTrack failed: ") + SDL_GetError());
            return nullptr;
        }
        MIX_SetTrackAudio(track, p.audio);
        p.tracksPool.push_back(track);
        return track;
    }

    auto* stolen = p.tracksPool[p.nextIndex];
    p.nextIndex = (p.nextIndex + 1) % p.tracksPool.size();

    MIX_StopTrack(stolen, 0);
    return stolen;  
  }

}

