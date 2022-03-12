#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

struct Glyph {
    glm::vec2 advance;
    glm::vec2 size;
    glm::vec2 bearing;
    glm::vec2 uv;
};

class Font {
public:
    Font(const FT_Face& face, int size);
    ~Font();

    void bind() const;
    void unbind() const;

private:
    GLuint textureId;
    int width;
    int height;
    int metrics;
    std::map<unsigned char, Glyph> glyphs;

    friend class TextMesh;
};
