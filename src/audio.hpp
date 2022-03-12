#pragma once

class Audio {
public:
    Audio(const std::string& path, bool loop = true, int pitch = 1, float gain = 1.0f);
    ~Audio();

    void play() const;
    void stop() const;
    void pause() const;

    void setPosition(const glm::vec3& position) const;
    void setVelocity(const glm::vec3& velocity) const;
    void setLooping(bool loop) const;
    void setPitch(int pitch) const;
    void setGain(float gain) const;

    uint16_t getState() const;
    uint8_t getChannels() const;
    int32_t getSampleRate() const;
    uint8_t getBitsPerSample() const;

    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;

private:
    ALuint buffer;
    ALuint source;
    int32_t sampleRate;
    uint8_t channels;
    uint8_t bitsPerSample;
};