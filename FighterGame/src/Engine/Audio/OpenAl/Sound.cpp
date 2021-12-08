#include "Sound.h"
#include <ios>
#include <iostream>
#include "AudioFile.h"
#include <AL/alc.h>
#include <AL/al.h>

struct RIFF_HEADER {
	uint32_t RIFF; //riff in ascii
	uint32_t chunkSize; //filesize - 8
	uint32_t type; //wave in ascii
};

struct WAVE_HEADER {
	uint32_t	subChunk1ID; //"fmt "
	uint32_t	subChunk1Size; //if 16, no extra parameters, else extra parameters
	uint16_t	audioFormat; //if 1, linear, else, im not going to read it
	uint16_t	numChannels; //channels
	uint32_t	sampleRate;
	uint32_t	byteRate;
	uint16_t	blockAlign;
	uint16_t	bitsPerSample;
};

struct WAVE_DATA {
	uint32_t	subChunk2ID; //"data"
	uint32_t	subChunk2Size; //number of data bytes
};

int AL_Sound::loadWave(const std::string& filepath)
{
	std::cout << "loading " + filepath << "\n";

	RIFF_HEADER riffhead;
	std::fstream stream;
	stream.read((char*)&riffhead, 12);

	if (riffhead.RIFF != 0x52494646) {
		std::cout << "ERROR!!! " + filepath + " is not a riff file!!!";
		return 0;
	}
	if (riffhead.type != 0x57415645) {
		std::cout << "ERROR!!! " + filepath + " is not a riff file!!!";
		return 0;
	}

	AudioFile<float> af;
	af.load(filepath);

	channels = af.getNumChannels();
	stereo = af.isStereo();
	mono = af.isMono();
	sampleRate = af.getSampleRate();
	bitsPerSample = af.getBitDepth();

	if (!(stereo || mono) || (sampleRate != 8 && sampleRate != 16))
	{
		std::cerr
			<< "ERROR: unrecognised wave format: "
			<< channels << " channels, "
			<< bitsPerSample << " bps" << std::endl;
		return 0;
	}

	uint32_t format = 0x1100 
		+ (stereo ? 2 : 0)
		+ (sampleRate == 16 ? 1 : 0);

	alGenBuffers(1, &m_bufferId);
}

AL_Sound::AL_Sound(const std::string& filepath)
{
	//extract the letters after the period
	char* period = (char*)filepath.c_str() + filepath.size();
	while (*(period--) != '.');
	std::string type = (period+1);
	
	if (type == "wave" || type == "wav") {
		loadWave(filepath);
	}
}
