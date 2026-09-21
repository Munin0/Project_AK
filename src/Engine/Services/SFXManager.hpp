// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Services/KeyHash.hpp"
#include "SDL3/SDL_audio.h"
#include "SDL3_mixer/SDL_mixer.h"
// | -------------------------------
#include <cstddef>
#include <functional>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
// | -------------------------------
#define MAX_TRACKS_MEMORY 16
// | -------------------------------

namespace ENG
{
  /**
   * @brief A sound effect and the pool of tracks used to play it.
   *
   * A sound effect can be triggered again before the previous playback has
   * finished, so each SFX has a pool of tracks that can play it at the same
   * time (overlapping playbacks).
   */
  struct SFXPool
  {
    /// @brief Audio data loaded from the sound effect file.
    MIX_Audio* audio;
    /// @brief Tracks that can play the sound effect simultaneously.
    std::vector<MIX_Track*> tracksPool;
    /// @brief Volume of the sound effect. 1.0 is the original volume.
    float volume;
    /// @brief Maximum number of tracks the pool can hold.
    ///        Defaults to MAX_TRACKS_MEMORY.
    size_t maxTracksMemory = MAX_TRACKS_MEMORY;
    /// @brief Index of the next track of the pool to use when the pool is full.
    size_t nextIndex = 0;
  };

  /**
   * @brief Loads, plays and controls the sound effects of the game.
   *
   * SFXManager creates an SDL_mixer mixer on the default playback device
   * and keeps every loaded sound effect under a string key. Each sound
   * effect owns a pool of tracks, so the same effect can be played several
   * times at once.
   *
   * Usage:
   * @code
   * sfx.Init();
   * sfx.LoadSFX("jump.wav", "jump", 4);
   * sfx.PlaySFX("jump");
   * @endcode
   */
  class SFXManager
  {
    public:
      /**
       * @brief Plays a loaded sound effect.
       *
       * Uses a track from the effect's pool, so the effect can overlap with
       * itself.
       *
       * @param[in] key Key the sound effect was loaded with.
       */
      void PlaySFX(const std::string& key);
      /**
       * @brief Loads a sound effect and stores it under a key.
       *
       * @param[in] path            Path of the sound effect file to load.
       * @param[in] key             Key used to refer to this sound effect in
       *                            the other functions of the manager.
       * @param[in] maxTracksMemory Maximum number of tracks in the pool of
       *                            this effect, that is, how many playbacks
       *                            of it can overlap.
       */
      void LoadSFX(const std::string& path, const std::string& key, size_t maxTracksMemory);
      /**
       * @brief Sets the volume of a sound effect.
       *
       * @param[in] key    Key the sound effect was loaded with.
       * @param[in] volume New volume. 1.0 is the original volume.
       */
      void SetVolume(const std::string& key, float volume);
      /**
       * @brief Returns the volume of a sound effect.
       *
       * @param[in] key Key the sound effect was loaded with.
       * @return The current volume of the sound effect. 1.0 is the original volume.
       */
      float GetVolume(const std::string& key) const;
      /**
       * @brief Tells whether a sound effect is loaded.
       *
       * @param[in] key Key to look for.
       * @return true if a sound effect was loaded with that key, false otherwise.
       */
      bool IsSFXLoaded(const std::string& key);
      /**
       * @brief Releases the loaded sound effects.
       *
       * Any key loaded before this call must be loaded again with
       * LoadSFX() before it can be played.
       */
      void Clear();
      /**
       * @brief Creates the mixer on the default playback device.
       *
       * Must be called before loading or playing any sound effect. The SDL
       * audio subsystem and SDL_mixer must be initialized beforehand.
       *
       * @warning The result is not checked: if the mixer cannot be created,
       *          the internal mixer stays as nullptr and no error is reported.
       */
      void Init(void)
      {
        m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
      }
    private:
      /**
       * @brief Gets a track of a pool to play a sound effect on.
       *
       * Returns a track of the pool that is free to use, or creates a new
       * one if the pool has not reached its maximum size. When the pool is
       * full, it reuses one of its tracks.
       *
       * @param[in,out] pool Pool of the sound effect to be played.
       * @return The track to play the sound effect on.
       */
      MIX_Track* GetTrackFreeOrNew(SFXPool& pool);
    private:
      /// @brief SDL_mixer mixer used to play the sound effects. nullptr until Init() is called.
      MIX_Mixer* m_mixer = nullptr;
      /// @brief Loaded sound effects, indexed by key.
      std::unordered_map<std::string, SFXPool, StringHash, std::equal_to<>> m_sfx;
  };
}
