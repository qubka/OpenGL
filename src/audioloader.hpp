#pragma once

class AudioLoader {
public:
    /// WAV
    static std::vector<char> LoadWav(const std::string& filename, uint8_t& channels, int32_t& sampleRate, uint8_t& bitsPerSample);
};