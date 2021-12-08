#include <cstdint>
#include <vector>
#include <string>

class AL_Sound {
private:
	uint32_t m_bufferId;
	int loadWave(const std::string& filepath);
public:
	uint8_t channels;
	int32_t sampleRate;
	uint8_t bitsPerSample;
	bool stereo;
	bool mono;

	AL_Sound(const std::string& filepath);
};