#ifndef SOUND_STREAM_MANAGER_H_
#define SOUND_STREAM_MANAGER_H_

#include "AudioManager.h"

namespace AudioManager 
{
	class SoundStreamManager : public AudioManagerBase<SoundStreamData>
	{
	private:
		sf::Music* MakeSoundStream(SoundStreamData* streamData) 
		{
			sf::Music* music = new sf::Music(); // Pointer, as it must be on the heap to stay in scope and active
			music->openFromFile(streamData->path);
			music->setVolume(streamData->volume);
			music->setPitch(streamData->pitch);
			music->setLoop(streamData->looping);
			return music;
		}
	public:
		audioID_t Load(const std::string& path, bool looping = false, float volume = 100, float pitch = 1)
		{
			LoadErrorDetection(path); // detect all errors: path exist? file supported? max of 255 audios exceeded?

			SoundStreamData* streamData = new SoundStreamData({ path, looping, volume, pitch, GenerateID() });
			streamData->audio = MakeSoundStream(streamData);
			streamData->duration = streamData->audio->getDuration().asMilliseconds();
			audioMap[streamData->ID] = streamData;
			return streamData->ID;
		}
	};
}

#endif SOUND_STREAM_MANAGER_H