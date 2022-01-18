# Audio Manager

The Audio Manager is used to elegantly implement audio into your C++ console or GUI applications. 

Implementing audio into an application can be quite daunting, with APIs such as OpenAL and XAudio2. This header only solution alleviates the burden of operating these low-level APIs, and presents the user with a simple pair of high level classes: ```SoundStreamManager``` for long audio (music) and ```SoundManager``` for short audio (sfx). 


## Project Setup

It is a helpful wrapper around the audio module of SFML, and as a result, you will need to download and link your application to the SFML dlls. 
Visit this webpage for further instructions: https://www.sfml-dev.org/tutorials/2.5/start-vc.php

## Usage
Each audio has an ID, of type ```audioID_t```, which is just an ```std::uint32_t``` in the background. You load the audio, and simply pass the ID of the audio you which to control.

In the following example, I use the ```SoundManager``` class. Alternatively, you could use ```SoundStreamManager``` in the exact same way. You can find a list of functions in AudioManager.h.

```cpp
#include "SoundManager.h"

using typename AudioManager::audioID_t;

int main()
{
	AudioManager::SoundManager sm;
	audioID_t audioID = sm.Load("wavSound1.wav");

	sm.Play(audioID);
	while (sm.IsPlaying(audioID)) {} // Audio playing here
	sm.Unload(audioID);

	return 0;
}
```

Contact me for general enqueries here: leytonodayabc123@gmail.com
