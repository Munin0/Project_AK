// | -------------------------------
#pragma once
// | -------------------------------
#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_properties.h"
#include "SDL3_mixer/SDL_mixer.h"
// | -------------------------------
#include <cstddef>
#include <map>
#include <stdlib.h>
#include <string>
// | -------------------------------

namespace ENG
{
  struct MusicTrack
  {
    MIX_Audio* audio;
    MIX_Track* track;
    float volume = 1.0f;
    SDL_PropertiesID options = 0;
  };

  class MusicManager
  {
    public:
      void PlayMusic(const std::string& key);
      void LoadMusic(const std::string& path, const std::string& key, SDL_PropertiesID options = 0);
      void SetVolume(const std::string& key, float volume);
      float GetVolume(const std::string& key) const;
      bool IsMusicLoaded(const std::string& key);
      void Clear();

      void Init(void)
      {
        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
      }
    private:
      MIX_Mixer* mixer = nullptr;
      std::map<std::string, MusicTrack> music;
  };
}
