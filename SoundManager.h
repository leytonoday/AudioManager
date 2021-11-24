#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include "AudioManager.h"

namespace AudioManager
{
	class SoundManager : public AudioManagerBase<SoundData>
	{
	private:
		sf::Sound* MakeSound(SoundData* soundData)
		{
			sf::Sound* sound = new sf::Sound(); // Pointer, as it must be on the heap to stay in scope and active
			soundData->soundBuffer.loadFromFile(soundData->path);
			sound->setBuffer(soundData->soundBuffer);
			sound->setVolume(soundData->volume);
			sound->setPitch(soundData->pitch);
			sound->setLoop(soundData->looping);
			return sound;
		}

	public:
		audioID_t Load(const std::string& path, bool looping = false, float volume = 100, float pitch = 1)
		{
			LoadErrorDetection(path); // detect all errors: path exist? file supported? max of 255 audios exceeded?

			SoundData* soundData = new SoundData({ path, looping, volume, pitch, GenerateID() });
			soundData->audio = MakeSound(soundData);
			soundData->duration = soundData->audio->getBuffer()->getDuration().asMicroseconds();
			audios[soundData->ID] = soundData;
			return soundData->ID;
		}
	};
}

#endif
