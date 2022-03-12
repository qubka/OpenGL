#pragma once

#include "audio.hpp"

class AudioManager
{
public:
    AudioManager() = default;
    ~AudioManager() = default;

    void init();
    void destroy();

    bool load(const std::string& path);

    void play(const std::string& path, const glm::vec3& position);
    void stop(const std::string& path);

private:
    ALCcontext* context;
    ALCdevice* device;

    std::unordered_map<std::string, std::unique_ptr<Audio>> sounds;

public:
    /// WAV
    static std::vector<char> LoadWav(const std::string& path, uint8_t& channels, int32_t& sampleRate, uint8_t& bitsPerSample);
};
