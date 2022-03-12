#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "opengl.hpp"

#include <assimp/material.h>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::shared_ptr<Texture> texture)
    : vertices{std::move(vertices)}
    , indices{std::move(indices)}
{
    initMesh();
    textures.push_back(texture);
}

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::vector<std::shared_ptr<Texture>>&& textures)
    : vertices{std::move(vertices)}
    , indices{std::move(indices)}
    , textures{std::move(textures)}
{
    initMesh();
}

Mesh::~Mesh() {
    glCall(glDeleteVertexArrays, 1, &vao);
    glCall(glDeleteBuffers, 1, &vbo);
    glCall(glDeleteBuffers, 1, &ibo);
}

void Mesh::initMesh() {
    if (vertices.empty() || indices.empty())
        assert("Vertices/Indices data buffer is empty");

    glCall(glGenVertexArrays, 1, &vao);
    glCall(glGenBuffers, 1, &vbo);
    glCall(glGenBuffers, 1, &ibo);

    glCall(glBindVertexArray, vao);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);
    glCall(glBufferData, GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, ibo);
    glCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glCall(glEnableVertexAttribArray, 0);
    glCall(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

    glCall(glEnableVertexAttribArray, 1);
    glCall(glVertexAttribPointer, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    glCall(glEnableVertexAttribArray, 2);
    glCall(glVertexAttribPointer, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture));

    glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
    glCall(glBindVertexArray, 0);
}

void Mesh::render(const std::unique_ptr<Shader>& shader) const {
    uint8_t diffuseIdx = 0;
    uint8_t specularIdx = 0;
    uint8_t heightIdx = 0;
    uint8_t ambientIdx = 0;

    shader->setUniform("has_texture", !textures.empty());

    for (int i = 0; i < textures.size(); i++) {
        const auto& texture = textures[i];

        std::string name;
        switch (texture->getType()) {
            case aiTextureType_DIFFUSE:
                name = "diffuse" + std::to_string(diffuseIdx++);
                break;
            case aiTextureType_SPECULAR:
                name = "specular" + std::to_string(specularIdx++);
                break;

            case aiTextureType_HEIGHT:
                name = "height" + std::to_string(heightIdx++);
                break;

            case aiTextureType_AMBIENT:
                name = "ambient" + std::to_string(ambientIdx++);
                break;

            default:
                assert("Unknown texture type");
                return;
        }

        shader->setUniform(name, i);
        shader->setUniform("texture_scale", texture->getScale());
        texture->bind(i);
    }

    glCall(glBindVertexArray, vao);
    glCall(glDrawElements, GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
    glCall(glBindVertexArray, 0);

    for (const auto& texture : textures) {
        texture->unbind();
    }
    glCall(glActiveTexture, GL_TEXTURE0);
}