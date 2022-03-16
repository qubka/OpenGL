#pragma once

#include "model.hpp"

struct TransformComponent {
    glm::vec3 translation{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};

    operator glm::mat4() const {
        glm::mat4 m{1.0f};
        return glm::translate(m, translation)
               * glm::mat4_cast(glm::quat(rotation))
               * glm::scale(m, scale);
    };
};

struct ModelComponent {
    std::unique_ptr<Model> model;

    std::unique_ptr<Model>& operator()() { return model; }
    const std::unique_ptr<Model>& operator()() const { return model; }
};

struct MeshComponent {
    std::unique_ptr<Mesh> mesh;

    std::unique_ptr<Mesh>& operator()() { return mesh; }
    const std::unique_ptr<Mesh>& operator()() const { return mesh; }
};