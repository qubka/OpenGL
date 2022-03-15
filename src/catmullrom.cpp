#include "catmullrom.hpp"
#include "opengl.hpp"
#include "shader.hpp"

CatmullRom::CatmullRom() {
    glCall(glGenVertexArrays, 1, &vao);
    glCall(glGenBuffers, 1, &vbo);
    glCall(glGenBuffers, 1, &ebo);

    loadPoints();

    GLint maxPatchVertices = 0;
    glCall(glGetIntegerv, GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    std::cout << "Max supported patch vertices " << maxPatchVertices << std::endl;
}

CatmullRom::~CatmullRom() {
    glCall(glDeleteVertexArrays, 1, &vao);
    glCall(glDeleteBuffers, 1, &vbo);
    glCall(glDeleteBuffers, 1, &ebo);
}

void CatmullRom::loadPoints() const {
    glCall(glBindVertexArray, vao);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);
    glCall(glBufferData, GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, ebo);
    glCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glCall(glEnableVertexAttribArray, 0);
    glCall(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
    glCall(glBindVertexArray, 0);
}

void CatmullRom::render() const {
    if (vertices.empty() || indices.empty())
        return;

    glBindVertexArray(vao);

    // draw control points
    glCall(glPointSize, 7);
    glCall(glDrawArrays, GL_POINTS, 0, vertices.size());
}

void CatmullRom::tessellate() const {
    if (vertices.empty() || indices.empty())
        return;

    // draw tessallation
    glCall(glPatchParameteri, GL_PATCH_VERTICES, 4);
    glCall(glLineWidth, 100.f);
    glCall(glDrawElements, GL_PATCHES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);

    glBindVertexArray(0);
}

void CatmullRom::addPoint(const glm::vec3& point) {
    vertices.push_back(point);

    size_t n = vertices.size();
    if (n <= 4) {
        indices.push_back(n - 1);
    } else {
        indices.push_back(n - 4);
        indices.push_back(n - 3);
        indices.push_back(n - 2);
        indices.push_back(n - 1);
    }

    loadPoints();
}

void CatmullRom::addPoints(const std::vector<glm::vec3>& data) {
    for (auto& p : data) {
        vertices.push_back(p);

        size_t n = vertices.size();
        if (n <= 4) {
            indices.push_back(n - 1);
        } else {
            indices.push_back(n - 4);
            indices.push_back(n - 3);
            indices.push_back(n - 2);
            indices.push_back(n - 1);
        }
    }

    loadPoints();
}