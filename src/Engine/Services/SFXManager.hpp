// | -------------------------------
#pragma once
// | -------------------------------
#include "SDL3/SDL_audio.h"
#include "SDL3_mixer/SDL_mixer.h"
// | -------------------------------
#include <cstddef>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>
// | -------------------------------
#define MAX_TRACKS_MEMORY 16
// | -------------------------------

namespace ENG
{
  struct SFXPool
  {
    MIX_Audio* audio;
    std::vector<MIX_Track*> tracksPool;
    float volume = 1.0f;
    size_t maxTracksMemory = MAX_TRACKS_MEMORY;
    size_t nextIndex = 0;
  };

  class SFXManager
  {
    public:
      void PlaySFX(const std::string& key);
      void LoadSFX(const std::string& path, const std::string& key, size_t maxTracksMemory);
      void SetVolume(const std::string& key, float volume);
      float GetVolume(const std::string& key) const;
      bool IsSFXLoaded(const std::string& key);
      void Clear();

      void Init(void)
      {
        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
      }
    private:
      MIX_Track* GetTrackFreeOrNew(SFXPool& pool);
    private:
      MIX_Mixer *mixer = nullptr;
      std::map<std::string, SFXPool> sfx;
  };
}
