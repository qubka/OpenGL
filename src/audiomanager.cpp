#include "audiomanager.hpp"
#include "audio.hpp"
#include "openal.hpp"

bool AudioManager::init() {
    device = alcCall(alcOpenDevice, nullptr);
    assert(device && "Failed to initialize OPENAL!");

    context = alcCall(alcCreateContext, device, nullptr);
    alcCall(alcMakeContextCurrent, context);

    return true;
}

void AudioManager::destroy() {
    sounds.clear();

    alcCall(alcMakeContextCurrent, nullptr);
    alcCall(alcDestroyContext, context);
    alcCall(alcCloseDevice, device);
}

bool AudioManager::load(const std::string& path) {
    if (sounds.find(path) != sounds.end())
        return false;

    sounds.emplace(path, std::make_unique<Audio>(path));
    return true;
}

void AudioManager::play(const std::string& path, const glm::vec3& position) {
    if (auto it = sounds.find(path); it != sounds.end()) {
        auto& sound{ *it->second };
        //sound.setPosition(position);
        sound.play();
    }
}

void AudioManager::stop(const std::string& path) {
    if (auto it = sounds.find(path); it != sounds.end()) {
        auto& sound{ *it->second };
        sound.stop();
    }
}