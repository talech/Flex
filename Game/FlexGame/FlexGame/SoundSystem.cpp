#include "SoundSystem.h"
#include "fmod_errors.h"
#include <stdio.h>
#include <windows.h>

SoundSystem::SoundSystem(void) {
	Initialize();
}

SoundSystem::~SoundSystem(void) {
	result = flux->release();
	ERRCHECK(result);

	result = system->release();
    ERRCHECK(result);
}

void SoundSystem::ERRCHECK(FMOD_RESULT result) {
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void SoundSystem::Initialize() {
	result = FMOD::System_Create(&system);		// Create the main system object.
	ERRCHECK(result);

	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	ERRCHECK(result);

	result = system->createStream("../Data/Music/Flux.mp3", FMOD_LOOP_NORMAL, 0, &flux);
    ERRCHECK(result);

	result = system->createStream("../Data/Music/Lex.mp3", FMOD_LOOP_NORMAL, 0, &backgroundMusic);
    ERRCHECK(result);

}

void SoundSystem::PlayBackground() {
	result = system->playSound(FMOD_CHANNEL_FREE, backgroundMusic, false, 0);
    ERRCHECK(result);
}

void SoundSystem::PlaySong() {
	result = system->playSound(FMOD_CHANNEL_FREE, backgroundMusic, true, 0);
    ERRCHECK(result);
	
	result = system->playSound(FMOD_CHANNEL_FREE, flux, false, 0);
    ERRCHECK(result);
}

void SoundSystem::StopSong() {
	result = system->playSound(FMOD_CHANNEL_FREE, flux, true, 0);
    ERRCHECK(result);
}

void SoundSystem::Update() {
	system->update();
}