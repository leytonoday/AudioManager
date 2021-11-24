#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "AudioData.h"

#include <SFML/Audio.hpp>

namespace AudioManager
{
	static enum AudioStates
	{
		STOPPED		= 0,
		PAUSED		= 1,
		PLAYING		= 2
	};

	template <class T>
	class AudioManagerBase
	{
	protected:
		audioID_t GenerateID()
		{
			return ++IDCounter;
		}
		void LoadErrorDetection(const std::string& path)
		{
			if (!std::filesystem::exists(path))
				throw std::exception("INVALID_PATH: path does not exist on this filesystem");

			if (!IsTypeSupported(path))
				throw std::exception("UNSUPPORTED_FILE");

			if (audios.size() >= AUDIO_LIMIT)
				throw std::exception("AUDIO_LIMIT_255_EXCEEDED");
		}

	public:
		T* ReturnAudioData(int audioID)
		{
			auto result = audios.find(audioID);
			if (result == audios.end())
				throw std::exception("INVALID_AUDIO_ID");
			return &*result->second;
		}
		bool IsTypeSupported(const std::string& path)
		{
			const int dotIndex = path.find_last_of('.');
			bool contains = std::find(supportedFileExtensions.begin(), supportedFileExtensions.end(), path.substr(dotIndex)) != supportedFileExtensions.end();
			return contains;
		}
		void Unload(audioID_t audioID) 
		{
			auto result = audios.find(audioID);
			delete result->second->audio;
			delete result->second;
			audios.erase(result);
		}

		//** sound control functions **
		void Play(audioID_t audioID)
		{
			ReturnAudioData(audioID)->audio->play();
		}
		void PlayAll()
		{
			for (auto& [key, audioData] : audios)
				audioData->audio->play();
		}
		void Pause(audioID_t audioID)
		{
			ReturnAudioData(audioID)->audio->pause();
		}
		void PauseAll()
		{
			for (const auto & [key, audioData] : audios)
				audioData.audio->pause();
		}
		void Unpause(audioID_t audioID)
		{
			ReturnAudioData(audioID)->audio->play();
		}
		void UnpauseAll()
		{
			for (const auto& [key, audioData] : audios)
				audioData.audio->play();
		}
		void Stop(audioID_t audioID)
		{
			ReturnAudioData(audioID)->audio->stop();
		}
		void StopAll()
		{
			for (const auto& [key, audioData] : audios)
				audioData.audio->stop();
		}

		//** setter functions **
		void SetPitch(audioID_t audioID, float pitch)
		{
			ReturnAudioData(audioID)->audio->setPitch(std::clamp(pitch, 0, 15));
		}
		void SetPitchAll(float pitch)
		{
			for (const auto& [key, audioData] : audios)
				audioData.audio->setPitch(std::clamp(pitch, 0, 15));
		}
		void SetVolume(audioID_t audioID, float volume)
		{
			ReturnAudioData(audioID)->audio->setVolume(std::clamp(volume, 0.0f, 100.0f));
		}
		void SetVolumeAll(float volume)
		{
			for (const auto& [key, audioData] : audios)
				audioData.audio->setVolume(std::clamp(volume, 0, 100));
		}
		void SetPlayingPosition(audioID_t audioID, int position)
		{
			sf::Time offset = sf::milliseconds(std::clamp(position, 0, GetDuration(audioID)));
			ReturnAudioData(audioID)->audio->setPlayingOffset(offset);
		}
		void SetPlayingPositionAll(int position)
		{
			for (const auto& [key, audioData] : audios)
			{
				sf::Time offset = sf::milliseconds(position);
				audioData.audio->setPlayingOffset(std::clamp(position, 0, GetDuration(key)));
			}
		}
		

		//** getter functions **
		float GetPitch(audioID_t audioID)
		{
			return ReturnAudioData(audioID)->audio->getPitch();
		}
		float GetVolume(audioID_t audioID) 
		{
			return ReturnAudioData(audioID)->audio->getVolume();
		}
		float GetPlayingPosition(audioID_t audioID) 
		{
			return ReturnAudioData(audioID)->audio->getPlayingOffset().asMilliseconds();
		}
		int GetCount()
		{
			return audios.size();
		}
		int GetDuration(audioID_t audioID)
		{
			return ReturnAudioData(audioID)->duration;
		}
		std::vector<int> GetAllIDs()
		{
			std::vector<int> IDs;
			for (const auto & [key, audioData]: audios)
				IDs.push_back(key);
			return IDs;
		}
		std::string GetPath(audioID_t audioID)
		{
			return ReturnAudioData(audioID)->path;
		}
		
		int IsPaused(audioID_t audioID) 
		{
			return ReturnAudioData(audioID)->audio->getStatus() == PAUSED;
		}
		int IsPlaying(audioID_t audioID) 
		{
			return ReturnAudioData(audioID)->audio->getStatus() == PLAYING;
		}
		bool IsManagerActive()
		{
			bool active = false;
			for (const auto & [key, audioData]: audios)
				active = IsPlaying(key);
			return active;
		}

		//** misc. functions **
		float DBToVolume(float dB)
		{
			return powf(10.0f, 0.05f * dB);
		}
		float VolumeTodB(float volume)
		{
			return 20.0f * log10f(volume);
		}

		//*** containers ***
		std::unordered_map<audioID_t, T*> audios; // A map containing the currently loaded audios (could be streams or sounds)
		std::vector<std::string> supportedFileExtensions{ ".ogg", ".wav", ".flac", ".aiff", ".au", ".raw", ".paf", ".svx", ".nist",
														".voc", ".ircam", ".w64", ".mat4", ".mat5", ".pvf", ".htk", ".sds", ".avr",
														".sd2", ".caf", ".wve", ".mpc2k", ".rf64" };

		unsigned int IDCounter = 0; //This will be incremented every GenerateID() call, and it's value will be used as a unique audioID
		const unsigned int AUDIO_LIMIT = 255;
	};
}

#endif