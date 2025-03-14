#ifndef __AUDIOMANAGER_H
#define __AUDIOMANAGER_H
#include "AssetManager.h"

namespace SDLFramework {

	class AudioManager {
	private:
		static AudioManager * sInstance;
		AssetManager * mAssetManager;

	public:
		void SetVolume(float volume);
		
		static AudioManager * Instance();
		static void Release();

		void PlayMusic(std::string filename, int loops = -1);
		void PlayMusic(Mix_Music * music, int loops = -1);
		void PauseMusic();
		void ResumeMusic();
		void SetMusicVolume(int volume);

		void PlaySFX(std::string filename, int loops = 0, int channel = -1);
		void PlaySFX(Mix_Chunk * sfx, int loops = 0, int channel = -1);

		void StopSFX(const std::string& soundFileName) {

			Mix_HaltChannel(-1);
		}

	private:
		AudioManager();
		~AudioManager();
	};
}
#endif