#include "AudioManager.h"

namespace SDLFramework {

    AudioManager* AudioManager::sInstance = nullptr;

    void AudioManager::SetVolume(float volume) {
        // Ensure the volume is between 0.0 and 1.0
        if (volume < 0.0f) volume = 0.0f;
        if (volume > 1.0f) volume = 1.0f;

        // Set the volume for all sound effects (Mix_Volume sets the volume for all channels)
        Mix_Volume(-1, static_cast<int>(volume * MIX_MAX_VOLUME)); // MIX_MAX_VOLUME is usually 128
    }

    AudioManager* AudioManager::Instance() {
        if (sInstance == nullptr) {
            sInstance = new AudioManager();
        }
        return sInstance;
    }

    void AudioManager::Release() {
        delete sInstance;
        sInstance = nullptr;
    }

    void AudioManager::PlayMusic(std::string filename, int loops) {
        Mix_PlayMusic(mAssetManager->GetMusic(filename), loops);
    }

    void AudioManager::PlayMusic(Mix_Music* music, int loops) {
        Mix_PlayMusic(music, loops);
    }

    void AudioManager::PauseMusic() {
        if (Mix_PlayingMusic() != 0) {
            Mix_PauseMusic();
        }
    }

    void AudioManager::ResumeMusic() {
        if (Mix_PausedMusic() != 0) {
            Mix_ResumeMusic();
        }
    }

    void AudioManager::SetMusicVolume(int volume) {
        std::cout << volume << std::endl;
        Mix_VolumeMusic(volume);
    }

    void AudioManager::PlaySFX(std::string filename, int loops, int channel) {
        Mix_Chunk* sfx = mAssetManager->GetSFX(filename);
        if (sfx == nullptr) {
            std::cerr << "Unable to load SFX " << filename << "! Mix error: " << Mix_GetError() << std::endl;
            return;
        }
        Mix_PlayChannel(channel, sfx, loops);
    }

    void AudioManager::PlaySFX(Mix_Chunk* sfx, int loops, int channel) {
        if (sfx == nullptr) {
            std::cerr << "Mix_Chunk is null! Unable to play SFX." << std::endl;
            return;
        }
        Mix_PlayChannel(channel, sfx, loops);
    }

    AudioManager::AudioManager() {
        mAssetManager = AssetManager::Instance();

        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
            std::cerr << "Unable to initialize SDL audio! SDL Error: " << SDL_GetError() << std::endl;
            return;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
            std::cerr << "Unable to initialize audio! Mix Error: " << Mix_GetError() << std::endl;
        }
    }

    AudioManager::~AudioManager() {
        mAssetManager = nullptr;
        Mix_CloseAudio();
        Mix_Quit();
    }
}
