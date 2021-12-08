#pragma
#include <AL/alc.h>
#include <AL/al.h>

namespace audio {
	ALCdevice* alcdevice;
	ALCcontext* alccontext;

	int init();
	int destroy();
}