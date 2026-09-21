// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Services/KeyHash.hpp"
#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_properties.h"
#include "SDL3_mixer/SDL_mixer.h"
// | -------------------------------
#include <cstddef>
#include <functional>
#include <stdlib.h>
#include <string>
#include <unordered_map>
// | -------------------------------

namespace ENG
{
  /**
   * @brief A loaded music track and its playback settings.
   *
   * Groups the SDL_mixer objects needed to play a piece of music, plus its
   * volume and play options.
   */
  struct MusicTrack
  {
    /// @brief Audio data loaded from the music file.
    MIX_Audio* audio;
    /// @brief SDL_mixer track used to play the audio.
    MIX_Track* track;
    /// @brief Volume of the track. 1.0 is the original volume.
    float volume = 1.0f;
    /// @brief SDL properties with the options used to play the track
    ///        (for example, looping). 0 means default options.
    SDL_PropertiesID options = 0;
  };

  /**
   * @brief Loads, plays and controls the music of the game.
   *
   * MusicManager creates an SDL_mixer mixer on the default playback device
   * and keeps every loaded music track under a string key, so it can be
   * played, stopped and adjusted by name.
   *
   * Usage:
   * @code
   * music.Init();
   * music.LoadMusic("theme.ogg", "theme");
   * music.PlayMusic("theme");
   * @endcode
   */
  class MusicManager
  {
    public:
      /**
       * @brief Plays a loaded music track.
       *
       * @param[in] key Key the music was loaded with.
       */
      void PlayMusic(const std::string& key);
      /**
       * @brief Stops a music track that is playing.
       *
       * @param[in] key Key the music was loaded with.
       */
      void StopMusic(const std::string& key);
      /**
       * @brief Loads a music file and stores it under a key.
       *
       * @param[in] path    Path of the music file to load.
       * @param[in] key     Key used to refer to this music in the other
       *                    functions of the manager.
       * @param[in] options SDL properties with the options used to play the
       *                    track (for example, looping). 0 means default
       *                    options.
       */
      void LoadMusic(const std::string& path, const std::string& key, SDL_PropertiesID options = 0);
      /**
       * @brief Sets the volume of a music track.
       *
       * @param[in] key    Key the music was loaded with.
       * @param[in] volume New volume. 1.0 is the original volume.
       */
      void SetVolume(const std::string& key, float volume);
      /**
       * @brief Returns the volume of a music track.
       *
       * @param[in] key Key the music was loaded with.
       * @return The current volume of the track. 1.0 is the original volume.
       */
      float GetVolume(const std::string& key) const;
      /**
       * @brief Tells whether a music track is loaded.
       *
       * @param[in] key Key to look for.
       * @return true if a music track was loaded with that key, false otherwise.
       */
      bool IsMusicLoaded(const std::string& key);
      /**
       * @brief Releases the loaded music.
       *
       * Any key loaded before this call must be loaded again with
       * LoadMusic() before it can be played.
       */
      void Clear();
      /**
       * @brief Creates the mixer on the default playback device.
       *
       * Must be called before loading or playing any music. The SDL audio
       * subsystem and SDL_mixer must be initialized beforehand.
       *
       * @warning The result is not checked: if the mixer cannot be created,
       *          the internal mixer stays as nullptr and no error is reported.
       */
      void Init(void)
      {
        m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
      }
    private:
      /// @brief SDL_mixer mixer used to play the music. nullptr until Init() is called.
      MIX_Mixer* m_mixer = nullptr;
      /// @brief Loaded music tracks, indexed by key.
      std::unordered_map<std::string, MusicTrack, StringHash, std::equal_to<>> m_music;
  };
}
