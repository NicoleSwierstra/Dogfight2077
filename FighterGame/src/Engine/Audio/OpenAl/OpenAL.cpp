#include "OpenAL.h"

int audio::init()
{
	alcdevice = alcOpenDevice(nullptr);
	if (!alcdevice) return 0;

	alccontext = alcCreateContext(alcdevice, nullptr);
	if (!alccontext) return 0;

	alcMakeContextCurrent(alccontext);
	
	return 1;
}

int audio::destroy()
{
	if(alcdevice)
		alcCloseDevice(alcdevice);
}
