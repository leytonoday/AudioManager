#include "SoundManager.h"

using typename AudioManager::audioID_t;

int main()
{
	AudioManager::SoundManager sm;
	audioID_t audioID = sm.Load("wavSound1.wav");

	sm.PlayAll();
	while (sm.IsPlaying(audioID)) {} // Audio playing here
	sm.UnloadAll();

	return 0;
}