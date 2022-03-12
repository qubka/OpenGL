#include "audioloader.hpp"

#include <al.h>
#include <alc.h>

/// WAV

int32_t convert_to_int(char* buffer, std::size_t len) {
    int32_t a;
    std::memcpy(&a, buffer, len);
    return a;
}

bool LoadWavHeaderFile(std::ifstream& file,
                                     uint8_t& channels,
                                     int32_t& sampleRate,
                                     uint8_t& bitsPerSample,
                                     ALsizei& size) {
    char buffer[4];
    if (!file.is_open())
        return false;

    // the RIFF
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read RIFF" << std::endl;
        return false;
    }

    if (std::strncmp(buffer, "RIFF", 4) != 0) {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
        return false;
    }

    // the size of the file
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read size of file" << std::endl;
        return false;
    }

    // the WAVE
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read WAVE" << std::endl;
        return false;
    }

    if (std::strncmp(buffer, "WAVE", 4) != 0) {
        std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
        return false;
    }

    // "fmt/0"
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read fmt/0" << std::endl;
        return false;
    }

    // this is always 16, the size of the fmt data chunk
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read the 16" << std::endl;
        return false;
    }

    // PCM should be 1?
    if (!file.read(buffer, 2)) {
        std::cerr << "ERROR: could not read PCM" << std::endl;
        return false;
    }

    // the number of channels
    if (!file.read(buffer, 2)) {
        std::cerr << "ERROR: could not read number of channels" << std::endl;
        return false;
    }
    channels = convert_to_int(buffer, 2);

    // sample rate
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read sample rate" << std::endl;
        return false;
    }
    sampleRate = convert_to_int(buffer, 4);

    // (sampleRate * bitsPerSample * channels) / 8
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
        return false;
    }

    // ?? dafaq
    if (!file.read(buffer, 2)) {
        std::cerr << "ERROR: could not read dafaq" << std::endl;
        return false;
    }

    // bitsPerSample
    if (!file.read(buffer, 2)) {
        std::cerr << "ERROR: could not read bits per sample" << std::endl;
        return false;
    }
    bitsPerSample = convert_to_int(buffer, 2);

    // data chunk header "data"
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read data chunk header" << std::endl;
        return false;
    }
    if (std::strncmp(buffer, "data", 4) != 0) {
        std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
        return false;
    }

    // size of data
    if (!file.read(buffer, 4)) {
        std::cerr << "ERROR: could not read data size" << std::endl;
        return false;
    }
    size = convert_to_int(buffer, 4);

    /* cannot be at the end of file */
    if (file.eof()) {
        std::cerr << "ERROR: reached EOF on the file" << std::endl;
        return false;
    }
    if (file.fail()) {
        std::cerr << "ERROR: fail state set on the file" << std::endl;
        return false;
    }

    return true;
}

std::vector<char> AudioLoader::LoadWav(const std::string& filename,
                                        uint8_t& channels,
                                        int32_t& sampleRate,
                                        uint8_t& bitsPerSample) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "ERROR: Could not open " << filename <<  std::endl;
        return {};
    }

    ALsizei size;
    if (!LoadWavHeaderFile(in, channels, sampleRate, bitsPerSample, size)) {
        std::cerr << "ERROR: Could not load wav header of " << filename << std::endl;
        return {};
    }

    std::vector<char> buffer(size);

    in.seekg(0);
    in.read(buffer.data(), size);

    in.close();

    return buffer;
}