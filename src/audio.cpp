#include "audio.hpp"
#include "openal.hpp"
#include "audioloader.hpp"

Audio::Audio(const std::string& path, bool loop, int pitch, float gain) {
    auto data = AudioLoader::LoadWav(path, channels, sampleRate, bitsPerSample);

    alCall(alGenBuffers, 1, &buffer);

    ALenum format;
    if (channels == 1 && bitsPerSample == 8)
        format = AL_FORMAT_MONO8;
    else if (channels == 1 && bitsPerSample == 16)
        format = AL_FORMAT_MONO16;
    else if (channels == 2 && bitsPerSample == 8)
        format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bitsPerSample == 16)
        format = AL_FORMAT_STEREO16;
    else {
        std::cerr << "ERROR: unrecognised wave format: "
                  << channels << " channels, "
                  << bitsPerSample << " bps" << std::endl;
        return;
    }

    alCall(alBufferData, buffer, format, data.data(), data.size(), sampleRate);

    alCall(alGenSources, 1, &source);
    alCall(alSourcef, source, AL_PITCH, pitch);
    alCall(alSourcef, source, AL_GAIN, gain);
    //alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
    //alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
    alCall(alSourcei, source, AL_LOOPING, loop);
    alCall(alSourcei, source, AL_BUFFER, buffer);
}

Audio::~Audio() {
    alCall(alDeleteSources, 1, &source);
    alCall(alDeleteBuffers, 1, &buffer);
}

void Audio::play() const {
    if (!isPlaying()) {
        alCall(alSourcePlay, source);
    }
}

void Audio::stop() const {
    if (isPlaying()) {
        alCall(alSourceStop, source);
    }
}

void Audio::pause() const {
    if (!isPaused()) {
        alCall(alSourcePause, source);
    }
}

/*void Audio::setListener(const glm::vec3& position) const {
    alCall(alListener3f, AL_POSITION, position.x, position.y, position.z);
    alCall(alListener3f, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    alCall(alListener3f, AL_ORIENTATION, orientation.x, orientation.y, orientation.z);
}*/

void Audio::setPosition(const glm::vec3& position) const {
    alCall(alSource3f, source, AL_POSITION, position.x, position.y, position.z);
}

void Audio::setVelocity(const glm::vec3& velocity) const {
    alCall(alSource3f, source, AL_POSITION, velocity.x, velocity.y, velocity.z);
}

void Audio::setLooping(bool loop) const {
    alCall(alSourcei, source, AL_LOOPING, loop);
}

void Audio::setPitch(int pitch) const {
    alCall(alSourcef, source, AL_PITCH, pitch);
}

void Audio::setGain(float gain) const {
    alCall(alSourcef, source, AL_GAIN, gain);
}

uint16_t Audio::getState() const {
    ALint state;
    alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    return state;
}

uint8_t Audio::getChannels() const {
    return channels;
}

int32_t Audio::getSampleRate() const {
    return sampleRate;
}

uint8_t Audio::getBitsPerSample() const {
    return bitsPerSample;
}

bool Audio::isPlaying() const {
    return getState() == AL_PLAYING;
}

bool Audio::isPaused() const {
    return getState() == AL_PAUSED;
}

bool Audio::isStopped() const {
    return getState() == AL_STOPPED;
}