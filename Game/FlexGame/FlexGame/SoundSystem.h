#pragma once

#include "fmod.hpp"

class SoundSystem
{
public:
	SoundSystem(void);
	~SoundSystem(void);
	
	
	void PlayBackground();
	void PlaySong();
	void StopSong();

	void Update();

private:
	void ERRCHECK(FMOD_RESULT result);
	void Initialize();

	FMOD_RESULT result;
	FMOD::System *system;
	FMOD::Channel *channel;
	FMOD::Sound *flux;
	FMOD::Sound *backgroundMusic;
	
};