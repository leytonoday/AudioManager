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
	enum class AudioStates
	{
		STOPPED		= 0,
		PAUSED		= 1,
		PLAYING		= 2
	};
	
	template <class T>
	class AudioManagerBase
	{
	private:
		unsigned int IDCounter = 0;					//This will be incremented to make unique audioIDs
		const unsigned int AUDIO_LIMIT = 255;

	protected:
		~AudioManagerBase() 
		{
			UnloadAll();
		}
		audioID_t GenerateID()
		{
			return ++IDCounter;
		}
		void LoadErrorDetection(const std::string& path)
		{
			if (!std::filesystem::exists(path))
				throw std::exception("INVALID_PATH: Does not exist on this filesystem");

			if (!IsTypeSupported(path))
				throw std::exception("UNSUPPORTED_FILE: Not a supported file type");

			if (audios.size() >= AUDIO_LIMIT)
				throw std::exception("AUDIO_LIMIT_255_EXCEEDED: Try unloading unused audios");
		}
		std::unordered_map<audioID_t, T*> audios;	// A map containing the currently loaded audios (could be streams or sounds)


	public:
		//** sound control functions **
		void Play(audioID_t audioID)
		{
			GetAudioData(audioID)->audio->play();
		}
		void PlayAll()
		{
			for (auto& [audioID, audioData] : audios)
				audioData->audio->play();
		}
		void Pause(audioID_t audioID)
		{
			GetAudioData(audioID)->audio->pause();
		}
		void PauseAll()
		{
			for (const auto & [audioID, audioData] : audios)
				audioData->audio->pause();
		}
		void Unpause(audioID_t audioID)
		{
			GetAudioData(audioID)->audio->play();
		}
		void UnpauseAll()
		{
			for (const auto& [audioID, audioData] : audios)
				audioData->audio->play();
		}
		void Stop(audioID_t audioID)
		{
			GetAudioData(audioID)->audio->stop();
		}
		void StopAll()
		{
			for (const auto& [audioID, audioData] : audios)
				audioData->audio->stop();
		}
		void Unload(audioID_t audioID) 
		{
			auto result = audios.find(audioID);
			delete result->second->audio;
			delete result->second;
			audios.erase(result);
		}
		void UnloadAll() 
		{
			for (auto& [audioID, audioData] : audios)
			{
				delete audioData->audio;
				delete audioData;
			}
			audios.clear();
		}

		//** setter functions **
		void SetPitch(audioID_t audioID, float pitch)
		{
			GetAudioData(audioID)->audio->setPitch(std::clamp(pitch, 0.0f, 15.0f));
		}
		void SetPitchAll(float pitch)
		{
			for (const auto& [audioID, audioData] : audios)
				audioData->audio->setPitch(std::clamp(pitch, 0.0f, 15.0f));
		}
		void SetVolume(audioID_t audioID, float volume)
		{
			GetAudioData(audioID)->audio->setVolume(std::clamp(volume, 0.0f, 100.0f));
		}
		void SetVolumeAll(float volume)
		{
			for (const auto& [audioID, audioData] : audios)
				audioData->audio->setVolume(std::clamp(volume, 0.0f, 100.0f));
		}
		void SetPlayingPosition(audioID_t audioID, int position)
		{
			T* audioData = GetAudioData(audioID);
			if (position > audioData->duration)
				return Stop(audioID);
			audioData->audio->setPlayingOffset(sf::milliseconds(std::clamp(position, 0, GetDuration(audioID))));
		}
		void SetPlayingPositionAll(int position)
		{
			for (const auto& [audioID, audioData] : audios)
				SetPlayingPosition(audioID, position);
		}
		
		//** getter functions **
		T* GetAudioData(int audioID)
		{
			auto result = audios.find(audioID);
			if (result == audios.end())
				throw std::exception("INVALID_AUDIO_ID");
			return &*result->second;
		}
		float GetPitch(audioID_t audioID)
		{
			return GetAudioData(audioID)->audio->getPitch();
		}
		float GetVolume(audioID_t audioID) 
		{
			return GetAudioData(audioID)->audio->getVolume();
		}
		float GetPlayingPosition(audioID_t audioID) 
		{
			return GetAudioData(audioID)->audio->getPlayingOffset().asMilliseconds();
		}
		int GetAudioCount()
		{
			return audios.size();
		}
		int GetDuration(audioID_t audioID)
		{
			return GetAudioData(audioID)->duration;
		}
		int GetAudioLimit()
		{
			return AUDIO_LIMIT;
		}
		std::vector<int> GetAllIDs()
		{
			std::vector<int> IDs;
			for (const auto & [audioID, audioData]: audios)
				IDs.push_back(audioID);
			return IDs;
		}
		std::string GetPath(audioID_t audioID)
		{
			return GetAudioData(audioID)->path;
		}
		
		bool IsTypeSupported(const std::string& path)
		{
			const int dotIndex = path.find_last_of('.');
			const std::string extension = path.substr(dotIndex);
			for (std::string i : supportedFileExtensions)
				if (extension == i)
					return true;
			return false;
		}
		bool IsManagerActive()
		{
			bool active = false;
			for (const auto & [audioID, audioData]: audios)
				active = IsPlaying(audioID);
			return active;
		}
		int IsPaused(audioID_t audioID) 
		{
			return GetAudioData(audioID)->audio->getStatus() == (int)AudioStates::PAUSED;
		}
		int IsPlaying(audioID_t audioID) 
		{
			return GetAudioData(audioID)->audio->getStatus() == (int)AudioStates::PLAYING;
		}

		//** misc. functions **
		float DBToVolume(float DB)
		{
			return powf(10.0f, 0.05f * DB);
		}
		float VolumeToDB(float volume)
		{
			return 20.0f * log10f(volume);
		}

		std::string supportedFileExtensions[23] = { ".ogg", ".wav", ".flac", ".aiff", ".au", ".raw", ".paf", ".svx", 
													".nist", ".voc", ".ircam", ".w64", ".mat4", ".mat5", ".pvf", ".htk",
													".sds", ".avr",".sd2", ".caf", ".wve", ".mpc2k", ".rf64" };
	};
}

#endif