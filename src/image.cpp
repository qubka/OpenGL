#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const std::string& path, bool flip) {
    assert(std::filesystem::exists(path) && "Could not load file: " && path.c_str());
    stbi_set_flip_vertically_on_load(flip);
    pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!pixels) {
        std::cerr << "ERROR: Failed to load image: \"" << path << "\" - " << stbi_failure_reason();
    }
}

Image::~Image() {
    stbi_image_free(pixels);
}