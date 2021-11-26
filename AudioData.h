#ifndef AUDIO_DATA_H_
#define AUDIO_DATA_H_

#include <string>
#include <SFML/Audio.hpp>

namespace AudioManager
{
	using audioID_t = std::uint32_t; //This will be used to represent all audioIDs

	struct AudioDataBase
	{
		std::string		path;
		bool			looping;
		float			volume;
		float			pitch;
		audioID_t		ID;
		int			duration;
	};
	
	// Used for small audios and effects
	struct SoundData : AudioDataBase
	{
		sf::SoundBuffer soundBuffer;
		sf::Sound* audio = nullptr;
	};

	// Used for large audios, to stream the data rather than loading it all into memory
	struct SoundStreamData : public AudioDataBase
	{
		sf::Music* audio = nullptr;
	};
}

#endif
