#pragma once

#include "vertex.hpp"

class Shader;
class Texture;

class Mesh {
public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::shared_ptr<Texture> texture);
    Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::vector<std::shared_ptr<Texture>>&& textures);
    ~Mesh();

    void render(const std::unique_ptr<Shader>& shader) const;

private:
    GLuint vao, vbo, ibo;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    void initMesh();

    friend class Model;
};
